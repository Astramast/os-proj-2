#! /bin/bash

USAGE='Usage : ./monitoring shutdown [<pid>]'

function parse_args () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done
	GIVE_PID=0
	if [ $# -ne 0 ]; then
		if pgrep -P $1 &> /dev/null ;
			then
				GIVE_PID=$1
			else
				echo "PID is not of principal processus." 1>&2
				exit $BAD_USAGE
		fi

	fi
}

parse_args "$@"
if [ $GIVE_PID -ne 0 ]; 
	then
		kill -s SIGINT $GIVE_PID
	else
		for GIVE_PID in $(pgrep tinydb); do
			if pgrep -P $GIVE_PID &> /dev/null ; then
				echo "Kill tinydb process $GIVE_PID ? "
				select yn in "Yes" "No"; do
					case $yn in
						Yes)
							kill -s SIGINT $GIVE_PID
							;;
						No)
							;;
					esac
					break
				done
			fi
		done
fi
