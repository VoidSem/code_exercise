#!/bin/bash
# counter.sh
# caculate total file size 
# liuxueneng@airfly
# 2019/04/16


dir=./

:<<@
check user arg
@

if [ -d $1 ]
then
    dir=$1
else
    echo $1 "is not a dir"
    exit -1
fi

:<<@
#caculate large file sum
ls -l -R $dir | awk 'BEGIN{{sum = 0} {mSize = 1024 * 1024}}\
                /^-/{if ($5 > mSize)\
                 {let sum++ }}\
                     END{print "large file total: "sum}'
@

#caculate total file size
ls -l -R $dir | awk 'BEGIN{{sum = 0} {largeSum = 0} {mSize = 1024 * 1024}} \
                /^-/{ {sum += $5} \
                        {if ($5 > mSize) { let largeSum++}}} \
                END{{print "totalSize: ", sum, "Bytes"}\
                       {print "totalLargeFile: ", largeSum}}'
