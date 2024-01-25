#!/bin/bash

#g++ *.cpp *.h -std=c++17 -o ./exe/simulator.exe

BB=0.25
SB=0.1
AI=2.0

for i in {1..10}
do
	./../simulator.exe --CO Bandit_0.5_0.01_0.9_0.5_10000 --DE Bandit_0.5_0.01_0.9_0.5_10000 --SB Bandit_0.5_0.01_0.9_0.5_10000 --BB_CO Bandit_0.5_0.01_0.9_0.5_10000 --bb ${BB} --sb ${SB} --all_in ${AI} --print 1000000 --repeat 10 --rounds 10000000 > bb=${BB}_sb=${SB}_allin=${AI}_${i}.log &
done


