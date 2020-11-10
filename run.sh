#!/bin/bash
./server 1 1 data_1 7788
sleep 1 
./server 2 1 conn_1 6666 8080
sleep 1
./server 3 1 niu_1 11001 
