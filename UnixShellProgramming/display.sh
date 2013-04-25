#! /bin/bash
# Display entry from the phonebook

echo
echo "----------------------------------"

entry="$1"
IFS='^'
set $entry

for line in "$1" "$2" "$3" "$4" "$5" "$6"; do
    printf "| %-30.30s |\n" $line
done

echo "|        o               o       |"
echo "----------------------------------"
echo
