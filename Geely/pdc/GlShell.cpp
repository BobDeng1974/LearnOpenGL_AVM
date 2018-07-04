#include "GlShell.h"

using namespace android;

GlShell::GlShell()
{
    PDCL_LOG_INFO("GlShell :: GlShell");
}


GlShell::~GlShell()
{


}

void GlShell::InitView(ANativeWindow *surface, int screenWidth, int screenHeight)
{
    PDCL_LOG_INFO("GlShell | InitView");
    mWin = surface;
    // m_width = 152;
    // m_height = 228;
    // m_textureId = 0;
    eglInit();
    // init();
    LoadShaders();
    glGenTextures(1, &m_uiTexture);
    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    glViewport(0, 0,screenWidth, screenHeight);
    //to make surface working??
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    PDCL_LOG_INFO("GlShell | InitView end");
}

int GlShell::eglInit() {
   
    /****************************************************************/
    // Step 1 - Get the default display.

    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(EGL_NO_DISPLAY == mEglDisplay) {
        PDCL_LOG_INFO("The default display is wrong!");
        return -1;
    } else{
        PDCL_LOG_INFO("Get the default display!");
    }

    /****************************************************************/
    //Step 2 - Initialize EGL.
    GLint majorVersion;
    GLint minorVersion;

    if(!eglInitialize(mEglDisplay, &majorVersion, &minorVersion)) {
        PDCL_LOG_INFO("Initialize EGL is ERROR");
        return -1;
    } else {
        PDCL_LOG_INFO("Initialize EGL is OK");
    }

    /**************************************************************/
    //Step 3 - Make OpenGL ES the current API.

    /*******************************************************/
    //Step 4 - Specify the required configuration attributes.
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_ALPHA_SIZE, 1,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    /***********************************************************/
    //Step 5 - Find a config that matches all requirements.
    int num_configs = 0;
    EGLConfig   eglConfig;
    if(!eglChooseConfig(mEglDisplay, config_attribs, &eglConfig, 1, &num_configs))
    {
        PDCL_LOG_INFO("Can't find a config that matches all requirements");
        return -1;
    } else{
        PDCL_LOG_INFO("Find a config that matches all requirements");
    }

    /*************************************************************/
    //Step 6 - Create a surface to draw to.
    mEglSurface = eglCreateWindowSurface(mEglDisplay, eglConfig, mWin, NULL);
    if(EGL_NO_SURFACE == mEglSurface) {
        PDCL_LOG_INFO("Can't Create a surface");
        return -1;
    } else{
        PDCL_LOG_INFO("Create a pdc window surface to draw to.");
    }

    /* if(!eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &mSurfaceWidth) ||
        !eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &mSurfaceHeight)) {
         return -1;
     }*/

    /********************************************************************/
    //Step 7 -Create a context.
    EGLint context_attrib[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext mEglContext;
    mEglContext = eglCreateContext(mEglDisplay, eglConfig, EGL_NO_CONTEXT, context_attrib);
    if(EGL_NO_CONTEXT == mEglContext) {
        PDCL_LOG_INFO("Can't create context!");
        return -1;
    } else{
        PDCL_LOG_INFO("Create Context!");
    }

    /*************************************************************************/
    //Step 8 - Bind the context to the current thread and use our  window surface for drawing and reading.
    if(!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        return -1;
    } else {
        PDCL_LOG_INFO(" Bind the context!");
    }
    return 0;
}

const char* pAntiAliasedLineVertShader2 = "\
                                         attribute mediump vec4 myVert;\
                                         attribute mediump vec4 myUV;\
                                         uniform mediump vec2 imgSize;\
                                         varying mediump vec2 texCoord;\
                                         void main()\
                                         {\
                                         float x = 2.0*myVert.x/imgSize.x-1.0;\
                                         float y = 2.0 * myVert.y/imgSize.y - 1.0;\
                                         gl_Position = vec4(x,y,0.0,1.0);\
                                         texCoord = myUV.st;\
                                         }";

const char* pAntiAliasedLineFragShader2 = "\
                                         uniform lowp sampler2D tex;\
                                         varying mediump vec2 texCoord;\
                                         void main()\
                                         {\
                                         gl_FragColor = texture2D(tex, texCoord);\
                                         }";

void GlShell::LoadShaders()
{
    GLuint pid = glCreateProgram();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glAttachShader(pid, vertex_shader);
    glShaderSource(vertex_shader, 1, (const char**)&pAntiAliasedLineVertShader2, NULL);
    glCompileShader(vertex_shader);
    glDeleteShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glAttachShader(pid, fragment_shader);
    glShaderSource(fragment_shader, 1, (const char**)&pAntiAliasedLineFragShader2, NULL);
    glCompileShader(fragment_shader);
    glDeleteShader(fragment_shader);
    glBindAttribLocation(pid, 0, "myVert");
    glBindAttribLocation(pid, 1, "myUV");

    glLinkProgram(pid);
    glUseProgram(pid);
    m_RenderObj.uiId = pid;
    m_RenderObj.uiImgSize = glGetUniformLocation(m_RenderObj.uiId,"imgSize");
    glUniform1i(glGetUniformLocation(m_RenderObj.uiId,"tex"),0);
}

#if 0
void GlShell::generateTextures(void *databuffer, bool flag)
{
   // Texture object handle
   // printf("OpeningPainterImpl | GenerateTextures() textureId:%d\n", m_textureId);
  // m_width = width;
  // m_height = height ; 
   if (0 != m_textureId) {
       // stopDraw();
   }
   if (databuffer == NULL) {
       printf("Error loading image.\n");
       return;
   }
   GLuint textureId;
   // Generate a texture object
   glGenTextures(1, &textureId);
   // glActiveTexture(GL_TEXTURE0);
   // Bind the texture object
   glBindTexture(GL_TEXTURE_2D, textureId);
   printf("OpeningPainterImpl | GenerateTextures() m_width:%d,,,,,,,m_height:%d\n", m_width, m_height);
   if (databuffer == NULL) {
    return;
   }
   // Load the texture
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, databuffer);

   // printf("OpeningPainterImpl | GenerateTextures() textureId:%d\n", m_textureId);
   // Set the filtering mode
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);

   if (0 == textureId) {
       return;
   }
   if (flag) {
    m_flag = flag;
    m_textureIdBack = textureId;
   }
   else {
    m_textureId = textureId;
   }
   printf("OpeningPainterImpl | GenerateTextures() textureId:%d\n", m_textureId);
   glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
}

