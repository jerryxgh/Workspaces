#! /bin/bash
user="$1"
if who | grep "^$user " > /dev/null
then
    echo "User $1 is logged on."
fi
