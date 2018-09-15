#!/bin/bash

# TODO: make sure you edit postdata to reflect your information.
. ./postdata
if (( $# != 1 )) ; then
	echo "Usage: $0 <filename>"
	exit 1
fi

rcpt=$(basename $1)-rcpt.html

curl -k --form "file=@$1" --form lname=$lastname --form sid=$sidlast4 \
	"https://www-cs.ccny.cuny.edu/~wes/CSC103/upload_file.php" > $rcpt

if (( $? != 0 )) ; then
	echo -e "\033[31mError:\033[0m upload failed.  Check network?"
	exit 1
fi
if grep -qiE '(fail|warn)' $rcpt ; then
	echo -e "\033[1;31mWarning:\033[0m Receipt looks wrong.  Double check postdata?"
	exit 2
fi
echo -e "\033[32mSuccess (probably).\033[0m  See receipt in $rcpt"
