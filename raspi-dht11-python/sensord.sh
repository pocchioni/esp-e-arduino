#!/bin/sh
#
# init script for sensord
#
# Thanks to http://blog.scphillips.com/posts/2013/07/getting-a-python-script-to-run-in-the-background-as-a-service-on-boot/
# Usage:
# Copy the init script into /etc/init.d using:    sudo cp sensord.sh /etc/init.d/sensord
# Make sure the script is executable (chmod again) and make sure that it has UNIX line-endings (dos2unix)
# Run the command       sudo update-rc.d sensord defaults
# This command adds in symbolic links to the /etc/rc?.d directories so that the init script is run at the default times
# Show the links        ls -l /etc/rc?.d/*sensord
# Now it is possible to start-stop-status the service     sudo service sensord status
# The logs can be viewed with    tail -f /var/log/sensord.log



### BEGIN INIT INFO
# Provides:          sensor-daemon
# Required-Start:    $remote_fs $syslog $network
# Required-Stop:     $remote_fs $syslog $network
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: init script for sensors
# Description:       Avvia uno script python che legge dht11 e manda in mqtt
### END INIT INFO

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
NAME=sensord
DAEMON=/home/pi/dht11/dht11-mqtt.py
DAEMONARGS=""
PIDFILE=/var/run/$NAME.pid
LOGFILE=/var/log/$NAME.log

. /lib/lsb/init-functions

test -f $DAEMON || exit 0

case "$1" in
    start)
        start-stop-daemon --start --background \
            --pidfile $PIDFILE --make-pidfile --startas /bin/bash \
            -- -c "exec stdbuf -oL -eL $DAEMON $DAEMONARGS > $LOGFILE 2>&1"
        log_end_msg $?
        ;;
    stop)
        start-stop-daemon --stop --pidfile $PIDFILE
        log_end_msg $?
        rm -f $PIDFILE
        ;;
    restart)
        $0 stop
        $0 start
        ;;
    status)
        start-stop-daemon --status --pidfile $PIDFILE
        log_end_msg $?
        ;;
    *)
        echo "Usage: $0 {start|stop|restart|status}"
        exit 2
        ;;
esac

exit 0
