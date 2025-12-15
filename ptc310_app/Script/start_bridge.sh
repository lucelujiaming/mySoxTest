#!/bin/sh
# ps | grep start_ethernet
# kill -9 126
# kill -9 127

if [ $# -ge 1 ] ; then
    echo "Adding br0 ......"
    ip addr del 192.168.168.129/24 dev eth0
    ip addr del 192.168.168.130/24 dev eth1

    # eth0      Link encap:Ethernet  HWaddr 76:D7:12:71:00:CE
    # eth1      Link encap:Ethernet  HWaddr 76:D6:12:71:00:CE

    echo "----------------brctl addbr br0----------------"
    brctl addbr br0
    brctl addif br0 eth0
    brctl addif br0 eth1
    brctl stp br0 off
    echo "----------------brctl addbr br0 over----------------"

    ifconfig br0 192.168.10.$1 netmask 255.255.255.0 up
    route add default gw 192.168.10.1

    ip route add 192.168.10.0 via 192.168.10.$1
    ip link set br0 up
    # sleep 5
    # /root/traceroute_daemon.sh &
    # /usr/sbin/arping -I br0 -s 192.168.10.$1 192.168.10.180 &
    /root/arping_daemon.sh $1 180 &

else
    echo "Delete br0......"
    ifconfig br0 down
    brctl delif br0 eth0
    brctl delif br0 eth1
    brctl delbr br0
    killall rstpd
    ifconfig eth0 192.168.168.129
    ifconfig eth1 192.168.168.130
fi
