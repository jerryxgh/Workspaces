#! /bin/bash
# Change entry in the phonebook

name="$1"

# Get match entries and save in temp file

grep "$1" $PHONEBOOK > /tmp/matches$$

if [ ! -s /tmp/matches$$ ]; then
    echo "Con't find $name in the phonebook"
    exit 1
fi

# Display matching entris one at a time and confirm Change

while read line; do
    display.sh "$line"
    echo -e "Change this entry (y/n)? \c"
    read answer < /dev/tty
    if [ "$answer" = y ]; then
        break
    fi
done < /tmp/matches$$

rm /tmp/matches$$

if [ "$answer" != y ]; then
    exit
fi

# Start up editor on the selected entry

echo -e "$line\c" | tr '^' '\012' > /tmp/vim$$
echo "Enter changes with ${EDITOR:=/usr/bin/vim}"
trap " " 2
${EDITOR} /tmp/vim$$

# Remove old entry and insert new one
grep -v "^$line$" $PHONEBOOK > /tmp/phonebook$$
{ tr '\012' '^' < /tmp/vim$$; echo; } >> /tmp/phonebook$$

sort /tmp/phonebook$$ -o $PHONEBOOK
rm /tmp/vim$$ /tmp/phonebook$$