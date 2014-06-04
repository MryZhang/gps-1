#!/bin/bash
#echo "starting rpn check for /home/gps/dump.raw"
#echo -e '\x01 cucu' > asd #root/logs/tOut
for i in {1..32}
do
   echo "Testing RPN $i..."
   ./SoftOSGPS_single_channel -m 4 -f ../dump-ros.raw -0 $i -s 30 >rpn$i
done
#echo $((`cat rpn* | wc -c`/3))
#echo $((`cat rpn* | wc -c`/3)) | python -c 'import sys; line = sys.stdin.readline(); print "\x00%s%s" % (line,"asd") ,' > asd #/root/logs/tOut
./discover.o 666>log 1>gps-t5.sh
chmod +x gps-t5.sh
