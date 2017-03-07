# Apache Quickstep (Incubating)

[![Travis Widget]][Travis]

[Travis]: https://travis-ci.org/apache/incubator-quickstep
[Travis Widget]: https://travis-ci.org/apache/incubator-quickstep.svg?branch=master

## What is Quickstep?
Apache Quickstep is high-performance database engine designed to exploit the full potential of hardware that is packed in modern computing boxes (servers and laptops). The initial version (available now!) targets single-node in-memory environments. If your data spills overs the memory limit Quickstep will still work, so you don't have to obsessively worry about the in-memory part. Also, if your working set fits in memory then Quickstep will transparently and automatically figure that out, and cache that hot set to  deliver in-memory performance.

Distributed execution is the next big feature for Quickstep.

Quickstep began life in 2011 as a
[research project at the University of Wisconsin](https://www.cs.wisc.edu/~jignesh)
and entered incubation at the
[Apache Software Foundation](https://www.apache.org) in April, 2016.

## Why Quickstep?
Did you know that the hardware that you have in your laptop was spread across a small cluster just a decade ago? (PS: Hopefully you are not using a very old laptop!) If you look at a high-end server box, then that packs compute and storage power that was a full rack about 5 years ago! And, the way hardware technology is going, that box is going to become even more powerful in the future. In fact, it is likely that the computing power in each box is going to grow faster than other hardware components (e.g. networking) in data centers. So, if you care about performance and/or total operating costs, paying attention to single box performance is likely to be super important in the long run.

In other words there is a small data center in an individual compute boxes today! Quickstep aims to allow you to fully exploit the potential of that data center that is hidden in each individual box today. We call this the **scaling-in approach**, and it complements a scaling-out approach. But without scaling-in, you are overpaying (by a lot!) when you run your data service.

## What are the key ingredients?

Modern computing boxes contain a large number of computing cores and large main memory configuration. Quickstep allows you to fully exploit these hardware resources using novel data processing, data storage, and query processing methods that include:

1. A unique **decoupling of data-flow from control-flow** for query execution that allows for unlimited intra and inter-query parallelism. Thus, using all the processing core effectively.

2. A **template meta-programming** framework that provides fast vectorized query execution. Thus, using each processor cycle very efficiently.

3. A **hybrid data storage** architecture that includes columnar and row-store. Yes, this may surprise some of you, but sometimes a row-store beats a column-store!

And, it is **open source!**

## Giving it a spin

1. Checkout the code: ```git clone https://git-wip-us.apache.org/repos/asf/incubator-quickstep.git quickstep```
2. Then, go to the code directory: ```cd quickstep```
3. Initialize the dependencies: ```git submodule init```
4. Checkout the dependencies: ```git submodule update```
5. Download additional third-party dependencies and apply patches:<br/>
```cd third_party && ./download_and_patch_prerequisites.sh && cd ../```
6. Go into the build directory: ```cd build```
7. Create the Makefile: ```cmake -D CMAKE_BUILD_TYPE=Release ..```
8. Build: ```make -j4```. Note you may replace the 4 with the number of cores
   on your machine.
9. Start quickstep: ```./quickstep_cli_shell --initialize_db=true```. You can
   now fire SQL queries. To quit, you can type in ```quit;``` Your data is
   stored in the directory ```qsstor```. Note the next time you start Quickstep,
   you can omit the ``` --initialize_db``` flag (as the database has already
   been initialized), and simply start Quickstep as: ```./quickstep_cli_shell```.
   There are also a number of optional flags that you can specify, and to see
   the full list, you can type in: ```./quickstep_cli_shell --help```
10. Next let us load some data and fire some queries. A few points to note:
The SQL surface of Quickstep is small (it will grow over time). The
traditional SQL CREATE TABLE and SELECT statements work. The data types
that are supported include INTEGER, FLOAT, DOUBLE, VARCHAR, CHAR, DATE,
and DATETIME. Quickstep also does not have support for NULLS or keys (yet).
Let create two tables by typing into the Quickstep shell (which you opened
in the step above.

```
CREATE TABLE Weather (cid INTEGER, recordDate DATE, highTemperature FLOAT, lowTemperature FLOAT);
```

and then,

```
CREATE TABLE City (cid Integer, name VARCHAR(80), state CHAR(2));
```

10. Next, let us insert some tuples in these two tables.
    ```
    INSERT INTO City VALUES (1, 'Madison', 'WI');
    INSERT INTO City VALUES (2, 'Palo Alto', 'CA');
    INSERT INTO Weather VALUES (1, '2015-11-1', 50, 30);
    INSERT INTO Weather VALUES (1, '2015-11-2', 51, 32);
    INSERT INTO Weather VALUES (2, '2015-11-1', 60, 50);
    ```

11. Now we can issue SQL queries such as:
  a. Find all weather records for California:
  ```
  SELECT * FROM WEATHER W, City C WHERE C.cid = W.cid AND C.state = 'CA';
  ```

  b. Find the min and max temperature for each city, printing the ```cid```:
  ```
  SELECT cid, MIN(lowTemperature), MAX(highTemperature) FROM Weather GROUP BY cid;
  ```

  c. Find the min and max temperature for each city using a nested query, and
     printing thie city name:
  ```
  SELECT * FROM City C, (SELECT cid, MIN(lowTemperature), MAX(highTemperature) FROM Weather GROUP BY cid) AS T WHERE C.cid = T.cid;
  ```

12. Quickstep also supports a COPY TABLE command. If you want to try that, then
    from a separate shell file type in the following:

    ```
    echo "3|2015-11-3|49|29" > /tmp/tmp.tbl
    echo "3|2015-11-4|48|28" >> /tmp/tmp.tbl
    echo "3|2015-11-5|47|27" >> /tmp/tmp.tbl
    ```

    Then, load this new data by typing the following SQL in the Quickstep shell:

    ```
    COPY Weather FROM '/tmp/tmp.tbl' WITH (DELIMITER '|');
    ```

    Now, you have loaded three more tuples into the Weather table, and you can
    fire the SQL queries above again against this modified database.

    Remember, to quit Quickstep, you can type in ```quit;``` into the Quickstep
    shell.


## Additional pointers

1. For other build options, see the more comprehensive [build guide](BUILDING.md).
2. To get started as a developer, you should start with the [code organization guide](DEV_README.md).


## Licensing

Quickstep is licensed under the Apache License, Version 2.0. See [LICENSE](https://github.com/apache/incubator-quickstep/blob/master/LICENSE) for the full license text.

## Disclaimer
Apache Quickstep is an effort undergoing incubation at the Apache Software
Foundation (ASF), sponsored by the Apache Incubator PMC.

Incubation is required of all newly accepted projects until a further
review indicates that the infrastructure, communications, and decision
making process have stabilized in a manner consistent with other
successful ASF projects.

While incubation status is not necessarily a reflection of the
completeness or stability of the code, it does indicate that the
project has yet to be fully endorsed by the ASF.
