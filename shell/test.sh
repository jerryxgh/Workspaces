#! /bin/bash
#生成网络拓扑
master='192.168.1.144' #主节点IP地址
node_list_file='slaves' #存放集群节点IP地址的文件
echo $master > masters
while read ip
do
    echo "In ${ip} ..."
    if [ "$ip" != "$master" ]; then
        echo $ip >> slaves
    fi
done < ./nodeList.txt
sort -u ${node_list_file} > /tmp/${node_list_file}$$
cat /tmp/${node_list_file}$$ > ${node_list_file}
rm /tmp/${node_list_file}$$

while read ip
do
    echo "In ${ip} ..."

    #使用FIFO
    ssh -n xgh@${ip} "rm ~/local/hadoop-1.0.3/conf"
    ssh -n xgh@${ip} "ln -s ~/local/hadoop-1.0.3/conf.fifo ~/local/hadoop-1.0.3/conf"
    #使用Fair
    #ssh -n xgh@${ip} "ln -s ~/local/hadoop-1.0.3/conf.fair ~/local/hadoop-1.0.3/conf"
    #使用改进算法
    #ssh -n xgh@${ip} "ln -s ~/local/hadoop-1.0.3/conf.mine ~/local/hadoop-1.0.3/conf"

    scp ./masters xgh@${ip}:/home/xgh/local/hadoop-1.0.3/conf
    scp ./slaves xgh@${ip}:/home/xgh/local/hadoop-1.0.3/conf
done < ./nodeList.txt
