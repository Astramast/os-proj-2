#! /bin/bash

USAGE='Usage : ./monitoring [run|status|sync|shutdown|help]'

function parse_command () {
	for param ; do
		if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
			echo "$USAGE" 1>&2
			exit
		fi
	done
	
	case "$1" in
		run)
			shift
			./run.sh $@
			;;
		status)
			shift
			./status.sh $@
			;;
		sync)
			shift
			./sync.sh $@
			;;
		shutdown)
			shift
			./shutdown.sh $@
			;;
		*)
			echo "$USAGE" 1>&2
			exit $BAD_USAGE
			;;
	esac
}

parse_command "$@"
echo 'Done.'

