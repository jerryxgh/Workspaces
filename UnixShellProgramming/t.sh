#! /bin/bash
hour=$(date | cut -d' ' -f5 | cut -d: -f1)
minite=$(date | cut -d' ' -f5 | cut -d: -f2)

case "$hour" in
    1[2-9] | 2[0-3])
        echo "$(($hour-12)):$minite pm";;
    *)
        echo "$hour:$minite am";;
esac
