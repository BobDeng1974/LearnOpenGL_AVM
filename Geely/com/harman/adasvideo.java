package com.harman;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.os.RemoteException;
import java.io.IOException;

public class adasvideo{

	public final void setPreviewDisplay(SurfaceHolder holder) throws IOException {
        if (holder != null) {
            setPreviewSurface(holder.getSurface());
        } else {
            setPreviewSurface((Surface)null);
        }
    }

	public final void setGuideLineSurface(SurfaceHolder holder) throws IOException {
        if (holder != null) {
            enableGuideLine(holder.getSurface());
        } else {
            enableGuideLine((Surface)null);
        }
    }


    static {
        System.loadLibrary("adasvideo");
    }

    public native final void enableGuideLine(Surface surface);

    public native final void setPreviewSurface(Surface surface);

    public native final void startPreview();

    public native final void showGuideLine();

    public native final void connect();

}