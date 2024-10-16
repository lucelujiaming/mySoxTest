package com.xungao.xungaoblectrl.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import com.xungao.xungaoblectrl.R;
import com.xungao.xungaoblectrl.adapter.PanelInfoListViewAdapter;
import com.xungao.xungaoblectrl.bleUtils.BleController;
import com.xungao.xungaoblectrl.bleUtils.callback.OnReceiverCallback;
import com.xungao.xungaoblectrl.bleUtils.callback.OnWriteCallback;
import com.xungao.xungaoblectrl.utils.EnumInfo;
import com.xungao.xungaoblectrl.utils.HexUtil;
import com.xungao.xungaoblectrl.utils.PanelDeviceInfo;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

/**
 * 类名: PanelOperateActivity
 * 描述: NULL
 */
public class PanelOperateActivity extends BaseActivity {

    private static final Integer CMD_READ_PANELINFO = 0x01;
    private static final Integer CMD_PANELINFO_RSP = 0x02;
    private static final Integer CMD_PANELINFO_IMPLEMENT_RSP = 0x03;
    private static final Integer CMD_PANELINFO_ACK = 0x04;
    private static final Integer CMD_MOD_PANELINFO = 0x05;
    private static final Integer CMD_MOD_PANELINFO_ACK = 0x06;

    private static final Integer DATA_TYPE_ENUM = 0x01;
    private static final Integer DATA_TYPE_INT = 0x02;
    private static final Integer DATA_TYPE_FLOAT = 0x03;
    private static final Integer DATA_TYPE_DOUBLE = 0x04;

    private static final Integer CMD_MOD_PANELINFO_ACK_OK = 0xC8;
    private static final Integer CMD_MOD_PANELINFO_ACK_RO = 0x193;

    private static final String TAG = "PanelOperateActivity";

    private String[] unit_names = {"", "%", "m/s", "m3/h", "mm", "Ω", "mV", "Pa", "s", "V", "度"};
    //
    // private Integer mPanelInfoCount;


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
    @BindView(R.id.panel_info_listview)
    ListView mPanleInfoListview;

    //震动指令
    private String mShockinstructions = "AA5502010202020232003c";
    //
    private String mReadPanelInfo = "010000";
    private String mPanelInfoListAck = "040000";

