#! /bin/bash

USAGE='./monitoring status'

function verify_args () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done
	
	if [ $# -ne 0 ]; then
		echo "Status accept no argument. Usage : $USAGE" 1>&2
		exit $BAD_USAGE
	fi
}

verify_args "$@"
echo "Found $(ps -e | grep tinydb -c) processes called tinydb."
echo "$(ps -e | grep tinydb)"
