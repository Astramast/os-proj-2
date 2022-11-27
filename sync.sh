#! /bin/bash

USAGE='./monitoring sync'

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
for TDB_PID in $(pgrep tinydb); do
	if pgrep -P $TDB_PID &> /dev/null; then
		echo "Syncing $TDB_PID"
		kill -s SIGUSR1 $TDB_PID
	fi
done