    private BleController mBleController;
    private PanelInfoListViewAdapter mPanelInfoListAdapter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_panel_operate);

        // ButterKnife俗称黄油刀,是控件注入框架,可以帮助开发者避免重复初始化控件的工作,简单快捷的初始化布局中的控件,极大提升开发效率。
        ButterKnife.bind(this);
        setToolbar();
        initListview();
        //
        final TextView textTitle  = (TextView) findViewById(R.id.tv_recv_title);
        final TextView textMsg = (TextView) findViewById(R.id.tv_msg);
        final TextView textMsg2 = (TextView) findViewById(R.id.tv_msg_2);
        textTitle.setVisibility(View.GONE);
        textMsg.setVisibility(View.GONE);
        textMsg2.setVisibility(View.GONE);

        btnSend.setEnabled(false);
        //获得实例
        mBleController = BleController.getInstance();
        // mPanelInfoCount = 0 ;

        // TODO 接收数据的监听
        mBleController.RegistReciveListener(TAG, new OnReceiverCallback() {
            @Override
            public void onReceiver(byte[] value) {
                // 显示收到的数据。
                tvMsg.setText(HexUtil.bytesToHexString(value));
                if(value[0] == CMD_PANELINFO_RSP)
                {
                    // mPanelInfoCount = 0;
                    mPanelInfoListAdapter.clear();
                    // Add Column Header
                    // PanelDeviceInfo objPanelDeviceInfo = new PanelDeviceInfo();
                    // objPanelDeviceInfo.seqnum = "序号";
                    // objPanelDeviceInfo.name   = "名称";
                    // objPanelDeviceInfo.dataType = 0;
                    // objPanelDeviceInfo.dataFormat = 0;
                    // objPanelDeviceInfo.unit = "(单位)";
                    // objPanelDeviceInfo.value = "设定值";
                    // mPanelInfoListAdapter.addDevice(objPanelDeviceInfo);
                    FillPanelList(value);
                    Write(mPanelInfoListAck);
                    btnSend.setEnabled(true);
                    
                }
                // 这个数据包发送一次，就会收到很多次。好像是蓝牙本身的特性。无法解决。
                else if(value[0] == CMD_PANELINFO_IMPLEMENT_RSP)
                {
                    FillPanelList(value);
                    Write(mPanelInfoListAck);
                }
                else if(value[0] == CMD_MOD_PANELINFO_ACK)
                {
                    Integer iSeq = Integer.valueOf(value[3]);
                    short shortRetCode = HexUtil.byteArray2short(value, 4);
                    if(shortRetCode == CMD_MOD_PANELINFO_ACK_OK)
                    {
                        updateElementInPanelList(iSeq);
                    }
                    else if(shortRetCode == CMD_MOD_PANELINFO_ACK_RO)
                    {
                        AlertDialog.Builder builder = new AlertDialog.Builder(PanelOperateActivity.this);
                        builder.setMessage("该值为只读变量，无法修改！");
                        builder.setPositiveButton("确定", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // 用户点击确定按钮后执行的操作
                            }
                        });
                        AlertDialog dialog = builder.create();
                        dialog.show();
                    }
                }
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        readPanelInfo();
    }

    private int AddPanelList(byte[] value, int iPos)
    {
        PanelDeviceInfo objPanelDeviceInfo = new PanelDeviceInfo();
        // java.lang.ArrayIndexOutOfBoundsException: length=4; index=4
        if((iPos > 0) && (iPos + 4 < value.length))
        {
            objPanelDeviceInfo.seqnum = String.valueOf(value[iPos]);
            Integer iNameLen = Integer.valueOf(value[iPos + 1]);
            Integer iEnumOptionsLen = 0;
            byte byteName[] = new byte[iNameLen];
            Integer copyPos = iPos + 2;
            if(value.length - copyPos < iNameLen)
            {
                Toast.makeText(PanelOperateActivity.this, objPanelDeviceInfo.name + "的值错误!", Toast.LENGTH_SHORT).show();
                return -1;
            }
            System.arraycopy(value, copyPos, byteName, 0, iNameLen);
            objPanelDeviceInfo.name = new String(byteName, StandardCharsets.UTF_8);
            if(objPanelDeviceInfo.name == null)
            {
                Toast.makeText(PanelOperateActivity.this, objPanelDeviceInfo.name + "的值错误!", Toast.LENGTH_SHORT).show();
                return -1;
            }

            copyPos = iPos + 4 + iNameLen;
            if((copyPos > 0) && (value.length - copyPos <= 0))
            {
                Toast.makeText(PanelOperateActivity.this, objPanelDeviceInfo.name + "的值错误!", Toast.LENGTH_SHORT).show();
                return -1;
            }
            objPanelDeviceInfo.dataType = Integer.valueOf(value[iPos + 2 + iNameLen]);
            objPanelDeviceInfo.dataFormat  = Integer.valueOf(value[iPos + 3 + iNameLen]);
            Integer iUnit = Integer.valueOf(value[iPos + 4 + iNameLen]);
            if(iUnit <= unit_names.length)
            {
                objPanelDeviceInfo.unit  = " " + unit_names[iUnit];
            }
            else
            {
                objPanelDeviceInfo.unit  = " ";
            }

            byte byteValue[] = new byte[4];
            copyPos = iPos + 5 + iNameLen;
            if(value.length - copyPos < 4)
            {
                Toast.makeText(PanelOperateActivity.this, objPanelDeviceInfo.name + "的值错误!", Toast.LENGTH_SHORT).show();
                return -1;
            }

            if((objPanelDeviceInfo.dataType == DATA_TYPE_INT))
            {
                System.arraycopy(value, copyPos, byteValue, 0, 4);
                objPanelDeviceInfo.value = String.valueOf(HexUtil.bytes2IntBig(byteValue));
            }
            else if(objPanelDeviceInfo.dataType == DATA_TYPE_ENUM) {
                System.arraycopy(value, copyPos, byteValue, 0, 4);
                // Old Enum
                if(byteValue[1] == 0x00)
                {
                    objPanelDeviceInfo.value = String.valueOf(HexUtil.bytes2IntBig(byteValue));
                }
                // New Enum
                else {
                    objPanelDeviceInfo.arrayEnumOptions = new ArrayList<EnumInfo>();
                    Integer iEnumOptionsCount = Integer.valueOf(byteValue[1]);
                    // Set value of Enum
                    Integer currentValue = Integer.valueOf(byteValue[0]);
                    // objPanelDeviceInfo.value = String.valueOf(HexUtil.bytes2IntBig(byteValue));
                    objPanelDeviceInfo.value = "未定义";
                    for(int i = 0; i < iEnumOptionsCount; i++)
                    {
                        copyPos = iPos + 7 + iNameLen + iEnumOptionsLen;
                        EnumInfo objEnumOption  = new EnumInfo();
                        objEnumOption.enumNumber = Integer.valueOf(value[copyPos]);
                        copyPos = iPos + 8 + iNameLen + iEnumOptionsLen;
                        Integer iEnumOptionNameLen = Integer.valueOf(value[copyPos]);

                        copyPos = iPos + 9 + iNameLen + iEnumOptionsLen;
                        byte byteEnumOptionName[] = new byte[iEnumOptionNameLen];
                        System.arraycopy(value, copyPos, byteEnumOptionName, 0, iEnumOptionNameLen);
                        objEnumOption.enumName = new String(byteEnumOptionName, StandardCharsets.UTF_8);
                        if(objEnumOption.enumName == null)
                        {
                            Toast.makeText(PanelOperateActivity.this, objEnumOption.enumName + "的值错误!", Toast.LENGTH_SHORT).show();
                            return -1;
                        }
                        if(currentValue == objEnumOption.enumNumber)
                        {
                            objPanelDeviceInfo.value = objEnumOption.enumName;
                        }
                        objPanelDeviceInfo.arrayEnumOptions.add(objEnumOption);
                        // EnumOption includes value, length of name and name.
                        iEnumOptionsLen += iEnumOptionNameLen + 2;
                    }
                }
                // Enum return here
                objPanelDeviceInfo.modValue = objPanelDeviceInfo.value;
                mPanelInfoListAdapter.addDevice(objPanelDeviceInfo);
                // 5 + enum value(1) + option count(1) + iEnumOptionsLen
                return iNameLen + 7 + iEnumOptionsLen;
            }
            else if(objPanelDeviceInfo.dataType == DATA_TYPE_FLOAT)
            {
                System.arraycopy(value, copyPos, byteValue, 0, 4);
                float fValue = HexUtil.bytesToFloat(byteValue);
                if(objPanelDeviceInfo.dataFormat > 0)
                {
                    objPanelDeviceInfo.value =  String.format(
                            "%." + String.valueOf(objPanelDeviceInfo.dataFormat) + "f",
                            fValue);
                }
                else
                {
                    objPanelDeviceInfo.value = String.valueOf(fValue);
                }
            }
            else
            {
                System.arraycopy(value, copyPos, byteValue, 0, 4);
                objPanelDeviceInfo.value = String.valueOf(HexUtil.bytes2IntBig(byteValue));
            }
            objPanelDeviceInfo.modValue = objPanelDeviceInfo.value;
            mPanelInfoListAdapter.addDevice(objPanelDeviceInfo);
            return iNameLen + 9;
        }
        return -1;
    }

    private void FillPanelList(byte[] value)
    {
        Integer iPosition = 3 ;
        Integer iListCount = Integer.valueOf(value[3]);
        iPosition = iPosition + 1;
        for(int i = 0; i < iListCount; i++)
        {
            iPosition += AddPanelList(value, iPosition);
            if(iPosition > value.length)
            {
                System.out.printf("iPos = %d \r\n", iPosition);
                break;
            }
        }
    }

    private void updateElementInPanelList(int iSeq)
    {
        mPanelInfoListAdapter.updateDevice(iSeq);
    }

    /**
     * 初始化listview
     */
    private void initListview() {
        // View headerView = LayoutInflater.from(this).inflate(R.layout.panel_info_list_header, null, false);
        // 创建头部视图
        View headerView = LayoutInflater.from(this).inflate(R.layout.panel_info_list_header, mPanleInfoListview, false);
        mPanleInfoListview.addHeaderView(headerView);

        mPanelInfoListAdapter = new PanelInfoListViewAdapter(PanelOperateActivity.this);
        mPanleInfoListview.setAdapter(mPanelInfoListAdapter);

        // View headerView = getLayoutInflater().inflate(R.layout.panel_info_list_header, null);
        // mPanleInfoListview.setFixedHeader(headerView);

        mPanleInfoListview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // Not deal wiht the first line of listView,
                // if(position == 1)
                // {
                //      return;
                // }
                // position should skip Listhead: 序号，名字，设定值(单位)。
                final PanelDeviceInfo device = mPanelInfoListAdapter.getDevice(position - 1);
                // Toast.makeText(PanelOperateActivity.this, strToast, Toast.LENGTH_SHORT).show();

                if((device.dataType == DATA_TYPE_ENUM) && (device.arrayEnumOptions != null)) {
                    // 在Activity中创建和显示自定义对话框
                    LayoutInflater inflater = getLayoutInflater();
                    View dialogView = inflater.inflate(R.layout.modify_enum_panel_info, null);
                    // 获取布局中的控件
                    final RadioGroup radioGroup = new RadioGroup(PanelOperateActivity.this);
                    radioGroup.setOrientation(RadioGroup.VERTICAL);

                    // 循环添加RadioButton
                    for (int i = 0; i < device.arrayEnumOptions.size(); i++) {
                        RadioButton radioButton = new RadioButton(PanelOperateActivity.this);
                        EnumInfo objEnumInfo = device.arrayEnumOptions.get(i);
                        radioButton.setText(objEnumInfo.enumNumber + " - " + objEnumInfo.enumName);
                        radioButton.setId(objEnumInfo.enumNumber);
                        radioButton.setButtonTintList(ColorStateList.valueOf(Color.GREEN));
                        if(objEnumInfo.enumName == device.value)
                        {
                            radioButton.setChecked(true);
                        }
                        else
                        {
                            radioButton.setChecked(false);
                        }
                        radioGroup.addView(radioButton);
                    }

                    final LinearLayout radioGroupLayout = (LinearLayout) dialogView.findViewById(R.id.enum_radio_group);
                    radioGroupLayout.addView(radioGroup);

                    // 设置监听器来响应用户的选择
                    radioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                        @Override
                        public void onCheckedChanged(RadioGroup group, int checkedId) {
                            // 通过checkedId可以得知哪个RadioButton被选中
                            for (int i = 0; i < device.arrayEnumOptions.size(); i++) {
                                EnumInfo objEnumInfo = device.arrayEnumOptions.get(i);
                                if(checkedId == objEnumInfo.enumNumber)
                                {
                                    Toast.makeText(PanelOperateActivity.this,
                                            objEnumInfo.enumName + " selected", Toast.LENGTH_SHORT).show();
                                    break;
                                }
                            }
                        }
                    });

                    final Button btnModify = (Button) dialogView.findViewById(R.id.enum_btn_modify);
                    final Button btnCancel = (Button) dialogView.findViewById(R.id.enum_btn_cancel);

                    AlertDialog.Builder builder = new AlertDialog.Builder(PanelOperateActivity.this);
                    builder.setView(dialogView);
                    final AlertDialog alertDialog = builder.create();
                    btnModify.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            // TODO Auto-generated method stub
                            Integer Id = radioGroup.getCheckedRadioButtonId();
                            ModifyDeviceValue(device, Id.toString());

                            for (int i = 0; i < device.arrayEnumOptions.size(); i++) {
                                EnumInfo objEnumInfo = device.arrayEnumOptions.get(i);
                                if(Id == objEnumInfo.enumNumber)
                                {
                                    device.modValue = objEnumInfo.enumName;
                                    break;
                                }
                            }
                            alertDialog.dismiss();// 对话框消失
                        }
                    });
                    btnCancel.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            // TODO Auto-generated method stub
                            alertDialog.dismiss();// 对话框消失
                        }
                    });
                    alertDialog.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
                    alertDialog.show();
                }
                else {
                    // 在Activity中创建和显示自定义对话框
                    LayoutInflater inflater = getLayoutInflater();
                    View dialogView = inflater.inflate(R.layout.modify_panel_info, null);
                    // 获取布局中的控件
                    final EditText item_value = (EditText) dialogView.findViewById(R.id.item_value);
                    final Button btnModify = (Button) dialogView.findViewById(R.id.btn_modify);
                    final Button btnCancel = (Button) dialogView.findViewById(R.id.btn_cancel);
                    item_value.setText(device.value);

                    AlertDialog.Builder builder = new AlertDialog.Builder(PanelOperateActivity.this);
                    builder.setView(dialogView);
                    final AlertDialog alertDialog = builder.create();
                    btnModify.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            // TODO Auto-generated method stub
                            String strValue = item_value.getText().toString();
                            // String strToast = new String();
                            // strToast = device.name + " change into " + strValue + device.unit;
                            // Toast.makeText(PanelOperateActivity.this, strToast, Toast.LENGTH_SHORT).show();
                            ModifyDeviceValue(device, strValue);
                            device.modValue = strValue;
                            alertDialog.dismiss();// 对话框消失
                        }
                    });
                    btnCancel.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            // TODO Auto-generated method stub

                            alertDialog.dismiss();// 对话框消失
                        }
                    });
                    alertDialog.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
                    alertDialog.show();
                }
            }
        });
    }

    private void ModifyDeviceValue(PanelDeviceInfo device, String strValue){
        String strSend = new String();
        strSend = String.format("%02X", CMD_MOD_PANELINFO);
        strSend = strSend + "0006";
        strSend = strSend + String.format("%02X%02X",
                Integer.valueOf(device.seqnum), Integer.valueOf(device.dataType));
        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.putFloat(Float.parseFloat(strValue));
        byte[] byteArray = buffer.array();
        strSend = strSend + String.format("%02X%02X%02X%02X",
                byteArray[0], byteArray[1], byteArray[2], byteArray[3]);
        Write(strSend);
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
                // Write(mShockinstructions);
                Write(mReadPanelInfo);
                btnSend.setEnabled(false);
                break;
        }
    }
    // 这个函数不能在进入
    private void readPanelInfo(){
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                //执行耗时操作
                try {
                    Thread.sleep(3000);
                    WriteNoResponse(mReadPanelInfo);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        t.start();
    }

    // 发送数据
    public void Write(String value){
        mBleController.WriteBuffer(value, new OnWriteCallback() {
            @Override
            public void onSuccess() {
                // Toast.makeText(PanelOperateActivity.this, "ok", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailed(int state) {
                // Toast.makeText(PanelOperateActivity.this, "fail", Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void WriteNoResponse(String value){
        mBleController.WriteBuffer(value, new OnWriteCallback() {
            @Override
            public void onSuccess() {
            }

            @Override
            public void onFailed(int state) {
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
