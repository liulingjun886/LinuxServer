#!/bin/bash
killall server
sleep 2
./server -t1 -s0 
sleep 1 
./server -t2 -s0
sleep 1
./server -t3 -s0
sleep 1
./server -t4 -s0 
sleep 1
./server -t5 -s0
