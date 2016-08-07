#Generator functions

##What are generator functions?
Generator functions are functions that can return a sequence of values rather than a single value. In [many programming languages](https://en.wikipedia.org/wiki/Generator_%28computer_programming%29), they look like functions but behave like iterators.

In Quickstep, a generator function takes a list of constant arguments at query compile time, and populates a relation at query run time. One example is the `generate_series` function, which in the simplest case takes two integer arguments `(start, end)` and generates a single-column table with values `start, start+1, ..., end`.

##Use generator functions
Usage of a generator function is similar to that of a normal function except that it is placed inside a SQL query's `FROM` clause.

For example:
```
SELECT *
FROM generate_series(1, 5);
```
And the output will be:
```
+---------------+
|generate_series|
+---------------+
|              1|
|              2|
|              3|
|              4|
|              5|
+---------------+
```
Here `generate_series(1, 5)` stands for a relation with 5 rows.

A more complex example is:
```
SELECT i, j, i*j
FROM generate_series(1, 3) AS gs1(i),
     generate_series(1, 3) AS gs2(j)
WHERE i <= j;
```
It is a theta-join between two generated relations. The output will be:
```
+-----------+-----------+-----------+
|i          |j          |(i*j)      |
+-----------+-----------+-----------+
|          1|          1|          1|
|          1|          2|          2|
|          1|          3|          3|
|          2|          2|          4|
|          2|          3|          6|
|          3|          3|          9|
+-----------+-----------+-----------+
```

##Overview of the design that supports the feature
1. Given a SQL query, at first each generator function call is parsed as an abstract syntax tree node carrying the function's name and arguments information.

 - *See `ParseGeneratorTableReference` as the abstract syntax tree node for generator functions.*

2. Then, in `Resolver`, the generator function is resolved and its arguments are validated. If this process is successful, we obtain a function handle that contains the actual implementation of this function. A logic plan node is then created to hold the function handle and to represent the relation that this function will generate.

 - *See `GeneratorFunctionFactory` as the factory class that resolves a function from its name.*
 - *See `GeneratorFunction` as the abstract function class that checks the validity of arguments and creates the function handle.*
 - *See `GeneratorFunctionHandle` as the function handle class that provides the actual implementation of a particular generator function.*
 - *See `logical::TableGenerator` as the logical plan node class that represents the relation that a generator function will create.*

3. The logical plan node `logical::TableGenerator` is then transformed into a physical plan node `physical::TableGenerator`.
 - *See `physical::TableGenerator` as the physical plan node class that represents the relation that a generator function will create.*

4. At the final stage of query planning, the physical plan node is transformed into a relational operator.
 - *See `TableGeneratorOperator` as the relational operator class that represents a operator that will produce run-time work orders.*

5. At the time of query execution, the function handle's `populateColumns()` method is invoked to actually generate a relation.
 - *See `TableGeneratorWorkOrder` as the work order class that actually invokes the generator function to populate a relation at run time.*

##Implement a new generator function
There are just three steps to implement a new generator function.

1. Subclass **`GeneratorFunctionHandle`** and implement all virtual methods.
 - *See `GenerateSeriesHandle` as an example.*

2. Subclass **`GeneratorFunction`** and implement all virtual methods. Also define a static `Instance()` method in the subclass to return a singleton instance reference.
 - *See `GenerateSeries` as an example.*

3. Register the **`GeneratorFunction`** subclass into **`GeneratorFunctionFactory`** by adding into `GeneratorFunctionFactory`'s constructor a new line:
```
GeneratorFunctionFactory::GeneratorFunctionFactory() {
  ...
  // Register all generator functions here.
  REGISTER_GENERATOR_FUNCTION_(YourGeneratorFunctionSubclass);
  ...
}
```
