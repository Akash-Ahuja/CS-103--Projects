#!/bin/bash

# clear old output file:
echo -n "" > output

./towers -n 4 -s 1 -e 3 | awk \
		'BEGIN{OFS = "\t"} {print $1, $2}' >> output
echo "@@" >> output
./towers -n 3 -s 1 -e 2 | awk \
		'BEGIN{OFS = "\t"} {print $1, $2}' >> output
echo "@@" >> output
./towers -n 3 -s 3 -e 1 | awk \
		'BEGIN{OFS = "\t"} {print $1, $2}' >> output
echo "@@" >> output
./towers -n 5 -s 1 -e 2 | awk \
		'BEGIN{OFS = "\t"} {print $1, $2}' >> output
echo "@@" >> output
./towers -n 6 -s 1 -e 3 | awk \
		'BEGIN{OFS = "\t"} {print $1, $2}' >> output
echo -n "@@@@@" >> output
