package com.xungao.xungaoblectrl.bleUtils.callback;

import android.bluetooth.BluetoothDevice;

/**
 * 类名: ScanCallback
 * 描述: 扫描回调
 */
public interface ScanCallback  {
    /**
     * 扫描完成回调
     */
    void onSuccess();

    /**
     * 扫描过程中,每扫描到一个设备回调一次
     *
     * @param device     扫描到的设备
     * @param rssi       设备的信息强度
     * @param scanRecord
     */
    void onScanning(final BluetoothDevice device, int rssi, byte[] scanRecord);
}
