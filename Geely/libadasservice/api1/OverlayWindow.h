
#ifndef OVERLAYWINDOW_H_
#define OVERLAYWINDOW_H_

#include <ui/DisplayInfo.h>
#include <gui/SurfaceControl.h>
#include <gui/SurfaceComposerClient.h>
#include <utils/RefBase.h>
#include <android/native_window.h>

#include "../AdasServiceImpl.h"
#include "AdasDefine.h"


namespace android {

class OverlayWindow : public RefBase
{
public:
    OverlayWindow();
    virtual ~OverlayWindow();
    void init();
    void startWindow();
    void stopWindow();
    sp<SurfaceControl> createViewSurface(viewInfo& viewInfo);
    void registerSurfaceView(sp<SurfaceControl>& viewcontrol);

private:
    DisplayInfo *mDisplayInfo;
    sp<SurfaceComposerClient> mComposerClient;
    Vector<sp<SurfaceControl> > m_vViewList;
    sp<SurfaceControl> m_pSurfaceControl;
};


}/*namespace android*/

#endif /* OVERLAYWINDOW_H_ */
