#! /bin/bash
if echo $1 | grep '^[a-zA-Z_][a-zA-Z_0-9]*'
then
    echo yes
else
    echo no
fi
