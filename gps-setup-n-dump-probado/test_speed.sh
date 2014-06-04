(./gps-setup-n-dump -l 2 -f 4 -d | dd of=/dev/null) & true; sleep 3 && kill %2
