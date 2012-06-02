#! /bin/bash
i=0
for var in "$@"
do
   echo "$((++i)): $var"
done
