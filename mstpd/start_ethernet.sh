#!/bin/sh
# echo "rm /root/wifi-sta-dhcp.sh------------------------"
# if [ -e "/root/wifi-sta-dhcp.sh" ]; then
#     rm /root/wifi-sta-dhcp.sh
# fi
# if [ -e "/usr/sbin/wpa_supplicant" ]; then
#     mv /usr/sbin/wpa_supplicant /usr/sbin/rm_wpa_supplicant
# fi
# if [ -e "/usr/sbin/wpa_cli" ]; then
#     mv /usr/sbin/wpa_cli  /usr/sbin/rm_wpa_cli
# fi
# if [ -e "/sbin/udhcpc" ]; then
#     mv /sbin/udhcpc /sbin/rm_udhcpc
#     reboot
# fi

echo "lujiaming -----------------------------------"
chmod 711 /var/empty
if [ ! -e "/dev/mmcblk0" ]; then
    /root/app/v3s_sdc_det_operator
    sleep 5
fi
# Enable framebuffer play 
ln -sf /dev/fb8  /dev/fb0
# # Enable two ethernet 
# echo "Enable two ethernet -----------------------------------"
# echo "Enable two ethernet by usb-set-hostmode.sh -----------------------------------"
# /root/usb-set-hostmode.sh 
# sleep 1
# echo "Enable two ethernet by usb-set-devicemode.sh -----------------------------------"
# /root/usb-set-devicemode.sh
# sleep 1
# echo "Enable two ethernet by usb-set-hostmode.sh -----------------------------------"
# /root/usb-set-hostmode.sh 
# echo "Enable two ethernet over -----------------------------------"
ifconfig -a
echo "lujiaming -----------------------------------"
sleep 1
if [ ! -e "/root/app/current_first_ip" ]; then
    ifconfig eth0 192.168.168.129
else
    FIRSTIPADDR=`cat /root/app/current_first_ip`
    if [ $? -eq 0 ]; then
        echo "lujiaming --------------FIRSTIPADDR=$FIRSTIPADDR---------------------"
        ifconfig eth0 $FIRSTIPADDR
    else
        ifconfig eth0 192.168.168.129
    fi
fi

if [ ! -e "/root/app/current_second_ip" ]; then
    ifconfig eth1 192.168.168.130
else
    SECONDIPADDR=`cat /root/app/current_second_ip`
    if [ $? -eq 0 ]; then
        # echo "lujiaming --------------SECONDIPADDR=$SECONDIPADDR---------------------"
        ifconfig eth1 $SECONDIPADDR
    else
        ifconfig eth1 192.168.168.130
    fi
fi

if [ -e "/root/app/current_first_mac" ]; then
    FIRSTMACADDR=`cat /root/app/current_first_mac`
    if [ $? -eq 0 ]; then
        echo "lujiaming --------------FIRSTMACADDR=$FIRSTMACADDR---------------------"
        /sbin/ifconfig eth0 down
        /sbin/ifconfig eth0 hw ether $FIRSTMACADDR
        /sbin/ifconfig eth0 up
    fi
fi

if [ -e "/root/app/current_second_mac" ]; then
    SECONDMACADDR=`cat /root/app/current_second_mac`
    if [ $? -eq 0 ]; then
        echo "lujiaming --------------SECONDMACADDR=$SECONDMACADDR---------------------"
        /sbin/ifconfig eth1 down
        /sbin/ifconfig eth1 hw ether $SECONDMACADDR
        /sbin/ifconfig eth1 up
    fi
fi
# 打开本地网络回环
ifconfig lo up

mkdir -p /root/sdcard/app
/root/app/goahead --verbose --home /root/app/www > /root/sdcard/app/goahead_output.log &
# Run sedona app.
if [ ! -e "/root/app/current_first_ip" ]; then
    echo "lujiaming --------------We need /root/app/current_first_ip ---------------------"
else
    cd /root/app/
    # ./svm app_134.scode app.sab >> svm_output.log &
    cd - 1>/dev/null 2>&1
fi
echo "lujiaming -----------------------------------"

    echo "lujiaming --------------We need /root/app/current_first_ip ---------------------"   
    ifconfig eth0
    echo "lujiaming --------------We need /root/app/current_first_ip ---------------------"        
    # ifconfig -a
    echo "lujiaming --------------We need /root/app/current_second_ip ---------------------"       
    # ifconfig eth1
    echo "lujiaming --------------We need /root/app/current_second_ip ---------------------" 

# # ebtables-legacy -A INPUT -d 01:80:c2:00:00:00 -j DROP
# ebtables-legacy -A FORWARD -d 01:80:c2:00:00:00 -j DROP
# ebtables-legacy -A OUTPUT -d 01:80:c2:00:00:00 -j DROP

# sleep 1
/root/start_bridge.sh  31

