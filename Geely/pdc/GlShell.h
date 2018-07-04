#if !defined(_GlShell_H_)
#define _GlShell_H_
#include "HeadDefine.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace android {


class GlShell
{
public:
    GlShell();
    virtual ~GlShell();
    
    void InitView(ANativeWindow *surface, int screenWidth, int screenHeight);
    void LoadShaders();
    void update();
    bool drawToSurface(bool animationstatus, int frameNo = 0);
    void generateTextures(void *databuffer, bool flag);

private:
    int eglInit() ;
    GLuint LoadShader(GLenum type, const char* shaderSrc);
    int init();

protected:
    GLuint m_uiTexture;
    struct{
        GLuint uiId;
        GLuint uiColor;
        GLuint uiImgSize;
        GLuint uiSourcePosition;
    }m_RenderObj;

    EGLDisplay mEglDisplay;
    ANativeWindow * mWin;
    EGLSurface mEglSurface;


    int m_width;
    int m_height;
    GLuint m_programObject;
    GLint  m_positionLoc;
    GLint  m_texCoordLoc;

     // Sampler location
    GLint m_samplerLoc;
    GLint m_samplerLocBase;
    GLuint m_textureId;
    GLuint m_textureIdBack;
    bool m_flag;
    
};


}

#endif