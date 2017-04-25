#!/bin/bash
ipaddr=192.168.1.139
sleepDelay=5
echo $ipaddr

curl http://$ipaddr/speed?t=30
curl http://$ipaddr/intensity?t=40

curl http://$ipaddr/wipe?t=wipe
sleep $sleepDelay
echo **********************

curl http://$ipaddr/wipe?t=split
sleep $sleepDelay
echo **********************

curl http://$ipaddr/wipe?t=reversesplit
sleep $sleepDelay
echo *********************
curl http://$ipaddr/wipe?t=lsd
sleep $sleepDelay

curl http://$ipaddr/wipe?t=wipe
curl http://$ipaddr/intensity?t=255
sleep $sleepDelay
curl http://$ipaddr/intensity?t=40
curl http://$ipaddr/speed?t=10
sleep $sleepDelay
curl http://$ipaddr/vladoff
