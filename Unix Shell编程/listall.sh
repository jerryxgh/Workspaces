#! /bin/bash
# List all of the entries in the phonebook

echo "------------------------------------------------"
IFS='^'

while read line; do
    set $line
    eval printf "\"%-36.36s %s\\n\"" "\"$1\"" "\"\${$#}\""
done < $PHONEBOOK
echo "------------------------------------------------"
