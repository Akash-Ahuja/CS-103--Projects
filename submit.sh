#!/bin/bash

# TODO: make sure you edit postdata to reflect your information.
. ./postdata
if (( $# != 1 )) ; then
	echo "Usage: $0 <filename>"
	exit 1
fi

curl -k --form "file=@$1" --form lname=$lastname --form sid=$sidlast4 \
	"https://www-cs.ccny.cuny.edu/~wes/CSC103/upload_file.php" \
	> $(basename $1)-rcpt.html
