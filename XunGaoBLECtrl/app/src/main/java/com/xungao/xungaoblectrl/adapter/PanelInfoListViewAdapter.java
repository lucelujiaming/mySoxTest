package com.xungao.xungaoblectrl.adapter;

import android.app.Activity;
import android.support.v4.content.ContextCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.xungao.xungaoblectrl.R;
import com.xungao.xungaoblectrl.utils.PanelDeviceInfo;

import java.util.ArrayList;
import java.util.List;


/**
 * 已扫描到的设备适配器
 */
public class PanelInfoListViewAdapter extends BaseAdapter {
    private Activity mContext;
    public List<PanelDeviceInfo> mPanelInfo;

    private int rwColor;
    private int roColor;

    private static final Integer DATA_TYPE_RO_ITEM = 0x40;

    public PanelInfoListViewAdapter(Activity mContext) {
        this.mContext = mContext;
        mPanelInfo = new ArrayList<PanelDeviceInfo>();
        rwColor = ContextCompat.getColor(mContext, R.color.lightblue);
        roColor = ContextCompat.getColor(mContext, R.color.white);
    }

    public void addDevice(PanelDeviceInfo device) {
        if (!mPanelInfo.contains(device)) {
                if(findDevice(device.seqnum))
                {
                    return;
                }
                else
                {
                    mPanelInfo.add(device);
                }
        }
        notifyDataSetChanged();
    }
    
    public boolean findDevice(String seqnum)
    {
        for (PanelDeviceInfo element : mPanelInfo) {
            if (element.seqnum == seqnum) {
                return true;
            }
        }
        return false;
    }

    public void updateDevice(int iSeq)
    {
        for (PanelDeviceInfo element : mPanelInfo) {
            if (element.seqnum == String.valueOf(iSeq)) {
                element.value = element.modValue;
                break;
            }
        }
        notifyDataSetChanged();
    }

    public PanelDeviceInfo getDevice(int position) {
        if (mPanelInfo != null) {
            return mPanelInfo.get(position);
        }
        return null;
    }

    public String getValue(int position) {
        if (mPanelInfo != null) {
            return mPanelInfo.get(position).value.toString();
        }
        return "".toString();
    }

    public void clear() {
        mPanelInfo.clear();
    }

    @Override
    public int getCount() {
        if (mPanelInfo != null) {
            return mPanelInfo.size();
        }
        return 0;
    }

    @Override
    public Object getItem(int i) {
        return mPanelInfo.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int position, View view, ViewGroup viewGroup) {
        PanelInfoViewHolder viewHolder;

        if (view == null) {
            view = LayoutInflater.from(mContext).inflate(R.layout.panel_info_list_item, viewGroup, false);
            viewHolder = new PanelInfoViewHolder();
            viewHolder.txt_seqnum = (TextView) view.findViewById(R.id.txt_seqnum);
            viewHolder.txt_name = (TextView) view.findViewById(R.id.txt_name);
            viewHolder.txt_value = (TextView) view.findViewById(R.id.txt_value);
            viewHolder.txt_unit = (TextView) view.findViewById(R.id.txt_unit);
            view.setTag(viewHolder);
        } else {
            viewHolder = (PanelInfoViewHolder) view.getTag();
        }
        PanelDeviceInfo mDeviceInfo = mPanelInfo.get(position);

        String seqnum = mDeviceInfo.seqnum;
        String name = mDeviceInfo.name;
        String unit = mDeviceInfo.unit;
        String value = mDeviceInfo.value;
        viewHolder.txt_seqnum.setText(seqnum);
        viewHolder.txt_name.setText(name);
        viewHolder.txt_value.setText(value.toString());
        viewHolder.txt_unit.setText(unit.toString());

        if(mDeviceInfo.dataType < DATA_TYPE_RO_ITEM)
        {
            view.setBackgroundColor(rwColor);
        }
        else
        {
            view.setBackgroundColor(roColor);
        }

        return view;
    }
}

class PanelInfoViewHolder {
    TextView txt_seqnum;
    TextView txt_name;
    TextView txt_value;
    TextView txt_unit;
}
