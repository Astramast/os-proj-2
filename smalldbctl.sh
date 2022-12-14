#! /bin/bash

USAGE='Usage : ./smalldbctl [list|sync|stop|help]'

function parse_command () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done

	case "$1" in
		list)
			shift
			./list.sh $@
			;;
		sync)
			shift
			./sync.sh $@
		stop)
			shift
			./stop.sh $@
		*)
			echo "$USAGE" 1&>2
			exit $BAD_USAGE
			;;
	esac
}

parse_command "$@"
echo 'Done.'

