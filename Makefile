#!/bin/bash
# 
# Folder structure
# project/ 			-	Project folder
# project/src			- 	All .cpp files (sub folders also included)
# project/inc			-	All .h files
#
# make clean - to delete all output files
# make build - to build only
# make all   - to compile and link
# make run   - to compile, link and run 
#

include Makefile.vars

default: all

include Makefile.in
