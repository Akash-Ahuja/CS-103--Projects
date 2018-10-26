#!/bin/bash

# Run though supported options, check output of system's version
# against the local one.

tdir=$(mktemp -d /tmp/p3test-XXXXXX)
[[ -n $binary ]] && \
	head -c 1024 /dev/urandom > "$tdir/generic" || \
	head -c 8192 /dev/urandom | tr -c -d '[:alnum:]\n\t' > "$tdir/generic"

# list of options --> subsets of options:
pow() {
	(( $# == 0 )) && printf "\n" || (shift; pow $@) | \
		while read l ; do
			printf "%s\n" "$1 $l"
			printf "%s\n" "$l"
		done
}

declare -A flags
flags=([wc]="-c -l -w -L" [sort]="-r -u -f" [uniq]="-c -d -u" \
	[shuf]="-e -i -n" [tr]="-c -d")

pretr() {
	local tfname="$tdir/generic"
	allflags=( \
		"-c -d '{}'" \
		"-d '{}'" \
		"'{}' '[]'" \
		"-c '{}' '[]'" \
		"' \t' '\n'" \
		)
	unset cmdmine
	unset cmdsys
	for (( i = 0; i < ${#allflags[@]}; i++ )); do
		cmdmine[$i]="./tr ${allflags[$i]} < $tfname"
		cmdsys[$i]="tr ${allflags[$i]} < $tfname"
	done
}

presort() {
	local tfname="$tdir/generic"
	mapfile -t allflags < <(pow ${flags[sort]})
	unset cmdmine
	unset cmdsys
	for (( i = 0; i < ${#allflags[@]}; i++ )); do
		cmdmine[$i]="./sort ${allflags[$i]} < $tfname"
		cmdsys[$i]="sort ${allflags[$i]} < $tfname"
	done
}

preshuf() {
	# XXX this isn't a very good test; it just makes sure that you
	# didn't lose anything in the output.
	local tfname="$tdir/generic"
	# mapfile -t allflags < <(pow ${flags[shuf]})
	allflags=( \
		"-e 'hello' 'there' 'lolol' ':D' | sort" \
		"-e 'o_o' 'O_O' 'x_x' '@_@' | sort" \
		"-i0-50 | sort" \
		"-i1-100 | sort" \
		"< $tfname | sort" \
		"-n5 < $tfname | wc -l" \
		"-n25 < $tfname | wc -l" \
		)
	unset cmdmine
	unset cmdsys
	for (( i = 0; i < ${#allflags[@]}; i++ )); do
		cmdmine[$i]="./shuf ${allflags[$i]}"
		cmdsys[$i]="shuf ${allflags[$i]}"
	done
}

preuniq() {
	local tfname="$tdir/t-uniq"
	head -c 8192 /dev/urandom | tr '\xf5' '\n' | \
		tr -c -d 'abAB\n' | sort > $tfname
	mapfile -t allflags < <(pow ${flags[uniq]})
	unset cmdmine
	unset cmdsys
	for (( i = 0; i < ${#allflags[@]}; i++ )); do
		cmdmine[$i]="./uniq ${allflags[$i]} < $tfname"
		cmdsys[$i]="uniq ${allflags[$i]} < $tfname"
	done
}

# XXX this doesn't check the extra option for unique words.
prewc() {
	# get a more interesting file for wc (consecutive spaces...)
	local tfname="$tdir/t-wc"
	head -c 2048 /dev/urandom | \
		tr '\0-\377' '\40-\176\40-\176\n\n\n\t\t\t\t\t ' > "$tfname"
	mapfile -t allflags < <(pow ${flags[wc]})
	unset cmdmine
	unset cmdsys
	local awkwc="awk 'BEGIN{OFS = \".\"} {print \$1, \$2, \$3, \$4}'"
	for (( i = 0; i < ${#allflags[@]}; i++ )); do
		cmdmine[$i]="./wc ${allflags[$i]} < $tfname | $awkwc"
		cmdsys[$i]="wc ${allflags[$i]} < $tfname | $awkwc"
	done
}

# programs to test should be given in $@
failcount=0
for p in "$@" ; do
	# [[ $(type -t "pre${p}") == "function" ]] ; then
	if [[ ${flags["$p"]+asdf} ]] ; then
		echo "$p : testing flags ${flags[$p]} ..."
		pre${p}
		for (( i = 0; i < ${#allflags[@]}; i++ )); do
			mapfile -t out < <(sha1sum <(eval ${cmdmine[$i]}) <(eval ${cmdsys[$i]}))
			if [[ "${out[0]:0:40}" != "${out[1]:0:40}" ]] ; then
				(( failcount++ ))
				echo -e "\033[1;31mXXX:\033[0m ${cmdmine[$i]}"
			fi
		done
	else
		echo "$p : unrecognized command"
	fi
done
if (( $failcount > 0 )) ; then
	echo "$failcount tests failed.  Input file(s) saved in ${tdir}/."
else
	echo "Test passed :D"
	[[ -z $save ]] && rm -r "$tdir"
fi
