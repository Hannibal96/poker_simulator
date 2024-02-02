#!/usr/bin/bash


for i in {1..5}
do
	./simulator_db.exe -j MWU.json --seed ${i} --out MWU_debug_${i}.log &
done


