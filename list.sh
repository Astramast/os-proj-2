#! /bin/bash

USAGE='./smalldbctl list'

function verify_args () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done

	if [ $# -ne 0 ]; then
		echo "List accept no argument. Usage : $USAGE" 1>&2
		exit $BAD_USAGE
	fi
}

verify_args "$@"
for IP in $(ss --no-header -Ontp4 'sport = :28772' | awk '{print $5}' | cut -d: -f1 -s); do
	echo $IP
done

