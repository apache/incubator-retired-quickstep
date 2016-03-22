# Window aggregation #

## Design
* Foreman changes:
    * Added an API to register for a single timer to call
      [`RelationalOperator::getAllWorkOrders()`](relational_operators/RelationalOperator.hpp).
    * Operators that need timed calls to get all work-orders need to implement
      [`RelationalOperator::registerTimeWorkOrderRequest()`](relational_operators/RelationalOperator.hpp) to return the
      time period of requests. (It defaults to no timer.)
    * Foreman queries all the operators in the DAG to get the timed get all work-orders requests (if any). It then
      initializes a heap of timer events ordered by the firing time. Foreman then periodically checks for timer events
      that need to be run, executes them if necessary, and updates the firing time for the next cycles.
* [Window aggregation operator](relational_operators/WindowAggregationOperator.hpp) changes:
    * Performs aggregation on tumbling windows specified by the window size/duration. (Other forms of window aggregation
      like rolling windows or SQL windows are not implemented.)
    * Optionally can take a `PARTITION BY` list of expressions to perform aggregation on pairs specified by window and
      `PARTITION BY` tuple.
    * The operator in the current state can operate on the blocks fed to it, and outputs the tuples for windows that
      changed due to the current block.
    * Aging of windows is currently **not** implemented. This can be implemented by:
        * adding functionality to delete entries in the hash tables (which is currently not supported)
        * by maintaining two hash tables which hold windows in two aging duration, and throwing away a hash table that
          exceeds the aging duration and creating a new one.
* [Monitored text scan operator](relational_operators/MonitoredTextScanOpeartor.hpp) changes:
    * Sets up a timer for get all work-orders request.
    * Everytime a get all work-orders request is called, it scans a hard-coded folder for files with new tuples to insert.
    * This serves as a mechanism for streaming data into the system.
    * The hard coded folder is `/tmp/<table-name>-input`.
    * Input files are text files with columns separated by `|`.
* Optimizer and parser changes:
    * New clause (WINDOW) is added to the select statement in the parser. See the query format for syntax details.
    * We have added a logical and physical plan nodes for window aggregates, and there is a one-to-one strategy to map
      the logical plan node to physical plan node.
    * For now, the execution generator is instrumented to the generate a operator tree fragment containing
      monitored-text-scan-operator followed by window-aggregation-operator to print-to-screen-operator for a
      window-aggregation physical plan node.
    * Currently, Quickstep regenerates the temporary catalog relation as a result of query execution, and then outputs
      the entire relation to standard output. However, that will not work for streaming case, since output is produced
      on the fly. Thus, we, ideally, need to add a print-to-screen operator to the top of the operator DAG to print tuples as they
      are being generated.

## Query format

### Simple no-grouping
```
SELECT <aggregation-expression-list> FROM <table>
WINDOW <window-duration> ON <window-attribute>
EMIT <emit-duration>
AGE <age-duration>
```

For example, on a table `R(A int, B int, C int)`:

```
SELECT SUM(B), AVG(C) FROM R
WINDOW 10 ON A -- Window of size 10 on the A values, so create windows in the range [0, 10) [10, 20) ...
EMIT 5         -- Every 5 secs.
AGE 60         -- Every 60 secs.
```

```
SELECT SUM(B), AVG(C) FROM R
WINDOW 10 ON A             -- Window of size 10.
EMIT INTERVAL '5 seconds'  -- Every 5 secs.
AGE INTERVAL '60 seconds'  -- Every 60 secs.
```

### Grouping window aggregates.

```
SELECT <aggregation-expression-list> FROM <table>
WINDOW <window-duration> ON <window-attribute>
PARTITION BY <grouping-expression-list>
EMIT <emit-duration>
AGE <age-duration>
```

For example, on a table `R(A int, B int, C int)`:

```
SELECT AVG(C) FROM R
WINDOW 10 ON A -- Staggered window of size 10 on the A values.
PARTITION BY B -- Grouping expression.
EMIT 5         -- Every 5 secs.
AGE 60         -- Every 60 secs.
```

```
SELECT AVG(C) FROM R
WINDOW 10 ON A             -- Staggered window of size 10 on the A values.
PARTITION BY B -- Grouping expression.
EMIT INTERVAL '5 seconds'  -- Every 5 secs.
AGE INTERVAL '60 seconds'  -- Every 60 secs.
```

## Simple demo

1. Build quickstep.
   * `cd build; cmake ..; make -j`
2. Fire up quickstep_cli_shell.
   * `./quickstep_cli_shell`
3. In CLI:
   * Create table:
     * `CREATE TABLE R (A INT, B INT, C INT);` 
   * Spawn the window aggregate query:
     * `SELECT A, AVG(C) FROM R WINDOW 2 ON B PARTITION BY A EMIT 5 AGE 60;`
   * Now place text files at `/tmp/R-input/` folder. Quickstep looks at `/tmp/<table-name>-input` folder for new files.
     * In some bash terminal, create this helper function to create synthetic files:
      * `write_file() { k=$1; for i in 1 2; do for j in $(seq 0 5); do echo "$i|$j|$(($i*$j*$k))" >>/tmp/R-input/R.$k; done; done }`
     * Add files to folder by running `write_file <seed-value>`:
      * `write_file 1`
      * `write_file 2`
      * ...

## Complex demo
TODO(quickstep-team): Looking at replaying hourly logs number of page edits for Wikipages (or Wikipage projects) from
http://dumps.wikimedia.org/other/pagecounts-raw/.
