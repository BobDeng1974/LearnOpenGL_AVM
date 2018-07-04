#if !defined(_GraphicPDC_H)
#define _GraphicPDC_H

#include "IGraphicPDC.h"
#include "HeadDefine.h"


class GraphicPDC : public IGraphicPDC
{
public:
    GraphicPDC();
    virtual ~GraphicPDC();

    virtual void InitPDC(int screenWidth, int screenHeight);
    virtual bool bSetPDCLeftTop(int x, int y);
    virtual bool bSetPDCSize(int w, int h);
    virtual bool bSetGraphicPDCState(int *pdcStatus);
    virtual bool bSetGraphicPDCPause();
    virtual bool bSetGraphicPDCResume();    
    virtual bool bSetPDCDisable();


    void saveAsBmp(char* path, void* databuffer);
    bool ReadConfigFile(const char* cfgPath);
    void setMode();
    void SetSumNum();
    void InitGroupMap();
    void LoadTextureData(int index);
    void MallocGroupPlaneData();
    void MallocGroupBorderData(int index);

    void CalPlaneData();
    void CalPlaneBorder(int index);

    void LoadTextures();
    void CompoundTex();
    void InitPdcFinish();
    bool getPdcInitStatus() {return m_initStatus;};
    


protected:
    planesData* m_Planes;//save info from side line and distance mark line according groups
    //int m_iScreenWidth;
    //int m_iScreenHeight;
    bool m_bPause;

    groupMap* m_iGroupMaps;

    //configuration information
    int m_iGroupNum;    //show group
    int sumNum;
    int dayCamPosx,dayCamPosy,dayCamTexWidth,dayCamTexHeight;
    int dayNoCamPosx,dayNoCamPosy,dayNoCamTexWidth,dayNoCamTexHeight;
    int nightCamPosx,nightCamPosy,nightCamTexWidth,nightCamTexHeight;
    int nightNoCamPosx,nightNoCamPosy,nightNoCamTexWidth,nightNoCamTexHeight;
    int posX,posY,texWidth,texHeight,planeWidth,planeHeight;// position and size
    int* familyNum;
    int *defaultShow;
    string modeStr,texFilePath;

    //files path
    string texfilesPathDayCam,texfilesPathDayNoCam,texfilesPathNightCam,texfilesPathNightNoCam;
    string familyNumStr,defaultShowStr;
    string texNames[200];
    char *tex;
    bool m_initStatus;
};


#endif