#! /bin/bash
# Program to add someone to the phonebook file

echo "Type in your new entry"
echo "When you are done, type just a single enter on the line"

first=
entry=

while [ true ]; do
    echo -e ">> \c"
    read line

    if [ -n "$line" ]; then
        if [ -z "$first" ]; then
            first=$line;
            entry="$line"
        else
            entry="$entry^$line"
        fi
    else
        break
    fi
done

echo "$entry" >> $PHONEBOOK
sort -o $PHONEBOOK $PHONEBOOK
echo
echo "$first has been added to the phone book"
