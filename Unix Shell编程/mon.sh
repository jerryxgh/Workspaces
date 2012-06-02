#! /bin/bash

if [ "$1" = -m ]; then
    mailopt=TRUE
    shift
else
    mailopt=FALSE
fi

if [ "$#" -eq 0 -o "$#" -gt 1 ]; then
    echo "Usage: mon [-m] user"
fi

user="$1"

until who | grep "^$user " > /dev/null; do
    sleep 60
done

echo "$user has logged on"