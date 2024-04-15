package com.xungao.xungaoblectrl.activity;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.xungao.xungaoblectrl.R;
import com.xungao.xungaoblectrl.bleUtils.BleController;
import com.xungao.xungaoblectrl.bleUtils.callback.OnReceiverCallback;
import com.xungao.xungaoblectrl.bleUtils.callback.OnWriteCallback;
import com.xungao.xungaoblectrl.utils.HexUtil;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

/**
 * 类名: PanelOperateActivity
 * 描述: NULL
 */
public class PanelOperateActivity extends BaseActivity {

    private static final String TAG = "PanelOperateActivity";

    @BindView(R.id.toolbar)
    Toolbar toolbar;
    @BindView(R.id.btn_send)
    Button btnSend;
    @BindView(R.id.btn_senddiy)
    Button btnSenddiy;
    @BindView(R.id.tv_msg)
    TextView tvMsg;
    @BindView(R.id.ed_msg)
    EditText edMsg;

    //震动指令
    private String mShockinstructions = "AA5502010202020232003c";

    private BleController mBleController;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_panel_operate);
        // ButterKnife俗称黄油刀,是控件注入框架,可以帮助开发者避免重复初始化控件的工作,简单快捷的初始化布局中的控件,极大提升开发效率。
        ButterKnife.bind(this);
        setToolbar();
        //获得实例
        mBleController = BleController.getInstance();

        // TODO 接收数据的监听
        mBleController.RegistReciveListener(TAG, new OnReceiverCallback() {
            @Override
            public void onReceiver(byte[] value) {
                // 显示收到的数据。
                tvMsg.setText(HexUtil.bytesToHexString(value));
            }
        });
    }

    /**
     * 设置标题栏
     */
    private void setToolbar() {
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayShowTitleEnabled(false);//设计隐藏标题
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);//设置显示返回键
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

    //点击事件
    @OnClick({R.id.tv, R.id.btn_send, R.id.btn_senddiy, R.id.tv_msg})
    public void onViewClicked(View view) {
        switch (view.getId()) {
            case R.id.btn_senddiy:
                if (TextUtils.isEmpty(edMsg.getText().toString())){
                    Toast.makeText(PanelOperateActivity.this, "请输入消息!", Toast.LENGTH_SHORT).show();
                    return;
                }
                // 发送数据
                Write(edMsg.getText().toString());
                break;
            case R.id.btn_send:
                // 发送数据
                Write(mShockinstructions);
                break;
        }
    }

    // 发送数据
    public void Write(String value){
        mBleController.WriteBuffer(value, new OnWriteCallback() {
            @Override
            public void onSuccess() {
                Toast.makeText(PanelOperateActivity.this, "ok", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailed(int state) {
                Toast.makeText(PanelOperateActivity.this, "fail", Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //移除接收数据的监听
        mBleController.UnregistReciveListener(TAG);
        // TODO 断开连接
        //mBleController.CloseBleConn();
    }

}
