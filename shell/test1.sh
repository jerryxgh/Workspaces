#! /bin/bash
#生成网络拓扑
while read ip
do
    echo "In ${ip} ..."
    #scp .profile xgh@${ip}:/home/xgh/
    ssh -n xgh@${ip} "cat ~/.ssh/id_dsa.pub >> ~/.ssh/authorized_keys"
done < ./nodeList.txt
