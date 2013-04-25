#! /bin/bash
# Look someone up in the phonebook

name=$1
grep "$name" $PHONEBOOK > /tmp/matches$$

if [ ! -s /tmp/matches$$ ]; then
    echo "I can't find $name in the phonebook"
else
    # Display each of the matching entries
    while read line; do
        display.sh "$line"
    done < /tmp/matches$$
fi

rm /tmp/matches$$