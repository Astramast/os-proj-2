#! /bin/bash

USAGE='./smalldbctl sync'

function verify_args () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done

	if [ $# -ne 0 ]; then
		echo "Sync accept no argument. Usage : $USAGE" 1>&2
		exit $BAD_USAGE
	fi
}

verify_args "$@"
for SDB_PID in $(pgrep smalldb); do
	kill -s SIGUSR1 $SDB_PID
done

