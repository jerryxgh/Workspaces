#! /bin/bash

: ${PHONEBOOK:=$HOME/phonebook}
PATH=.:"$PATH"
export PHONEBOOK PATH

if [ ! -e "$PHONEBOOK" ]; then
    echo "$PHONEBOOK does not exist!"
    echo -e "Should I create it for you (y/n)? \c"
    read answer

    if [ "$answer" != y ]; then
        exit 1
    fi

    > "$PHONEBOOK" || exit 1 # exit if the creation fails
fi

if [ "$#" -ne 0 ]; then
    lu "$@"
    exit
fi

# Set trap on interrupt (C-c)
trap "continue" 2

# Loop until user select exit

while [ true ]; do
    # Display menu
    echo -e '
Would you like to:

1. Look someone up
2. Add someone to the phone book
3. Remove someone from the phone book
4. Change an entry in the phone book
5. List all names and numbers in the phone book
6. Exit the program

Please select one of the above (1-6): \c'
    # Read and process selection
    read choice
    echo
    case "$choice" in
        1)
            echo -e "Enter name to look up: \c"
            read name
            
            if [ -z "$name" ]; then
                echo "Lookup ignored"
            else
                lu.sh "$name"
            fi
            ;;
        2)
            add.sh
            ;;
        3)
            echo -e "Enter name to remove: \c"
            read name
            if [ -z "$name" ]; then
                echo "Removal ignored"
            else
                rem.sh "$name"
            fi
            ;;
        4)
            echo -e "Enter name to change: \c"
            read name
            if [ -z "$name" ]; then
                echo "Change ignored"
            else
                change.sh "$name"
            fi
            
            ;;
        5)
            listall.sh
            ;;
        6)
            exit 0  
            ;;
        *)
            echo "Bad choice\a"
            ;;
    esac
done