bool GlShell::drawToSurface(bool animationstatus, int frameNo)
{
    GLfloat vVertices[] = { -1.0f,  -1.0f, 0.0f,  // Position 0
                               0.0f,  0.0f,        // TexCoord 0
                              1.0f, -1.0f, 0.0f,  // Position 1
                               1.0f,  0.0f,        // TexCoord 1
                               -1.0f, 1.0f, 0.0f,  // Position 2
                               0.0f,  1.0f,        // TexCoord 2
                               1.0f,  1.0f, 0.0f,  // Position 3
                               1.0f,  1.0f         // TexCoord 3
                            };
      GLushort indices[] = { 0, 1, 2, 1, 3, 2 };

    glViewport (0, 0, m_width, m_height);
    glClear (GL_COLOR_BUFFER_BIT);
//     glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // Use the program object
        glUseProgram (m_programObject);
        // Load the vertex position
        glVertexAttribPointer (m_positionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);
        // Load the texture coordinate
        glVertexAttribPointer (m_texCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

        glEnableVertexAttribArray (m_positionLoc);
        glEnableVertexAttribArray (m_texCoordLoc);
        float alpha = 1.0f;
        if (animationstatus) {
            alpha = frameNo / 20.0f;
        }
        glUniform1f(glGetUniformLocation(m_programObject, "Alpha"), alpha); 
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // glAlphaFunc(GL_GREATER, 0.5f);
        // Bind the texture
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, m_textureId);
       glUniform1i(m_samplerLoc, 0);

       // glActiveTexture(GL_TEXTURE1);
       // glBindTexture(GL_TEXTURE_2D, m_textureIdBack);
       // glUniform1i(m_samplerLocBase, 1);


        // Set the sampler texture unit to 0
//        glUniform1i ( m_samplerLoc, 0 );
//        float glMatrix[16];
//        glUniformMatrix4fv(m_texCoordLoc, 1, false, &glMatrix[0]);

        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

        return true;
}

GLuint  GlShell::LoadShader(GLenum type, const char *shaderSrc)
{
    printf("OpeningPainterImpl | LoadShader()\n");
    GLuint shader;
    GLint compiled;
    shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }
    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);
    // Compile the shader
    glCompileShader(shader);
    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = static_cast<char*>(malloc(sizeof(char) * infoLen));
            if (infoLog  != NULL) {
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                printf("Error compiling shader: %s\n", infoLog);
                free(infoLog);
             }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int GlShell::init()
{
    printf("OpeningPainterImpl | init()\n");
    GLbyte vShaderStr[] =
            "attribute vec4 a_position; \n"
            "attribute vec2 a_texCoord; \n"
            "varying vec2 v_texCoord; \n"
            "void main() \n"
            "{ \n"
            " gl_Position = a_position; \n"
            " v_texCoord = a_texCoord; \n"
            "} \n";
    GLbyte fShaderStr[] =
            "precision mediump float;                            \n"
                 "varying vec2 v_texCoord;                            \n"
                 "uniform sampler2D s_baseMap;                        \n"
                 "uniform sampler2D s_lightMap;                    \n"
                 "uniform mediump float Alpha;                    \n"
                 "void main()                                         \n"
                 "{                                                   \n"
                 "  vec4 baseColor;                                   \n"
                 "  vec4 lightColor;                                  \n"
                 "                                                    \n"
                 "  baseColor = texture2D( s_baseMap, v_texCoord );   \n"
                 "  lightColor = texture2D( s_lightMap, v_texCoord ); \n"
                 // "  gl_FragColor =  baseColor * (lightColor + Alpha);   \n"
                 "  gl_FragColor =  vec4(baseColor.rgba);  \n "
                 "}                                                   \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;
    vertexShader = LoadShader(GL_VERTEX_SHADER, reinterpret_cast<const char *>(vShaderStr));
    if (vertexShader == 0) {
        return 0;
    }
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, reinterpret_cast<const char *>(fShaderStr));
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return 0;
    }
    // Create the program object
    programObject = glCreateProgram();
    if (programObject == 0) {
        return 0;
    }
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = static_cast<char*>(malloc(sizeof(char) * infoLen));
            if (NULL != infoLog) {
                glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
                printf("Error linking program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(programObject);
        return false;
    }
    // Bind vPosition to attribute 0
    m_positionLoc = glGetAttribLocation(programObject, "a_position");
    m_texCoordLoc = glGetAttribLocation(programObject, "a_texCoord");

    m_samplerLoc = glGetUniformLocation(programObject, "s_baseMap");
    // m_samplerLocBase = glGetUniformLocation(programObject, "s_lightMap");

    m_programObject = programObject;
    return true;
}

#endif


