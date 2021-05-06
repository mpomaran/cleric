#!/bin/bash
LOG_FILE=/home/pi/cleric-storage/test.log
SERVER=`find . -name "cleric-server"`
if [ -x $SERVER == false ]; then
    echo "Cannot find executable, build first"
    exit 1
fi

PROPS=`find -name "cleric.properties"`
 if [ -x $PROPS == false ]; then
     echo "Cannot find properties"
     exit 1
 fi

touch $LOG_FILE
MaxFileSize=2048
sudo $SERVER --root_path=~/cleric/www --storage-strategy=hdd --storage-root=/home/pi/cleric-storage --prop-file=${PROPS} -v | tee ${LOG_FILE} &
while true
do
    #Get size in bytes**
    file_size=`du -b ${LOG_FILE} | tr -s '\t' ' ' | cut -d' ' -f1`
    if [ $file_size -gt $MaxFileSize  ];then
        timestamp=`date +%s`
        rm $LOG_FILE.1
        mv $LOG_FILE $LOG_FILE.1
        touch $LOG_FILE
    fi
done
