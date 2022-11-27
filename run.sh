#! /bin/bash

USAGE='./monitoring run [<path_to_database>] [-f|--file <queries_file>]'

function parse_args () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done
	
	DIR='data/students.bin'

	while [ $# -gt 0 ]; do
		case "$1" in
			-f|--file)
				if [ $# -lt 2 ] ; then
					echo '-f|--file requires an argument' 1>&2
					exit $BAD_USAGE
				fi
				FILE_NAME="$2"
				shift;shift
				;;
			-*)
				echo "Unknown option : $1"
				exit $BAD_USAGE
				;;
			*)
				DIR="$1"
				shift
				;;
		esac
	done
}

parse_args "$@"

if [ ! -f $DIR ]; then
	touch $DIR
	echo "$DIR not found, created empty one." 1>&2
fi

if [ -z $FILE_NAME ];
	then
		./tinydb $DIR & echo "Principal process PID : $!"
	else
		if [ -f $FILE_NAME ];
			then
				./tinydb $DIR < $FILE_NAME & echo "Principal process PID : $!"
			else
				echo "$FILE_NAME does not exist." 1>&2
				exit $BAD_USAGE
		fi
fi

