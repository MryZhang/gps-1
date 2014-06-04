# Launch script in background
#./gps-setup-n-dump.o -c -buff_size 256 -delay 10000 | dd of=/dev/null 2>&1 &
#./gps-setup-n-dump.o -c -buff_size 256 -delay 10000 | dd of=/dev/null 2>&1 | grep MB &
#gps-setup-n-dump --config_spi 0|1 --verbose 0|1 --buffer_size <size>


for busi in `seq 0 5 50`;do

	./gps-setup-n-dump.o --config_spi 0 --verbose 0 --buffer_size $busi | dd of=/dev/null 2>&1 | grep MB | cut -d" " -f8 &
	echo CHUNK =  $busi
	sleep 0.2
	PID=`ps -A | grep 'gps' | cut -d" " -f1`
	PID2=`ps -A | grep ' dd' | cut -d" " -f1`

	for i in `seq 1 5`;do
		sleep 0.2
		kill -USR1 $PID2
	done
	sleep 0.1
	kill $PID

done





