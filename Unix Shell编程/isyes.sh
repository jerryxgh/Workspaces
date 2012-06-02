#! /bin/bash
case "$1" in
    [yY] | [yY]es | YES)
        exit 0;;
    *)
        exit 1;;
esac
