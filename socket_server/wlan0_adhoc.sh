ifconfig wlan0 down
iwconfig wlan0 mode ad-hoc
iwconfig wlan0 essid "adhoc_B"
ifconfig wlan0 up
ifconfig wlan0 192.168.2.2