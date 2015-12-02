#!/bin/sh
raw=`cat /var/log/modem | sed /^$/d | awk '/AT\#RFSTS/{ print NR }' | awk 'END { print }'`
next_raw=`expr $raw + 1`
result=`cat /var/log/modem | sed /^$/d | sed -n "$next_raw"p | awk '{ print $1 }'`
split_result=${result:0:7}
#echo $result
if [ "$split_result" = "#RFSTS:" ]
then
	cmd=`cat /var/log/modem | sed /^$/d | sed -n "$next_raw"p'`
	echo $cmd
elif [ "$result" = "+CME" ]
then
	cmd=`cat /var/log/modem | sed /^$/d | sed -n "$next_raw"p'`
	echo $cmd
else 
	echo "URC MESSAGE"
fi
