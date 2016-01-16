#!/bin/bash

flex -i -I --header-file=SqlLexer_gen.hpp -oSqlLexer_gen.cpp ../SqlLexer.lpp
bison -d -o SqlParser_gen.cpp ../SqlParser.ypp
