# An Overview of Quickstep's Execution Engine

## Types of threads
There are two kinds of threads in Quickstep - Foreman and Worker. The foreman
thread controls the query execution progress, finds schedulable work (called as
WorkOrder) and assigns (or schedules) it for execution to the Worker threads. 
The Worker threads receive the WorkOrders and execute them. After execution they
send a completion message (or response message) back to Foreman. 

## High level functionality of Foreman
Foreman requests all the RelationalOperators in the physical query plan 
represented as a DAG to give any schedulable work (in the form of WorkOrders).
While doing so, Foreman has to respect dependencies between operators. There are
two kinds of dependencies between operators - pipeline breaking (or blocking) 
and pipeline non-breaking (or non-blocking). In the first case, the output of
the producer operator can't be pipelined to the consumer operator. In the second
case, the Foreman will facilitate the pipelining of the intermediate output
produced by the producer operator to the consumer operator. 

## Messages in execution engine

### WorkerMessage
There are multiple types of WorkerMessage, each of which indicates the purpose
of the message.

Foreman -> Worker : WorkerMessage which consists of the following things
- A pointer to the WorkOrder to be executed. The WorkOrder could be a normal
WorkOrder or a rebuild WorkOrder. A normal WorkOrder involves the invocation of
WorkOrder::execute() method which is overriden by all of the RelationalOperator 
classes. A rebuild WorkOrder has one StorageBlock as input and calls a 
rebuild() method on the block. More details about rebuild() can be found in the
storage module. 
- The index of the relational operator in the query plan DAG that produced the
WorkOrder. 

Main thread -> Worker : WorkerMessage of type PoisonMessage. This message is
used to terminate the Worker thread, typically when shutting down the Quickstep
process. 

### ForemanMessage
Multiple senders are possible for this message. There are multiple types of 
ForemanMessages, each of which indicates the purpose of the message.

Worker -> Foreman : ForemanMessage of types WorkOrderCompletion and 
RebuildCompletion are sent after a Worker finishes executing a respective type 
of WorkOrder. This message helps the Foreman track the progress of individual 
operators as well as the whole query. 

Some relational operators and InsertDestination -> Foreman : ForemanMessage of
types DataPipeline and WorkOrdersAvailable. InsertDestination first determines 
when an output block of a relational operator gets full. Once a block is full,
it streams the unique block ID of the filled block along with the index of the 
relational operator that produced the block to Foreman with the message type 
DataPipeline. Some operators which modify the block in place also send similar
messages to Foreman. 

### FeedbackMessage
This message is sent from Workers to the Foreman during a WorkOrder execution.

In certain operators, e.g. TextScan (used for bulk loading data from text files)
and Sort, there is a communication between the relational operator and its 
WorkOrders. In such cases, when a WorkOrder is under execution on a Worker 
thread, a FeedbackMessage is sent from the WorkOrder via the Worker to Foreman.
Foreman relays this message to the relational operator that produced the sender
WorkOrder. The relational operator uses this message to update its internal 
state to potentially generate newer WorkOrders. 

## How does the Foreman react after receiving various messages?
### WorkOrder completion message
* Update the book-keeping of pending WorkOrders per Worker and per operator.
* Fetch new WorkOrders if available for the operator of whose WorkOrder was
just executed.  
* Update the state of an operator - the possible options are:
  - Normal WorkOrders are still under execution
  - All normal WorkOrders have finished execution and rebuild WorkOrders are yet
  to be generated. 
  - All normal WorkOrders have finished execution, rebuild WorkOrders have been
  generated and issued to Workers. 
  - All normal and rebuild WorkOrders have been executed AND all the dependency
  operators for the given operator have finished execution, therefore the given 
  operator has finished its execution. 
* Fetch the WorkOrders from the dependents of the given operator. 

### Rebuild WorkOrder completion message
* Update the book-keeping of pending WorkOrders per Worker and per operator.
* If all the rebuild WorkOrders have finished their execution, try to fetch the
WorkOrders of the dependent operators of the operator whose rebuild WorkOrder
was just executed. 

### Data pipeline message
* Find the consumer operators (i.e. operators which have a non 
pipeline-breaking link) of the producer operator. 
* Stream the block ID to the eligible consumer operators. 
* Fetch new WorkOrders from these consumer operators which may have become 
available because of the streaming of data. 

### WorkOrder available message
* Fetch new WorkOrders that may have become available.

### Feedback message
* Relay the feedback message to a specified relational operator. The recipient 
operator is specified in the header of the message. 

## Example
We look at a sample query to better describe the flow of messages - 

SELECT R.a, S.b from R, S where R.a = S.a and R.c < 20;

This is an equi-join query which can be implemented using a hash join. We assume 
that S is a larger relation and the build relation is the output of the 
selection on R.

The query execution plan involves the following operators:
* SelectOperator to filter R based on predicate R.c < 20 (We call the output as 
R') 
* BuildHashOperator to construct a hash table on R'
* HashJoinOperator to probe the hash table, where the probe relation is S
* DestroyHashTableOperator to destroy the hash table after the join is done
* Multiple DropTableOperators to destroy the temporaray relations produced as 
output. 

R has two blocks with IDs as 1 and 2. S has two blocks with IDs as 3 and 4.
We assume that the SelectOperator produces one filled block and one partially 
filled block as output. Note that in the query plan DAG, the link between 
SelectOperator and BuildHashOperator allows streaming of data. The 
HashJoinOperator's WorkOrder can't be generated unless all of the 
BuildHashOperator's WorkOrders have finished their execution. The execution is
assumed to be performed by a single Worker thread. 

The following table describes the message exchange that happens during the 
query excution. We primarily focus on three operators - Select, BuildHash and 
HashJoin (probe). 

| Sender | Receiver | Message  | Message Description |
|:-----------------:|----------|---------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Foreman | Worker | WorkerMessage of type kWorkOrderMessage | SelectWorkOrder on block 1. |
| InsertDestination | Foreman | ForemanMessage of type kDataPipeline | SelectWorkOrder on block 1 produced one fully filled block as output.  The output block ID as pipelined from the InsertDestination to Foreman.  Foreman relays this block ID to BuildHashOperator, which generates a WorkOrder which is ready to be scheduled. |
| Worker | Foreman | ForemanMessage of type kWorkOrderCompletion | SelectWorkOrder on block 1 completed. |
| Foreman | Worker | WorkerMessage of type kWorkOrderMessage | SelectWorkOrder on block 2. |
| Worker | Foreman | ForemanMessage of type kWorkOrderCompletion | SelectWorkOrder on block 2 completed.  As a result of this execution, a partially filled block of output was produced.  |
| Foreman | Worker | WorkerMessage of type kWorkOrderMessage | BuildHashWorkOrder on the fully filled block of R'  |
| Worker | Foreman | ForemanMessage of type kWorkOrderCompletion | BuildHashWorkOrder execution complete.  |
| Foreman | Worker | WorkerMessage of type kWorkOrderMessage | BuildHashWorkOrder on the partially filled block of R' |
| Worker | Foreman | ForemanMessage of type kWorkOrderCompletion | BuildHashWorkOrder execution complete. |
| Foreman | Worker | WorkerMessage of type kWorkOrderMessage | HashJoinWorkOrder for block 3 from S |
| Worker | Foreman | ForemanMessage of type kWorkOrderCompletion | HashJoinWorkOrder execution complete. |
| Foreman | Worker | WorkerMessage of type kWorkOrderMessage | HashJoinWorkOrder for block 4 from S |
| Worker | Foreman | ForemanMessage of type kWorkOrderCompletion | HashJoinWorkOrder execution complete. |

