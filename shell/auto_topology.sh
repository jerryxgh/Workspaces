#! /bin/bash
master='192.168.1.144' #主节点IP地址
echo $master > masters
while read ip
do
    echo "In ${ip} ..."
    if [ "$ip" != "$master" ]; then
        echo $ip >> slaves
    fi
done < ./nodeList.txt
