#!/usr/bin/bash


for i in {1..5}
do
	./simulator.exe -j MWU.json --seed ${i} --out MWU_${i}.log &
	./simulator.exe -j Bandits.json --seed ${i} --out Bandits_${i}.log &
done


