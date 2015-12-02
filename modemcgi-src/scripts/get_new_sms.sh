#!/bin/sh
row=`cat /var/log/modem | sed /^$/d | awk '/AT\+CMGR/{ print NR }' | awk 'END { print }'`
next_row=`expr $row + 1`
result=`cat /var/log/modem | sed /^$/d | sed -n "$next_row"p | awk '{ print $1 }'`
#echo $result
if [ "$result" = "+CMGR:" ]
then
	cmd=`cat /var/log/modem | sed /^$/d | sed -n "$next_row"p'`
	echo $cmd
elif [ "$result" = "+CME" ]
then
	cmd=`cat /var/log/modem | sed /^$/d | sed -n "$next_row"p'`
	echo $cmd
else 
	echo "URC MESSAGE"
fi
