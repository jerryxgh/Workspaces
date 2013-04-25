#! /bin/bash
# 获取当前机器的IP地址
while read ip
do
    echo "In ${ip} ..."
    # 集群拷贝 .profile，设定Java环境变量、Hadoop环境变量以及自启动脚本等
    ssh -n xgh@${ip} "mkdir -p ~/local/bin"
    scp ./report.sh xgh@${ip}:~/local/bin
    ssh -n xgh@${ip} "chmod +x ~/local/bin/report.sh"
    scp ./.profile xgh@${ip}:~/
    scp -r ~/local/hadoop-1.0.3/conf.fair/ xgh@${ip}:/home/xgh/local/hadoop-1.0.3/
    ssh -n xgh@${ip} "ln -s ~/local/hadoop-1.0.3/conf.fifo local/hadoop-1.0.3/conf"

done < ./nodeList.txt
