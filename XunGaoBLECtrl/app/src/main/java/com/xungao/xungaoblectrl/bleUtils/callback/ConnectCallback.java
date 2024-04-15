package com.xungao.xungaoblectrl.bleUtils.callback;

/**
 * 类名: ConnectCallback
 * 描述: 连接回调
 */
public interface ConnectCallback {
    /**
     *  获得通知之后
     */

    void onConnSuccess();

    /**
     * 断开或连接失败
     */
    void onConnFailed();
}
