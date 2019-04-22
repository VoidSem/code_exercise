#!/bin/bash
# fibonacci.sh
# rm(with --clean)
# liuxueneng@airfly
# 2019/04/15
# modify 2019/04/22

#rm cmd
if [ XX$1 == "XX--clean" ]; then
    rm *.bin -rf
    exit
fi

ppreS=0
preS=1

dd if=/dev/zero of=0001.bin bs=1 count=1 &>> /dev/null

for i in $(seq 2 12)
do
        S=$[$ppreS+$preS]
        #echo $S
        ppreS=$preS
        preS=$S
        dd if=/dev/zero of=`printf %04d $i`.bin bs=$S count=1 &>> /dev/null
done

