#!/bin/sh
# This shell script is based off of MakeTree.bat and is used for creating
# the output directories for the intersection locator's current driver.

# Author: Austin Hartman
# Date: 11/26/04

# $Id: MakeTree.sh,v 1.1 2004/11/26 17:37:49 ahartman Exp $

USAGE="Use the command: $0 <directory>"

if [ $# -ne 1 ]; then
    echo $USAGE
else
    mkdir $1/Unfiltered
    mkdir $1/Filter_10DF
    mkdir $1/Filter_10VMF
    mkdir $1/Filter_20DF
    mkdir $1/Filter_20VMF
    mkdir $1/Filter_30DF
    mkdir $1/Filter_30VMF
    mkdir $1/Filter_40DF
    mkdir $1/Filter_40VMF
    mkdir $1/Filter_50DF
    mkdir $1/Filter_50VMF
    echo Done
fi

