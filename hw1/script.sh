#!/bin/bash
for ((i=1;i<=306000;i=i*2));
	do
	./minicat -o output.txt -b $i romeo2 romeo ch42 >> timing.txt
done
