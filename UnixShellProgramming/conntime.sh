#! /bin/bash
logDay=$(who | grep $1 | sed 's/  */ /g' | cut -d' ' -f3 | cut -d'-' -f3)
logTime=$(who | grep $1 | sed 's/  */ /g' | cut -d' ' -f4)
logHour=$(echo $logTime | cut -d: -f1)
logMinute=$(echo $logTime | cut -d: -f2)
nowDay=$(date | cut -d' ' -f3 | sed 's/日//g')
nowTime=$(date | cut -d' ' -f5)
nowHour=$(echo $nowTime | cut -d: -f1)
nowMinute=$(echo $nowTime | cut -d: -f2)
allMinutes=$(( ((nowDay - logDay) * 24 + nowHour - logHour) * 60 + nowMinute - logMinute  ))
onHour=$(( allMinutes / 60 ))
onMinute=$(( allMinutes % 60 ))

echo "$1 已经登录了 $onHour 小时， $onMinute 分钟"
