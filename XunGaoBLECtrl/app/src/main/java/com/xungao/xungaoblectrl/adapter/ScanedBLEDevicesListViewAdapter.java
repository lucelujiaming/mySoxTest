package com.xungao.xungaoblectrl.adapter;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.xungao.xungaoblectrl.R;

import java.util.ArrayList;
import java.util.List;


/**
 * 已扫描到的设备适配器
 */
public class ScanedBLEDevicesListViewAdapter extends BaseAdapter {
    private static final String DEVICE_PREFIX = "FHM-";

    private Activity mContext;
    public List<BluetoothDevice> mBleDevices;
    // Rssi信号强度，表示距离自己的距离。单位为米。
    private List<Double> mRssis;

    public ScanedBLEDevicesListViewAdapter(Activity mContext) {
        this.mContext = mContext;
        mBleDevices = new ArrayList<BluetoothDevice>();
        mRssis = new ArrayList<Double>();
    }

    public void addDevice(BluetoothDevice device, Double rssi) {
        // Only add FHM-**** and set with AT+NAME=FHM-****
        if (!mBleDevices.contains(device)) {
            String name = device.getName();
            if(name != null)
            {
                if(name.startsWith(DEVICE_PREFIX)) {
                    mBleDevices.add(device);
                    mRssis.add(rssi);
                    notifyDataSetChanged();
                }
            }
        }
    }

    public BluetoothDevice getDevice(int position) {
        if (mBleDevices != null) {
            return mBleDevices.get(position);
        }
        return null;
    }

    public String getAddress(int position) {
        if (mBleDevices != null) {
            return mBleDevices.get(position).getAddress();
        }
        return null;
    }

    public void clear() {
        mBleDevices.clear();
    }

    @Override
    public int getCount() {
        if (mBleDevices != null) {
            return mBleDevices.size();
        }
        return 0;
    }

    @Override
    public Object getItem(int i) {
        return mBleDevices.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        ScanedBLEDevicesViewHolder viewHolder;
        if (view == null) {
            view = LayoutInflater.from(mContext).inflate(R.layout.scaned_bledevices_list_item, viewGroup, false);
            viewHolder = new ScanedBLEDevicesViewHolder();
            viewHolder.txt_name = (TextView) view.findViewById(R.id.txt_name);
            viewHolder.txt_mac = (TextView) view.findViewById(R.id.txt_mac);
            viewHolder.txt_rssi = (TextView) view.findViewById(R.id.txt_rssi);
            view.setTag(viewHolder);
        } else {
            viewHolder = (ScanedBLEDevicesViewHolder) view.getTag();
        }
        BluetoothDevice mdevice = mBleDevices.get(i);
        Double mrssi = mRssis.get(i);

        String name = mdevice.getName();
        String mac = mdevice.getAddress();
        viewHolder.txt_name.setText(name);
        viewHolder.txt_mac.setText(mac);
        viewHolder.txt_rssi.setText(String.format("%.2f", mrssi) + "米");
        return view;
    }
}

class ScanedBLEDevicesViewHolder {
    TextView txt_name;
    TextView txt_mac;
    TextView txt_rssi;
}
