#!/bin/bash

# small test; memory usage unimportant.
testlist=(`<somenumbers`)
rightanswers=(`<rightanswers`)
youranswers=(`./primetest <somenumbers`)

failcount=0

# now just compare the differences:
for (( i = 0; i < ${#testlist[@]}; i++ )); do
	[[ -z "${youranswers[$i]}" ]] && failcount=1 && \
		echo "Insufficient output." && exit 1

	if (( ${youranswers[$i]} != ${rightanswers[$i]} )); then
		echo "Your code fails on input ${testlist[$i]}"
		(( failcount++ ))
	fi
done

[[ "$failcount" == 0 ]] && echo "Test passed :D"
