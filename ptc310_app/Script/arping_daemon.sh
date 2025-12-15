#!/bin/sh

# cat sys_config.ini | grep "Service" | sed 's/ //g' | sed "s/.*=//"
# To accelerate the startup process
ETH_INIT_COUNT=10
# We send $ETH_RECOVER_COUNT when the cable reconnect
ETH_RECOVER_COUNT=10
# $ARPING_SPAN is 10-6s, so 300000 is 0.3s
# It is not acurate for the shell environment.
ARPING_SPAN=300000
# It used with $ARPING_SPAN, 
# so heartbeat is larger than $HEARTBEAT_TIMES * $ARPING_SPAN = 10 * 0.3s = 3s
# Because the mstp witch time is 4 seconds. It can be larger . Such as 20.
HEARTBEAT_TIMES=10

eth_status=0
heartbeat_counter=0
last_ptc_timeout=0
if [ $# -ge 2 ] ; then
    eth_status=$ETH_INIT_COUNT
    while [ $eth_status -gt 0 ]
    do
        # traceroute -i br0 192.168.10.12 > /dev/null
        /usr/sbin/arping -f -b -I br0 -s 192.168.10.$1 192.168.10.$2
        echo "-------------------- Init eth_status=$eth_status --------------------"
        if [ $? -eq 0 ]; then
            eth_status=$((eth_status - 1))
        fi
        # sleep 2
        usleep $ARPING_SPAN
    done
    # Starting cycle to check the eth0 and eth1
    eth_status=0
    while true; do
        eth0result=`mstpctl showtreeport br0 eth0 0 | grep "role" | awk -F" " '{print $2}'`
        eth1result=`mstpctl showtreeport br0 eth1 0 | grep "role" | awk -F" " '{print $2}'`
        if [ "$eth0result" == "Disabled" ]; then
            # traceroute -i br0 192.168.10.12 > /dev/null
            eth_status=$ETH_RECOVER_COUNT
            /usr/sbin/arping -f -b -I br0 -s 192.168.10.$1 192.168.10.$2
            echo "-------------------- eth0Disabled eth_status=$eth_status --------------------"
            # sleep 2
            usleep $ARPING_SPAN
        elif [ "$eth1result" == "Disabled" ]; then
            # traceroute -i br0 192.168.10.12 > /dev/null
            eth_status=$ETH_RECOVER_COUNT
            /usr/sbin/arping -f -b -I br0 -s 192.168.10.$1 192.168.10.$2
            echo "-------------------- eth1Disabled eth_status=$eth_status --------------------"
            # sleep 2
            usleep $ARPING_SPAN
        else
            # eth0&eth1 Restore
            if [ $eth_status -gt 0 ]; then
                # traceroute -i br0 192.168.10.12 > /dev/null
                /usr/sbin/arping -f -b -I br0 -s 192.168.10.$1 192.168.10.$2
                echo "-------------------- br0Restore eth_status=$eth_status --------------------"
                if [ $? -eq 0 ]; then
                    eth_status=$((eth_status - 1))
                fi
            # Heart beat
            else
                ptcTimeout=`cat /root/app/sys_config.ini | grep "ModbusNetworkTimeout" | sed 's/ //g' | sed "s/.*=//"`
                if [ "$ptcTimeout" == "1" ]; then
                    if [ $last_ptc_timeout -eq 0 ]; then
                        /usr/sbin/arping -f -b -I br0 -s 192.168.10.$1 192.168.10.$2
                        echo "-------------------- HeartBeat startup arping return=$? --------------------"
                        last_ptc_timeout=1
                    fi
                    if [ $heartbeat_counter -gt $HEARTBEAT_TIMES ]; then
                        /usr/sbin/arping -f -b -I br0 -s 192.168.10.$1 192.168.10.$2
                        echo "-------------------- HeartBeat arping return=$? --------------------"
                        heartbeat_counter=0
                    fi
                    heartbeat_counter=$((heartbeat_counter + 1))
                else
                    if [ $last_ptc_timeout -eq 1 ]; then
                        last_ptc_timeout=0
                    fi
                fi
            fi
            # sleep 2
            usleep $ARPING_SPAN
        fi
        # arp -d 192.168.10.$2
    done
else
    echo "We need localIP and remoteIP......"
fi 

