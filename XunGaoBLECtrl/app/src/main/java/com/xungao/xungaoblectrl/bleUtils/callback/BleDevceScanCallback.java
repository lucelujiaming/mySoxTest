package com.xungao.xungaoblectrl.bleUtils.callback;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

/**
 * 类名: BleDeviceScanCallback
 * 描述: 扫描成功的回调
 */
public class BleDevceScanCallback implements BluetoothAdapter. LeScanCallback {
    private ScanCallback mScanCallback;

    public BleDevceScanCallback(ScanCallback scanCallback) {
        this.mScanCallback=scanCallback;
    }

    @Override
    public void onLeScan(BluetoothDevice device, int rssi, byte[] scanRecord) {
        if (null != mScanCallback) {
            //每次扫描到设备会回调此方法,这里一般做些过滤在添加进list列表
            mScanCallback.onScanning(device, rssi, scanRecord);
        }
    }
}
