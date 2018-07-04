#if !defined(_GraphicVPAFord_H)
#define _GraphicVPAFord_H

#include "GraphicPDC.h"
#include "GlShell.h"
#include "AdasRunable.h"
using namespace std;
#define PNG_BYTES_TO_CHECK 4
#define VERTEX_ARRAY 0
#define TEXCOORD_ARRAY 1

using namespace android;

class GraphicVPAFord : public GraphicPDC, public GlShell, public AdasRunable
{
public:
    GraphicVPAFord();
    ~GraphicVPAFord();
    void vInit(ANativeWindow *surface, int screenWidth, int screenHeight);
    void setscSize(int w,int h);
    int getScSizeWidth();
    int getScSizeHeight();

   

    bool bSetVPALeftTop(int x, int y);
    bool bSetVPASize(int w, int h);
    
    // bool bSetGraphicVPAPause();
    // bool bSetGraphicVPAResume();    
    virtual void update();
    virtual void init();

    void Render();
    void RenderGroupLines(int index);

private:
    //share vary value
    int temp_x;
    int temp_y;
    float rescale ;
    
    int m_iWidth;
    int m_iHeight;

    ANativeWindow* m_surface;
};

#endif