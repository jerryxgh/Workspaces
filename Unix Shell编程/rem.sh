#! /bin/bash
# Remove someone from the phonebook

name="$1"

grep "$name" $PHONEBOOK > /tmp/matches$$

if [ ! -s /tmp/matches$$ ]; then
    echo "Can't find $name in the phonebook"   
    exit 1
fi

# Display matching entries one at a time and confirm removal
while read line; do
    display.sh "$line"
    echo -e "Remove this entry (y/n)? \c"
    read answer < /dev/tty

    if [ "$answer" = y ]; then
        break
    fi
done < /tmp/matches$$

rm /tmp/matches$$

if [ "$answer" = y ]; then
    if grep -v "^$line$" $PHONEBOOK > /tmp/phonebook$$; then
        mv /tmp/phonebook$$ $PHONEBOOK
        echo "Selected entry has been removed."
    else
        echo "Selected entry not removed."
    fi
fi
