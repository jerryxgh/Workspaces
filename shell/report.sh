#! /bin/bash

# 定义变量
master='192.168.1.144' #主节点IP地址
node_list_file='node_list.txt' #存放集群节点IP地址的文件

# 获取当前机器的IP地址
ip=$(ifconfig -a | grep inet | grep -v 127.0.0.1 | grep -v inet6 | awk -F '[ :]+' '{print $4}')

# 向主节点上报
ssh xgh@$master "echo $ip >> ~/${node_list_file}; sort -u ~/${node_list_file} > /tmp/${node_list_file}$$; cat /tmp/${node_list_file}$$ > ~/${node_list_file}; rm /tmp/${node_list_file}$$"
