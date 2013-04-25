#! /bin/bash

# recursively searching current directory for "台州" in JSP files and replace it with "昆山"
for i in `find . -depth -name "*.jsp" -exec grep -Hni "台州" {} \; | awk -F: '{print $1}'`; do
    if [ -f "$i" ]; then
        sed -i -e 's/台州/昆山/g' "$i"
    fi
done;