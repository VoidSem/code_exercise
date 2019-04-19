#!/bin/bash
# fibonacci.sh
# create or rm(with --clean) files
# liuxueneng@airfly
# 2019/04/15

fileCount=45
fileName=0001.bin
fileSize=1

# fibonacci
#
#        | 0,                   n = 0
# f(n) = | 1,                   n = 1
#        | f(n-1) + f(n-2),     n >= 2
#
#
function FuncFibonacci()
{
    local tmp1=0
    local tmp2=1
    local tmp=1
    local i=2

    if [ $1 -lt 2 ]; then
        return $tmp
    fi

    # f(n) = f(n-1) + f(n-2)
    while [ $i -le $1 ]
    do
        tmp=$(($tmp1 + $tmp2))
        tmp1=$tmp2
        tmp2=$tmp
        let i++
    done
    fileSize=$tmp
}


#start run
for i in $(seq 1 $fileCount)
do

    curNum=`printf "%04d\n" $i`
    fileName=$curNum.bin

    case "$1" in
        --clean)
            if [  -f "$fileName" ]; then
                rm $fileName
            fi
            ;;
        *)
            #get file size
            FuncFibonacci $i

            #create and resize file
            dd if=/dev/zero of=$fileName bs=$fileSize count=1 &>> /dev/null
            ;;
    esac

done

