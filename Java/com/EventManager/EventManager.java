package com.EventManager;

import android.util.Log;

import com.EventManager.EventManagerDefinitions.EN_OSYSTEM_EVENT_LIST;

/**
 * Created by heji on 2016/7/7.
 */

public final class EventManager {

    private static final String LOG_TAG = "OSrv_EventService";
    private static EventManager mOSrv_EventService = null;
    public static EventManager getInstance() {
        synchronized (EventManager.class) {
            if (mOSrv_EventService == null) {
                new EventManager();
            }
        }
        return mOSrv_EventService;
    }

    private EventManager() {
        Log.d(LOG_TAG, "OSrv_EventService Created~");
        mOSrv_EventService = this;
        Connect();
    }

    static {
        Log.d(LOG_TAG, "EventManager Load library~");
        try {
            System.loadLibrary("JNI_OceanusTv_OClt_EventManager");
        } catch (UnsatisfiedLinkError e) {
            Log.d(LOG_TAG, "Cannot load JNI_OceanusTv_OClt_EventManager library:\n" + e.toString());
        }
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        Disconnect();
    }

    public void registeEventListener(Tv_EventListener listener, EN_OSYSTEM_EVENT_LIST event) {
        RegisteEventListener(listener, listener.getListenerName(), event.ordinal());
    }

    public void unregisteEventListener(Tv_EventListener listener, EN_OSYSTEM_EVENT_LIST event) {
        UnregisteEventListener(listener.getListenerName(), event.ordinal());
    }

    public void sendEvent(Tv_EventInfo info, String listener_name) {
        SendEvent(info, listener_name);
    }

    public void sendBroadcast(Tv_EventInfo info) {
        SendBroadcast(info);
    }

    public void enableSystemTimeEvent(Tv_EventInfo info, int time_sec)
    {
        EnableSystemTimeEvent(info,time_sec);
    }
    public void disableSystemTimeEvent(int event)
    {
        DisableSystemTimeEvent(event);
    }
    private native void SendEvent(Tv_EventInfo info, String listener_name);
    private native void SendBroadcast(Tv_EventInfo info);
    private native void RegisteEventListener(Tv_EventListener listener, String name, int event);
    private native void UnregisteEventListener(String listener_name, int event);
    private native void DisableSystemTimeEvent(int event);
    private native void EnableSystemTimeEvent(Tv_EventInfo info,int time_sec);
    private native void Connect();
    private native void Disconnect();
}
