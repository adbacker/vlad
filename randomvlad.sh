#!/bin/bash

ipaddr=192.168.1.139


while [[ 1==1 ]]
do
sleep 12
runroll=$(( ( RANDOM % 10 )  + 1 ))
echo $runroll
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [[ $runroll == 1 ]]
then
	intensity=$(( ( RANDOM % 255 ) )) 
	echo "intensity: $intensity"
	curl http://$ipaddr/intensity?t=$intensity
	curl http://$ipaddr/speed?t=30
	runchoice=$(( ( RANDOM % 7 )  + 1 ))
	case $runchoice in
		1)
		curl http://$ipaddr/wipe?t=wipe
		;;
		2)
		curl http://$ipaddr/wipe?t=split
		;;
		3)
		curl http://$ipaddr/wipe?t=reversesplit
		;;
		4)
		curl http://$ipaddr/wipe?t=lsd
		;;	
		*)
		;;
	esac
fi
done
