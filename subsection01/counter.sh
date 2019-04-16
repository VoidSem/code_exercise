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

#caculate total file size
ls -l -R $dir | grep total \
                | awk 'BEGIN{sum = 0} \
                      {sum += $2} \
                       END{print "total: ", sum, "Bytes"}'

#caculate large file sum
ls -l -R | awk 'BEGIN{{sum = 0} {mSize = 1024 * 1024}}\
                /^-/{if ($5 > mSize)\
                 {let sum++ }}\
                     END{print "large file total: "sum}'