# tcpdump -i br0 stp -w stp_packets_31_$(date +"%Y%m%d_%H%M%S").pcap &
while true; do
    eth0result=`mstpctl showtreeport br0 eth0 0 | grep "role" | awk -F" " '{print $2}'`
    eth1result=`mstpctl showtreeport br0 eth1 0 | grep "role" | awk -F" " '{print $2}'`
    if [ "$eth0result" == "Disabled" ]; then
        echo "mstpctl showtreeport br0 eth0 is Disabled"
        usleep 100000
    elif [ "$eth1result" == "Disabled" ]; then
        echo "mstpctl showtreeport br0 eth1 is Disabled"
        usleep 100000
    else
        break
    fi
done

/usr/sbin/arping -c 3  -b -I br0 -s 192.168.10.31 192.168.10.180
ping -c 3 192.168.10.180 
usleep 500000

echo "lujiaming start ptc310_app -----------------------------------"
# /root/app/ptc310_app ttyS1 ttyS2 > /root/sdcard/app/ptc310_app_log.log &
# /root/app/ptc310_app tcp 1234 ttyS1 > /root/sdcard/app/ptc310_app_log.log &
/root/app/ptc310_app udp 1234 ttyS1 > /root/sdcard/app/ptc310_app_log.log &
echo "lujiaming ptc310_app -----------------------------------"

# /usr/sbin/arping -q -b -I br0 -s 192.168.10.31 192.168.10.180 &
# ping -q 192.168.10.180 &

return


top_count=0
while true; do
    svmresult=`ps | grep '/root/app/svm' | grep -v grep`
    if [ -z "$svmresult" ]; then
      if [ -e "/root/app/app.sab.stage" ]; then
        mv /root/app/app.sab.stage /root/app/app.sab
        rm -rf /root/app/m*.zip
      fi
      # Print date and hwclock. 
      # echo -n "Soft Clock : " && date && echo -n "Hard Clock : " && hwclock
      # Secona would change the RTC device, 
      # so we have to sync /dev/rtc0 by hwclock before start svm.
      hwclock -w
      sleep 1
      if [ ! -e "/root/app/current_first_ip" ]; then  
          cd /root/app/
          # echo "lujiaming --------------We need /root/app/current_first_ip ---------------------" 
          cd - 1>/dev/null 2>&1
      else                                                                                        
          cd /root/app/
          # ./svm app_134.scode app.sab >> svm_output.log &
          cd - 1>/dev/null 2>&1
      fi
      cd - 1>/dev/null 2>&1
      # The svm does not always run successfully
      echo -n "Not Running" > /root/app/svm_info.txt
    else
      echo -n "Running" > /root/app/svm_info.txt
    fi
    # echo "lujiaming --------------We need /root/app/current_first_ip ---------------------" 
    # ifconfig eth0    
    # echo "lujiaming --------------We need /root/app/current_first_ip ---------------------" 
    # ifconfig -a 
    # echo "lujiaming --------------We need /root/app/current_second_ip ---------------------" 
    # ifconfig eth1   
    # echo "lujiaming --------------We need /root/app/current_second_ip ---------------------" 

    eth0result=`ifconfig eth0 | grep "inet addr"`
    if [ -z "$eth0result" ]; then
        if [ ! -e "/root/app/current_first_ip" ]; then
            ifconfig eth0 192.168.168.129
        else
            FIRSTIPADDR=`cat /root/app/current_first_ip`
            if [ $? -eq 0 ]; then
                echo "lujiaming --------------FIRSTIPADDR=$FIRSTIPADDR---------------------"
                ifconfig eth0 $FIRSTIPADDR
            else
                ifconfig eth0 192.168.168.129
            fi
        fi
    fi
    # echo "lujiaming --------------We need /root/app/current_first_ip ---------------------" 
    # eth1result=`ifconfig eth1 | grep "inet addr"`
    # if [ -z "$eth1result" ]; then
    #    if [ ! -e "/root/app/current_second_ip" ]; then
    #        ifconfig eth1 192.168.168.130
    #    else
    #        SECONDIPADDR=`cat /root/app/current_second_ip`
    #        if [ $? -eq 0 ]; then
    #            # echo "lujiaming --------------SECONDIPADDR=$SECONDIPADDR---------------------"
    #            ifconfig eth1 $SECONDIPADDR
    #        else 
    #            ifconfig eth1 192.168.168.130                
    #        fi
    #    fi
    # fi


    # echo "lujiaming --------------We need /root/app/current_second_ip ---------------------" 
    # Output top info
    if [ $top_count -gt 9 ]; then                         
        # top -b -n 1 -d 0 | head -2 > /data/svm/top_output.txt
        /root/app/www/output_topinfo.sh &
        sync
        top_count=0
    fi
    top_count=`expr $top_count + 1`
    # echo "lujiaming --------------We have top_count = $top_count ---------------------" 
    # end of Output top info 
    sleep 5
done

