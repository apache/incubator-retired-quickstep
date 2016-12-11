---
layout: post
title:  "Your First Query"
date:   2016-12-10 12:29:09 -0600
categories: guides
author: Marc
short: Building software from source can be tricky, especially if you've never used the project's build tools before. In this post, we'll step through how to get started with Quickstep.
---
For this tutorial, I'm going to assume you're running in a unix environment. If you're having trouble building on Windows, try asking the dev community (<dev@quickstep.incubating.apache.org>). 

If you're going to build Quickstep, you'll first need to clone it from Github and initialize the submodules

{% highlight bash %}
git clone https://github.com/apache/incubator-quickstep.git .
cd incubator-quickstep
git submodule init
git submodule update
{% endhighlight %}

Next, you'll need to generate Makefiles using CMake, a cross-platform build tool that's popular for building c++ projects. You'll need a version at least as new as **2.8.6**. (You can check with `cmake -v`). If you have a later version, try generating the Makefiles like

{% highlight bash %}
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
{% endhighlight %}

Note the weird flag `-DCMAKE_BUILD_TYPE=Debug`. This is telling cmake to use, you guessed it, a debug configuration for the build. This automatically includes flags in the generated Makefiles to include debug symbols and turn on optional checks and log statements in the code. There's tons more flags, but only a few which we commonly use. Mostly I just change the  `CMAKE_CXX_COMPILER` and `CMAKE_C_COMPILER`. These flags control the compiler and I prefer clang++ and clang.

If cmake ran successfully you can now build quickstep. If it didn't run successfully, then don't worry, we're always finding small issues. Like, on one of my linux test machines, I always have to delete lines related to `gflag` in the `third_party/glog/CMakeLists.txt` file because of a misconfiguration on that machine. However, on my mac, cmake runs fine.

If you're having trouble, one solution might be to install some dependencies. If you're running Ubuntu, this might help:

{% highlight bash %}
sudo apt-get install -y build-essential protobuf-compiler libprotobuf-dev flex bison libnuma-dev
{% endhighlight %}

If that still doesn't help, email the dev list. Seriously, we're nice.

Assuming you've generated the Makefiles, you can now build quickstep and all of the unit tests. This will run faster if we just build quickstep.

{% highlight bash %}
# the -j option is how many workers to assign to the compilation
make -j4 quickstep_cli_shell
{% endhighlight %}

Once quickstep builds, you should now be able to run your first query. You'll need to tell quickstep where to store its files.

{% highlight bash %}
./quickstep_cli_shell --initialize_db=true --storage_path=store
{% endhighlight %}

This will initialize a directory called `store` where quickstep will persist data. If this ran successfully, then you should be seeing the command line prompt. For more information on what flags are available, enter `./quickstep_cli_shell --help`.

Now we can run our first query! Let's create a bunch of records and then aggregate over them.

{% highlight sql %}
CREATE TABLE my_numbers (i INT, j INT);
INSERT INTO my_numbers SELECT k % 10, ((k % 1969) * 1337) / 3 FROM generate_series(0, 1000000) AS gs(k);
SELECT i, AVG(j) FROM my_numbers GROUP BY i;
{% endhighlight %}

Of course, that query is meaningless but it should give you some idea of the sophistication of the SQL interface.

