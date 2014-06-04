# Launch script in background
#./gps-setup-n-dump.o -c -buff_size 256 -delay 10000 | dd of=/dev/null 2>&1 &
#./gps-setup-n-dump.o -c -buff_size 256 -delay 10000 | dd of=/dev/null 2>&1 | grep MB &
#gps-setup-n-dump --config_spi 0|1 --verbose 0|1 --buffer_size <size>


./gps-setup-n-dump -l 2 -f 4 -d | dd of=/dev/null 2>&1 | grep MB | cut -d" " -f8 &
	sleep 2
	PID=`ps | grep 'gps' | cut -d" " -f1`
	PID2=`ps | grep ' dd' | cut -d" " -f1`

	kill $PID






