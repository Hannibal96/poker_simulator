#!/usr/bin/bash

g++ ./../main.cpp ./../agents_utils/*.cpp ./../agents_utils/*.h ./../poker_utils/*.cpp ./../poker_utils/*.h ./../deck_utils/*.cpp ./../deck_utils/*.h ./../jsons_utils/*.cpp ./../jsons_utils/*.h -o simulator.exe

echo "-I- simulator"

g++ -g ./../main.cpp ./../agents_utils/*.cpp ./../agents_utils/*.h ./../poker_utils/*.cpp ./../poker_utils/*.h ./../deck_utils/*.cpp ./../deck_utils/*.h ./../jsons_utils/*.cpp ./../jsons_utils/*.h -o simulator_debug.exe

echo "-I- simulator_debug"
