#include "GraphicVPAFord.h"
#include "GraphicPDC.h"

GraphicVPAFord::GraphicVPAFord()
    :AdasRunable("GraphicPDC")
{
    PDCL_LOG_INFO("GraphicVPAFord | GraphicVPAFord");
    m_bPause =false;
    m_surface = NULL;
}


GraphicVPAFord::~GraphicVPAFord()
{
//debug

}

void GraphicVPAFord::vInit(ANativeWindow *surface,int screenWidth, int screenHeight)
{
    PDCL_LOG_INFO("GraphicVPAFord | vInit");
    setscSize(screenWidth,screenHeight);
    m_surface = surface;
}

void GraphicVPAFord::init()
{
     PDCL_LOG_INFO("GraphicVPAFord | init");
     InitPDC(m_iWidth,m_iHeight);
     InitView(m_surface, m_iWidth, m_iHeight);
     PDCL_LOG_INFO("GraphicVPAFord | init end!!!!");
     // saveAsBmp("/data/456.bmp", (void*)tex);
     // generateTextures(tex, false);
}

bool GraphicVPAFord::bSetVPALeftTop(int x, int y)
{
    bSetPDCLeftTop(x,y);
    return true;
}


bool GraphicVPAFord::bSetVPASize(int w, int h)
{
    bSetPDCSize(w,h);    
    return true;
}

 void GraphicVPAFord::update()
 {
    //bug on opgl init must in a thread 
    //make sure init is done...
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    Render();
    eglSwapBuffers(mEglDisplay, mEglSurface);

    usleep(30000);//30ms sleep  
 }

void GraphicVPAFord::Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(m_RenderObj.uiId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_uiTexture);
    glUniform2f(m_RenderObj.uiImgSize,m_iWidth-1,m_iHeight-1);

    planesData lineGroup = m_Planes[0];
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    // saveAsBmp("/data/456.bmp", (void*)tex);
    //
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);

   // lineGroup.pBorderData[10] = 0;
   // lineGroup.pBorderData[11] = 0;

   // lineGroup.pBorderData[14] = 1;
   // lineGroup.pBorderData[15] = 0;

    glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)lineGroup.pBorderData);
    glVertexAttribPointer(TEXCOORD_ARRAY,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(&lineGroup.pBorderData[2]));

    glEnableVertexAttribArray(VERTEX_ARRAY);
    glEnableVertexAttribArray(TEXCOORD_ARRAY);
    glDrawElements(GL_TRIANGLES,lineGroup.pBorderIndexNum,GL_UNSIGNED_SHORT,lineGroup.pBorderIndex);
    // cout<<"lineGroup.pBorderData[0] ---------->"<<lineGroup.pBorderData[0]<<endl;
    

    glDisableVertexAttribArray(VERTEX_ARRAY);
    glDisableVertexAttribArray(TEXCOORD_ARRAY);
    glDisable(GL_BLEND);
    // drawToSurface(false, 0);
   
}


void GraphicVPAFord::setscSize(int screenWidth,int screenHeight)
 {
    m_iHeight = screenHeight;
    m_iWidth = screenWidth;
 }


 int GraphicVPAFord::getScSizeWidth()
 {
    return m_iWidth;
 }


 int GraphicVPAFord::getScSizeHeight()
 {
    return m_iHeight;
 }







