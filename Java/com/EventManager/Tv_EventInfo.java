package com.EventManager;

import org.json.JSONException;
import org.json.JSONObject;

import com.EventManager.EventManagerDefinitions.EN_EVENT_INFO_TYPE;


/**
 * Created by heji on 2016/7/14.
 */
public class Tv_EventInfo {
    private int event;
    private long infoNumber;
    private String infoString;
    private JSONObject infoJObject;
    private EN_EVENT_INFO_TYPE infoType;
    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        //Log.d("Oceanus","######gc eventinfo "+Tv_EventInfo.this+"in java~~##############");
    }
    public Tv_EventInfo(int event)
    {
        this.event = event;
        infoType = EN_EVENT_INFO_TYPE.E_EVENT_INFO_TYPE_NULL;
    }

    public Tv_EventInfo(int event, String info_str,boolean bIsJson)
    {
        this.event = event;
        if(bIsJson)
        {
            infoType = EN_EVENT_INFO_TYPE.E_EVENT_INFO_TYPE_JSON;

        }
        else
        {
            infoType = EN_EVENT_INFO_TYPE.E_EVENT_INFO_TYPE_STRING;
        }
        this.infoString = info_str;
    }
    public Tv_EventInfo(int event, JSONObject jObject)
    {
        this.event = event;
        this.infoString = jObject.toString();
        infoType = EN_EVENT_INFO_TYPE.E_EVENT_INFO_TYPE_JSON;
    }
    public Tv_EventInfo(int event, long info_number)
    {
        this.event = event;
        this.infoNumber = info_number;
        infoType = EN_EVENT_INFO_TYPE.E_EVENT_INFO_TYPE_NUMBER;
    }
    public String getString()
    {
        return this.infoString;
    }
    public JSONObject getJsonObject()
    {
        if(this.infoType == EN_EVENT_INFO_TYPE.E_EVENT_INFO_TYPE_JSON)
        {
            try {
                this.infoJObject = new JSONObject(this.infoString);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            if(this.infoJObject != null)
            {
                return this.infoJObject;
            }
        }
        return null;
    }
    public long getInfoNumber()
    {
        return this.infoNumber;
    }
    public EN_EVENT_INFO_TYPE getInfoType()
    {
        return this.infoType;
    }
    public int getInfoTypeIndex()
    {
        return this.infoType.ordinal();
    }
    public int getEventType()
    {
        return this.event;
    }
}
