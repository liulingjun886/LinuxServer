#!/bin/bash
# -t 服务器类型 1-中心服 2-玩家管理服 3-数据处理服 4-游戏服 5-连接服 按类型顺序依次启动  -s 该类服务器编号 从0开始
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
