#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <list>
#include <map>

#include <unistd.h>
#include <random>

#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

const float PI = 3.14159265359;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, unsigned int VAO, unsigned int VBO);

const unsigned int SCREEN_WIDTH = 2080;
const unsigned int SCREEN_HEIGHT = 720;

const unsigned int TOP_VIEW_WIDTH = 640;
const unsigned int TOP_VIEW_HEIGHT = 720;

const unsigned int FREE_VIEW_WIDTH = 1280;
const unsigned int FREE_VIEW_HEIGHT = 720;

const unsigned int NAVIGATION_BAR_VIEW_WIDTH = 160;
const unsigned int NAVIGATION_BAR_VIEW_HEIGHT = 720;

bool DOOR_L1 = false;
bool DOOR_L2 = false;
bool DOOR_R1 = false;
bool DOOR_R2 = false;
bool DOOR_B = false;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

unsigned int currentCursorX;
unsigned int currentCursorY;

int cameraUpLeft = 0;
int cameraUp = 0;
int cameraUpRight = 0;

int cameraDownLeft = 0;
int cameraDown = 0;
int cameraDownRight = 0;

int cameraLeft = 0;
int cameraRight = 0;

int cameraLeftUp2Down = 0;
int cameraRightUp2Down = 0;

int cameraLeftDown2Up = 0;
int cameraRightDown2Up = 0;

int navigationOne = 0;
int navigationTwo = 0;
int navigationThree = 0;
int navigationFour = 0;
int navigationFive = 0;
int navigationSix = 0;
int navigationSeven = 0;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 30.0f));
//bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float radius = 700.0f;//30.0f;
float Yaw = 90.0f;
float Pitch = 45.0f;

float _distance = 250.0f;
float r = 1.0f;
float g = 0.0f;

std::list<float> wheelAngleArray;

glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);

int RectangleNum = 0;

int maxAngleRectangleNum = 0;

std::vector<float> vertexArray;

int areaRectangleNum = 0;

std::vector<float> areaVertexArray;

float caculateX(float x0, float z0, float z, float radius, float startX, float _angle)
{
    float x = pow(pow(radius, 2) - pow(z - z0, 2), 0.5);

    if(_angle >= -45.0f && _angle <= 45.0f){
        if(x + x0 >= startX-1.0f)  {
            x = x + x0;
        }else if(-x + x0 >= startX-1.0f){
            x = -x + x0;
        }else{
            assert(0);
        }
    }else if(_angle >= 135.0f && _angle <= 225.0f){
        if(x + x0 <= startX+1.0f)  {
            x = x + x0;
        }else if(-x + x0 <= startX+1.0f){
            x = -x + x0;
        }else{
            assert(0);
        }
    }else{
        assert(0);
    }

    return x;
}

float caculateZ(float x0, float z0, float x, float radius)
{
    float z = pow(pow(radius, 2) - pow(x - x0, 2), 0.5);

    if(abs(z + z0) <  abs(z0)) {
        z = z + z0;
    }else if(abs(-z + z0) <  abs(z0)){
        z = -z + z0;
    }else{
        assert(0);
    }
    return z;
}

float AppendVertexArray(float beginX, float endX,
                        float endZ,
                        float _radius,
                        float _angle,
                        std::list<float>& arrayX, std::list<float>& arrayZ)
{
    //assert(beginX < endX);

    //yuan xin
    float x0 = -134.0f;
    float z0;
    if(_angle >= -45.0f && _angle <= 45.0f){
        z0 = 265.0f / (sin(glm::radians(float(_angle))) / cos(glm::radians(float(_angle))));
    }else if(_angle >= 135.0f && _angle <= 225.0f){
        z0 = 265.0f / (sin(glm::radians(float(_angle-180))) / cos(glm::radians(float(_angle-180))));
    }else{
        assert(0);
    }


    if(pow(_radius, 2) - pow(beginX - x0, 2) <= 0.0f) {
        assert(0);
    }

    float beginZ = pow(pow(_radius, 2) - pow(beginX - x0, 2), 0.5);
    if(abs(beginZ + z0) <  abs(z0)) {
        beginZ = beginZ + z0;
    }else if(abs(-beginZ + z0) <  abs(z0)){
        beginZ = -beginZ + z0;
    }else{
        assert(0);
    }

    float lengthZ = abs(endZ - beginZ);


    float _endX = pow(pow(_radius, 2) - pow(endZ - z0, 2), 0.5);

    if(_angle >= -45.0f && _angle <= 45.0f){
        if(_endX + x0 >= beginX-1.0f)  {
            _endX = _endX + x0;
        }else if(-_endX + x0 >= beginX-1.0f){
            _endX = -_endX + x0;
        }else{
            assert(0);
        }

        _endX = min(_endX, endX);
    }else if(_angle >= 135.0f && _angle <= 225.0f){
        if(_endX + x0 <= beginX+1.0f)  {
            _endX = _endX + x0;
        }else if(-_endX + x0 <= beginX+1.0f){
            _endX = -_endX + x0;
        }else{
            assert(0);
        }

        _endX = max(_endX, endX);
    }else{
        assert(0);
    }

    float lengthX = abs(_endX - beginX);

    if(lengthZ <=  lengthX){
        //an zhao X fen 100 fen

        float xStart = beginX;
        float xEnd = _endX;
        float zStart = beginZ;
        float zEnd = endZ;

        int num = 100;

        float dif = (xEnd - xStart)/num;
        for(int i = 0; i < num; i++){
            float x = xStart + i * dif;
            float z = caculateZ(x0, z0, x, _radius);
            arrayX.push_back(x);
            arrayZ.push_back(z);
            //std::cout << "lengthZ <=  lengthX ==> x = " << x << " z = " << z << std::endl;
        }
    }else{
        //an zhao Z fen 100 fen

        float xStart = beginX;
        float xEnd = _endX;
        float zStart = beginZ;
        float zEnd = endZ;

        int num = 100;

        float dif = (zEnd - zStart)/num;
        for(int i = 0; i < num; i++){
            float z = zStart + i * dif;
            float x = caculateX(x0, z0, z, _radius, xStart, _angle);
            if(x > endX) return zStart + (i-1) * dif;
            arrayX.push_back(x);
            arrayZ.push_back(z);
            //std::cout << "lengthZ >  lengthX ==> x = " << x << " z = " << z << std::endl;
        }
    }


    //assert(0);

    return 0.0f;
}

float AppendVertexArrayEx(float beginX, float endX,
                        float _radius,
                        float _angle,
                        std::list<float>& arrayX, std::list<float>& arrayZ)
{
    float x0 = -134.0f;
    float z0;
    if(_angle >= -45.0f && _angle <= 45.0f){
        z0 = 265.0f / (sin(glm::radians(float(_angle))) / cos(glm::radians(float(_angle))));
    }else if(_angle >= 135.0f && _angle <= 225.0f){
        z0 = 265.0f / (sin(glm::radians(float(_angle-180))) / cos(glm::radians(float(_angle-180))));
    }else{
        assert(0);
    }

    float xStart = beginX;
    float xEnd = endX;

    int num = 100;

    float dif = (xEnd - xStart)/num;
    for(int i = 0; i < num; i++){
        float x = xStart + i * dif;
        float z = caculateZ(x0, z0, x, _radius);
        arrayX.push_back(x);
        arrayZ.push_back(z);
    }
}

void generateLineVertexArray(std::vector<float>& array,
                            std::list<float>& XList,
                            std::list<float>& ZList,
                            float _angle)
{
    assert(XList.size() == ZList.size());

    int size = XList.size();

    for(int i = 0; i < size; i++)
    {
        array.push_back(XList.front());
        array.push_back(0.1f);
        array.push_back(ZList.front());
        array.push_back(_angle);
        array.push_back(0.8f);

        XList.pop_front();
        ZList.pop_front();
    }
}

void AppendRectangle(std::vector<float>& array,
                    std::list<float>& minX, std::list<float>& minZ,
                    std::list<float>& MaxX, std::list<float>& MaxZ,
                    float _angle,
                    int& num)
{
    float alpha = 1.0f/minX.size();
    int _RectangleNum = minX.size() - 1;

    for(int i = 0; i < _RectangleNum; i++){
        float minXone = minX.front();
        float minZone = minZ.front();

        float maxXone = MaxX.front();
        float maxZone = MaxZ.front();

        minX.pop_front();
        minZ.pop_front();
        MaxX.pop_front();
        MaxZ.pop_front();

        float minXtwo = minX.front();
        float minZtwo = minZ.front();

        float maxXtwo = MaxX.front();
        float maxZtwo = MaxZ.front();

        minX.push_back(minXone);
        minZ.push_back(minZone);
        MaxX.push_back(maxXone);
        MaxZ.push_back(maxZone);

        array.push_back(minXone);
        array.push_back(0.1f);
        array.push_back(minZone);
        array.push_back(_angle);
        array.push_back(1.0f - alpha*i);

        array.push_back(maxXone);
        array.push_back(0.1f);
        array.push_back(maxZone);
        array.push_back(_angle);
        array.push_back(1.0f - alpha*i);

        array.push_back(minXtwo);
        array.push_back(0.1f);
        array.push_back(minZtwo);
        array.push_back(_angle);
        array.push_back(1.0f - alpha*i);

        array.push_back(maxXone);
        array.push_back(0.1f);
        array.push_back(maxZone);
        array.push_back(_angle);
        array.push_back(1.0f - alpha*i);

        array.push_back(minXtwo);
        array.push_back(0.1f);
        array.push_back(minZtwo);
        array.push_back(_angle);
        array.push_back(1.0f - alpha*i);

        array.push_back(maxXtwo);
        array.push_back(0.1f);
        array.push_back(maxZtwo);
        array.push_back(_angle);
        array.push_back(1.0f - alpha*i);

        num++;
    }

    float lastminX = minX.front();
    float lastminZ = minZ.front();
    float lastMaxX = MaxX.front();
    float lastMaxZ = MaxZ.front();

    minX.pop_front();
    minZ.pop_front();
    MaxX.pop_front();
    MaxZ.pop_front();

    minX.push_back(lastminX);
    minZ.push_back(lastminZ);
    MaxX.push_back(lastMaxX);
    MaxZ.push_back(lastMaxZ);
}

void generateAreaVertexArrayWithAngle0(std::vector<float>& vertexArray, std::vector<float>& areaVertexArray,
                                        float startx, float endx,
                                        float leftLeftZ, float leftRightZ,
                                        float rightLeftZ, float rightRightZ,
                                        float _angle)
{
    int num = 100;
    float dif = (endx - startx)/num;


    std::list<float> frontLeftLeftX;
    for(int i = 0; i < num; i++){
        frontLeftLeftX.push_back(startx + i * dif);
    }

    std::list<float> frontLeftLeftZ;
    for(int i = 0; i < num; i++){
        frontLeftLeftZ.push_back(leftLeftZ);
    }

    std::list<float> frontLeftRightX;
    for(int i = 0; i < num; i++){
        frontLeftRightX.push_back(startx + i * dif);
    }

    std::list<float> frontLeftRightZ;
    for(int i = 0; i < num; i++){
        frontLeftRightZ.push_back(leftRightZ);
    }


    AppendRectangle(vertexArray,
                    frontLeftLeftX, frontLeftLeftZ,
                    frontLeftRightX, frontLeftRightZ,
                    _angle,
                    RectangleNum);
//--------------------------------------------------------------
    std::list<float> frontRightLeftX;
    for(int i = 0; i < num; i++){
        frontRightLeftX.push_back(startx + i * dif);
    }

    std::list<float> frontRightLeftZ;
    for(int i = 0; i < num; i++){
        frontRightLeftZ.push_back(rightLeftZ);
    }

    std::list<float> frontRightRightX;
    for(int i = 0; i < num; i++){
        frontRightRightX.push_back(startx + i * dif);
    }

    std::list<float> frontRightRightZ;
    for(int i = 0; i < num; i++){
        frontRightRightZ.push_back(rightRightZ);
    }

    AppendRectangle(vertexArray,
                    frontRightLeftX, frontRightLeftZ,
                    frontRightRightX, frontRightRightZ,
                    _angle,
                    RectangleNum);
//--------------------------------------------------------------


    AppendRectangle(areaVertexArray,
                    frontLeftRightX, frontLeftRightZ,
                    frontRightLeftX, frontRightLeftZ,
                    _angle,
                    RectangleNum);
}

void generateAreaVertexArray(std::vector<float>& vertexArray, std::vector<float>& areaVertexArray,
                            std::list<float>& _frontLeftMinX, std::list<float>& _frontLeftMinZ,
                            std::list<float>& _frontLeftMaxX, std::list<float>& _frontLeftMaxZ,
                            std::list<float>& _frontRightMinX, std::list<float>& _frontRightMinZ,
                            std::list<float>& _frontRightMaxX, std::list<float>& _frontRightMaxZ,
                            std::list<float>& _backMinX, std::list<float>& _backMinZ,
                            std::list<float>& _backMaxX, std::list<float>& _backMaxZ,
                            float _angle)
{
    //std::cout << " _angle = " << _angle << " _frontLeftMinX.size() = " << _frontLeftMinX.size() << " _frontLeftMaxX.size() = " << _frontLeftMaxX.size() << std::endl;

    assert(_frontLeftMinX.size() == _frontLeftMinZ.size());
    assert(_frontLeftMaxX.size() == _frontLeftMaxZ.size());
    assert(_frontRightMinX.size() == _frontRightMinZ.size());
    assert(_frontRightMaxX.size() == _frontRightMaxZ.size());
    assert(_backMinX.size() == _backMinZ.size());
    assert(_backMaxX.size() == _backMaxZ.size());

    assert(_frontLeftMinX.size() == _frontLeftMaxX.size());
    assert(_frontRightMinX.size() == _frontRightMaxX.size());
    assert(_backMinX.size() == _backMaxX.size());

    assert(_frontLeftMinX.size() == _frontRightMaxX.size());
    assert(_frontLeftMaxX.size() == _frontRightMinX.size());

    AppendRectangle(vertexArray,
                _frontLeftMinX,  _frontLeftMinZ,
                _frontLeftMaxX,  _frontLeftMaxZ,
                _angle, RectangleNum);

    AppendRectangle(vertexArray,
                _frontRightMinX,  _frontRightMinZ,
                _frontRightMaxX,  _frontRightMaxZ,
                _angle, RectangleNum);

    AppendRectangle(vertexArray,
                _backMinX,  _backMinZ,
                _backMaxX,  _backMaxZ,
                _angle, RectangleNum);

    assert(_frontLeftMinX.size() == _frontLeftMinZ.size());
    assert(_frontLeftMaxX.size() == _frontLeftMaxZ.size());
    assert(_frontRightMinX.size() == _frontRightMinZ.size());
    assert(_frontRightMaxX.size() == _frontRightMaxZ.size());
    assert(_backMinX.size() == _backMinZ.size());
    assert(_backMaxX.size() == _backMaxZ.size());

    assert(_frontLeftMinX.size() == _frontLeftMaxX.size());
    assert(_frontRightMinX.size() == _frontRightMaxX.size());
    assert(_backMinX.size() == _backMaxX.size());

    assert(_frontLeftMinX.size() == _frontRightMaxX.size());
    assert(_frontLeftMaxX.size() == _frontRightMinX.size());

    if(_angle == 0.0f){
        assert(0);
    }else if(_angle > 0.0f){
        AppendRectangle(areaVertexArray,
                    _frontLeftMinX,  _frontLeftMinZ,
                    _frontRightMaxX,  _frontRightMaxZ,
                    _angle, areaRectangleNum);
    }else if(_angle < 0.0f){
        AppendRectangle(areaVertexArray,
                    _frontLeftMaxX,  _frontLeftMaxZ,
                    _frontRightMinX,  _frontRightMinZ,
                    _angle, areaRectangleNum);
    }else{
        assert(0);
    }

    //std::cout << " areaVertexArray.size() = " << areaVertexArray.size()  << std::endl;
}

void generateAreaVertexArrayRear(std::vector<float>& vertexArray, std::vector<float>& areaVertexArray,
                            std::list<float>& _frontMinX, std::list<float>& _frontMinZ,
                            std::list<float>& _frontMaxX, std::list<float>& _frontMaxZ,
                            std::list<float>& _backLeftMinX, std::list<float>& _backLeftMinZ,
                            std::list<float>& _backLeftMaxX, std::list<float>& _backLeftMaxZ,
                            std::list<float>& _backRightMinX, std::list<float>& _backRightMinZ,
                            std::list<float>& _backRightMaxX, std::list<float>& _backRightMaxZ,
                            float _angle)
{
    //std::cout << " _angle = " << _angle << " _frontLeftMinX.size() = " << _frontLeftMinX.size() << " _frontLeftMaxX.size() = " << _frontLeftMaxX.size() << std::endl;

    assert(_frontMinX.size() == _frontMinZ.size());
    assert(_frontMaxX.size() == _frontMaxZ.size());
    assert(_backLeftMinX.size() == _backLeftMinZ.size());
    assert(_backLeftMaxX.size() == _backLeftMaxZ.size());
    assert(_backRightMinX.size() == _backRightMinZ.size());
    assert(_backRightMaxX.size() == _backRightMaxZ.size());

    assert(_frontMinX.size() == _frontMaxX.size());
    assert(_backLeftMinX.size() == _backLeftMaxX.size());
    assert(_backRightMinX.size() == _backRightMaxX.size());

    assert(_backLeftMinX.size() == _backRightMaxX.size());
    assert(_backRightMinX.size() == _backLeftMaxX.size());

    AppendRectangle(vertexArray,
                _frontMinX,  _frontMinZ,
                _frontMaxX,  _frontMaxZ,
                _angle, RectangleNum);

    AppendRectangle(vertexArray,
                _backLeftMinX,  _backLeftMinZ,
                _backLeftMaxX,  _backLeftMaxZ,
                _angle, RectangleNum);

    AppendRectangle(vertexArray,
                _backRightMinX,  _backRightMinZ,
                _backRightMaxX,  _backRightMaxZ,
                _angle, RectangleNum);

    assert(_frontMinX.size() == _frontMinZ.size());
    assert(_frontMaxX.size() == _frontMaxZ.size());
    assert(_backLeftMinX.size() == _backLeftMinZ.size());
    assert(_backLeftMaxX.size() == _backLeftMaxZ.size());
    assert(_backRightMinX.size() == _backRightMinZ.size());
    assert(_backRightMaxX.size() == _backRightMaxZ.size());

    assert(_frontMinX.size() == _frontMaxX.size());
    assert(_backLeftMinX.size() == _backLeftMaxX.size());
    assert(_backRightMinX.size() == _backRightMaxX.size());

    assert(_backLeftMinX.size() == _backRightMaxX.size());
    assert(_backRightMinX.size() == _backLeftMaxX.size());

    if(_angle == 180.0f){
        assert(0);
    }else if(_angle > 180.0f){
        AppendRectangle(areaVertexArray,
                    _backLeftMinX,  _backLeftMinZ,
                    _backRightMaxX,  _backRightMaxZ,
                    _angle, areaRectangleNum);
    }else if(_angle < 180.0f){
        AppendRectangle(areaVertexArray,
                    _backLeftMaxX,  _backLeftMaxZ,
                    _backRightMinX,  _backRightMinZ,
                    _angle, areaRectangleNum);
    }else{
        assert(0);
    }

    //std::cout << " areaVertexArray.size() = " << areaVertexArray.size()  << std::endl;
}

void generateMaxAngleLineVertexArray(std::vector<float>& maxAngleLineVertexArray,
                                    std::list<float>& _frontLeftMinX, std::list<float>& _frontLeftMinZ,
                                    std::list<float>& _frontLeftMaxX, std::list<float>& _frontLeftMaxZ,
                                    float _angle)
{
    assert(_frontLeftMinX.size() == _frontLeftMinZ.size());
    assert(_frontLeftMaxX.size() == _frontLeftMaxZ.size());

    assert(_frontLeftMinX.size() == _frontLeftMaxX.size());


    AppendRectangle(maxAngleLineVertexArray,
                _frontLeftMinX,  _frontLeftMinZ,
                _frontLeftMaxX,  _frontLeftMaxZ,
                _angle, maxAngleRectangleNum);

    assert(_frontLeftMinX.size() == _frontLeftMinZ.size());
    assert(_frontLeftMaxX.size() == _frontLeftMaxZ.size());

    assert(_frontLeftMinX.size() == _frontLeftMaxX.size());
}

void TestPasFunLine()
{
    while (1) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // wheelAngle += 1.0f;

        // if(wheelAngle == 0.0f) wheelAngle += 1.0f;

        // if(wheelAngle > 45.0f){
        //     wheelAngle = -45.0f;
        // }

        // wheelAngle -= 1.0f;

        // if(wheelAngle == 1.0f) wheelAngle = 45.0f;
    }
}


void TestPasFun()
{
    while (1) {
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // _distance += 20.0f;
        // r -= 0.1f;
        // g += 0.1f;

        // if (_distance > 450.0f) {
        //     _distance = 250.0f;
        //     r = 1.0f;
        //     g = 0.0f;
        // }
    }
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//-----------------------

int main()
{
    thread TestPASThread = thread{TestPasFun};

    //thread TestPASThread2 = thread{TestPasFunLine};

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // -------------------------------------glEnable-----------------------------------begin
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -------------------------------------glEnable-----------------------------------end

    // ------------------------------load car obj--------------------------------------begin
    Model ourModel(FileSystem::getPath("resources/objects/nanosuit/wey_vv6.obj"));
    //Model ourModel(FileSystem::getPath("resources/objects/nanosuit/VV6.obj"));
    //Model ourModel(FileSystem::getPath("resources/objects/nanosuit/pas.obj"));
    // ------------------------------load car obj--------------------------------------end

    // ------------------------------build and compile shaders------------------------begin
    Shader carShader("car.vs", "car.fs");
    Shader radarShader("radar.vs", "radar.fs");
    Shader groundShader("ground.vs", "ground.fs");
    //Shader dynimicLineShader("dynimic_line.vs", "dynimic_line.fs");
    Shader shaderProgram("dynimic_line.vs", "dynimic_line.fs");
    //Shader shaderProgramArea("dynimic_line_area.vs", "dynimic_line_area.fs");
    Shader shaderProgramMaxAngle("dynimic_line_max_angle.vs", "dynimic_line_max_angle.fs");
    Shader shaderCamera("3.1.blending.vs", "3.1.blending.fs");
    Shader shaderTest("test.vs", "test.fs");
    Shader shader("text.vs", "text.fs");
    // ------------------------------build and compile shaders------------------------end

    std::vector<float> maxAngleLineVertexArray;

    for(int angle = -45; angle <= 45; angle++){
        wheelAngleArray.push_back((float)angle);
    }

    for(int angle = 135; angle <= 225; angle++){
        wheelAngleArray.push_back((float)angle);
    }

    assert(wheelAngleArray.size() == 182);

    for(int angle = 45; angle >= -45; angle--)
    {
        if(angle == 0) {
            generateAreaVertexArrayWithAngle0(vertexArray, areaVertexArray,
                                            131.0f, 500.0f,
                                            -91.0f, -87.0f,
                                            87.0f,  91.0f,
                                            (float)angle);
        }

        if(angle > 0) {
            //-----------front left------------------------
            float radius1 = pow(pow((265.0f / (sin(glm::radians(float(angle))) / cos(glm::radians(float(angle)))) + 89), 2) + pow(265, 2), 0.5);
            float minR = radius1-2.0f;
            float maxR = radius1+2.0f;

            std::list<float> frontLeftMinX, frontLeftMinZ;
            float result;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, 265.0f, minR, (float)angle, frontLeftMinX, frontLeftMinZ))){
                frontLeftMinX.clear();
                frontLeftMinZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, minR, (float)angle, frontLeftMinX, frontLeftMinZ))){
                    assert(0);
                }
            }

            std::list<float> frontLeftMaxX, frontLeftMaxZ;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, 265.0f, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ))){
                frontLeftMaxX.clear();
                frontLeftMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ))){
                    assert(0);
                }
            }


            //-----------front right------------------------
            float radius2 = pow(pow((265.0f / (sin(glm::radians(float(angle))) / cos(glm::radians(float(angle)))) - 89), 2) + pow(265, 2), 0.5);
            minR = radius2-2.0f;
            maxR = radius2+2.0f;

            std::list<float> frontRightMinX, frontRightMinZ;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, 265.0f, minR, (float)angle, frontRightMinX, frontRightMinZ))){
                frontRightMinX.clear();
                frontRightMinZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, minR, (float)angle, frontRightMinX, frontRightMinZ))){
                    assert(0);
                }
            }

            std::list<float> frontRightMaxX, frontRightMaxZ;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, 265.0f, maxR, (float)angle, frontRightMaxX, frontRightMaxZ))){
                frontRightMaxX.clear();
                frontRightMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, maxR, (float)angle, frontRightMaxX, frontRightMaxZ))){
                    assert(0);
                }
            }

            //-----------back right------------------------
            float radius3 = 265.0f / (sin(glm::radians(float(angle))) / cos(glm::radians(float(angle)))) - 89.0f;
            minR = radius3-2.0f;
            maxR = radius3+2.0f;

            std::list<float> backRightMinX, backRightMinZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, 265.0f, minR, (float)angle, backRightMinX, backRightMinZ))){
                backRightMinX.clear();
                backRightMinZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, result, minR, (float)angle, backRightMinX, backRightMinZ))){
                    assert(0);
                }
            }

            std::list<float> backRightMaxX, backRightMaxZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, 265.0f, maxR, (float)angle, backRightMaxX, backRightMaxZ))){
                backRightMaxX.clear();
                backRightMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, result, maxR, (float)angle, backRightMaxX, backRightMaxZ))){
                    assert(0);
                }
            }

            generateAreaVertexArray(vertexArray, areaVertexArray,
                                    frontLeftMinX, frontLeftMinZ,
                                    frontLeftMaxX, frontLeftMaxZ,
                                    frontRightMinX, frontRightMinZ,
                                    frontRightMaxX, frontRightMaxZ,
                                    backRightMinX, backRightMinZ,
                                    backRightMaxX, backRightMaxZ,
                                    (float)angle);

            if(angle == 45){
                generateMaxAngleLineVertexArray(maxAngleLineVertexArray,
                                        frontLeftMinX, frontLeftMinZ,
                                        frontLeftMaxX, frontLeftMaxZ,
                                        (float)angle);
            }

            frontLeftMinX.clear();
            frontLeftMinZ.clear();
            frontLeftMaxX.clear();
            frontLeftMaxZ.clear();
            frontRightMinX.clear();
            frontRightMinZ.clear();
            frontRightMaxX.clear();
            frontRightMaxZ.clear();
            backRightMinX.clear();
            backRightMinZ.clear();
            backRightMaxX.clear();
            backRightMaxZ.clear();
        }


        if(angle < 0) {
            //-----------front left------------------------
            float radius1 = pow(pow((265.0f / (sin(glm::radians(float(-angle))) / cos(glm::radians(float(-angle)))) - 89), 2) + pow(265, 2), 0.5);
            float minR = radius1-2.0f;
            float maxR = radius1+2.0f;

            std::list<float> frontLeftMinX, frontLeftMinZ;
            float result;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, -265.0f, minR, (float)angle, frontLeftMinX, frontLeftMinZ))){
                frontLeftMinX.clear();
                frontLeftMinZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, minR, (float)angle, frontLeftMinX, frontLeftMinZ))){
                    assert(0);
                }
            }

            std::list<float> frontLeftMaxX, frontLeftMaxZ;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, -265.0f, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ))){
                frontLeftMaxX.clear();
                frontLeftMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ))){
                    assert(0);
                }
            }


            //-----------front right------------------------
            float radius2 = pow(pow((265.0f / (sin(glm::radians(float(-angle))) / cos(glm::radians(float(-angle)))) + 89), 2) + pow(265, 2), 0.5);
            minR = radius2-2.0f;
            maxR = radius2+2.0f;

            std::list<float> frontRightMinX, frontRightMinZ;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, -265.0f, minR, (float)angle, frontRightMinX, frontRightMinZ))){
                frontRightMinX.clear();
                frontRightMinZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, minR, (float)angle, frontRightMinX, frontRightMinZ))){
                    assert(0);
                }
            }

            std::list<float> frontRightMaxX, frontRightMaxZ;
            if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, -265.0f, maxR, (float)angle, frontRightMaxX, frontRightMaxZ))){
                frontRightMaxX.clear();
                frontRightMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(131.0f, 500.0f, result, maxR, (float)angle, frontRightMaxX, frontRightMaxZ))){
                    assert(0);
                }
            }

            //-----------back left------------------------
            float radius3 = 265.0f / (sin(glm::radians(float(-angle))) / cos(glm::radians(float(-angle)))) - 89.0f;
            minR = radius3-2.0f;
            maxR = radius3+2.0f;

            std::list<float> backLeftMinX, backLeftMinZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, -265.0f, minR, (float)angle, backLeftMinX, backLeftMinZ))){
                backLeftMinX.clear();
                backLeftMinZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, result, minR, (float)angle, backLeftMinX, backLeftMinZ))){
                    assert(0);
                }
            }

            std::list<float> backLeftMaxX, backLeftMaxZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, -265.0f, maxR, (float)angle, backLeftMaxX, backLeftMaxZ))){
                backLeftMaxX.clear();
                backLeftMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, 100.0f, result, maxR, (float)angle, backLeftMaxX, backLeftMaxZ))){
                    assert(0);
                }
            }

            generateAreaVertexArray(vertexArray, areaVertexArray,
                                    frontLeftMinX, frontLeftMinZ,
                                    frontLeftMaxX, frontLeftMaxZ,
                                    frontRightMinX, frontRightMinZ,
                                    frontRightMaxX, frontRightMaxZ,
                                    backLeftMinX, backLeftMinZ,
                                    backLeftMaxX, backLeftMaxZ,
                                    (float)angle);

            if(angle == -45){
                generateMaxAngleLineVertexArray(maxAngleLineVertexArray,
                                        frontRightMinX, frontRightMinZ,
                                        frontRightMaxX, frontRightMaxZ,
                                        (float)angle);
            }

            frontLeftMinX.clear();
            frontLeftMinZ.clear();
            frontLeftMaxX.clear();
            frontLeftMaxZ.clear();
            frontRightMinX.clear();
            frontRightMinZ.clear();
            frontRightMaxX.clear();
            frontRightMaxZ.clear();
            backLeftMinX.clear();
            backLeftMinZ.clear();
            backLeftMaxX.clear();
            backLeftMaxZ.clear();
        }
    }

    for(int angle = 225/*R : 45*/; angle >= 135/*R : -45*/; angle--)
    {
        if(angle == 180) {
            generateAreaVertexArrayWithAngle0(vertexArray, areaVertexArray,
                                            -134.0f, -500.0f,
                                            -91.0f, -87.0f,
                                            87.0f,  91.0f,
                                            (float)angle);
        }

        if(angle > 180) {
            //-----------front left------------------------
            float radius1 = pow(pow((265.0f / (sin(glm::radians(float((angle-180)))) / cos(glm::radians(float((angle-180))))) + 89), 2) + pow(265, 2), 0.5);
            float minR = radius1-2.0f;
            float maxR = radius1+2.0f;

            std::list<float> frontLeftMinX, frontLeftMinZ;
            AppendVertexArrayEx(131.0f, -130.0f, minR, (float)angle, frontLeftMinX, frontLeftMinZ);

            std::list<float> frontLeftMaxX, frontLeftMaxZ;
            AppendVertexArrayEx(131.0f, -130.0f, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ);

            //-----------back left------------------------
            float radius2 = 265.0f / (sin(glm::radians(float((angle-180)))) / cos(glm::radians(float((angle-180))))) + 89.0f;
            minR = radius2-2.0f;
            maxR = radius2+2.0f;

            float result;
            std::list<float> backLeftMinX, backLeftMinZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, 265.0f, minR, (float)angle, backLeftMinX, backLeftMinZ))){
                backLeftMinX.clear();
                backLeftMinZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, minR, (float)angle, backLeftMinX, backLeftMinZ))){
                    assert(0);
                }
            }

            std::list<float> backLeftMaxX, backLeftMaxZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, 265.0f, maxR, (float)angle, backLeftMaxX, backLeftMaxZ))){
                backLeftMaxX.clear();
                backLeftMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, maxR, (float)angle, backLeftMaxX, backLeftMaxZ))){
                    assert(0);
                }
            }

            //-----------back right------------------------
            float radius3 = 265.0f / (sin(glm::radians(float((angle-180)))) / cos(glm::radians(float((angle-180))))) - 89.0f;
            minR = radius3-2.0f;
            maxR = radius3+2.0f;

            std::list<float> backRightMinX, backRightMinZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, 265.0f, minR, (float)angle, backRightMinX, backRightMinZ))){
                backRightMinX.clear();
                backRightMinZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, minR, (float)angle, backRightMinX, backRightMinZ))){
                    assert(0);
                }
            }

            std::list<float> backRightMaxX, backRightMaxZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, 265.0f, maxR, (float)angle, backRightMaxX, backRightMaxZ))){
                backRightMaxX.clear();
                backRightMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, maxR, (float)angle, backRightMaxX, backRightMaxZ))){
                    assert(0);
                }
            }

            generateAreaVertexArrayRear(vertexArray, areaVertexArray,
                                    frontLeftMinX, frontLeftMinZ,
                                    frontLeftMaxX, frontLeftMaxZ,
                                    backLeftMinX, backLeftMinZ,
                                    backLeftMaxX, backLeftMaxZ,
                                    backRightMinX, backRightMinZ,
                                    backRightMaxX, backRightMaxZ,
                                    (float)angle);

            if(angle == 225){
                generateMaxAngleLineVertexArray(maxAngleLineVertexArray,
                                        backLeftMinX, backLeftMinZ,
                                        backLeftMaxX, backLeftMaxZ,
                                        (float)angle);
            }

            frontLeftMinX.clear();
            frontLeftMinZ.clear();
            frontLeftMaxX.clear();
            frontLeftMaxZ.clear();
            backLeftMinX.clear();
            backLeftMinZ.clear();
            backLeftMaxX.clear();
            backLeftMaxZ.clear();
            backRightMinX.clear();
            backRightMinZ.clear();
            backRightMaxX.clear();
            backRightMaxZ.clear();
        }


        if(angle < 180) {
            //-----------front right------------------------
            float radius1 = pow(pow((265.0f / (sin(glm::radians(float(abs(angle-180)))) / cos(glm::radians(float(abs(angle-180))))) + 89), 2) + pow(265, 2), 0.5);
            float minR = radius1-2.0f;
            float maxR = radius1+2.0f;

            float result;
            std::list<float> frontRightMinX, frontRightMinZ;
            AppendVertexArrayEx(131.0f, -130.0f, minR, (float)angle, frontRightMinX, frontRightMinZ);

            std::list<float> frontRightMaxX, frontRightMaxZ;
            AppendVertexArrayEx(131.0f, -130.0f, maxR, (float)angle, frontRightMaxX, frontRightMaxZ);

            //-----------back left------------------------
            float radius2 = 265.0f / (sin(glm::radians(float(abs(angle-180)))) / cos(glm::radians(float(abs(angle-180))))) - 89.0f;
            minR = radius2-2.0f;
            maxR = radius2+2.0f;

            std::list<float> backLeftMinX, backLeftMinZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, -265.0f, minR, (float)angle, backLeftMinX, backLeftMinZ))){
                backLeftMinX.clear();
                backLeftMinZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, minR, (float)angle, backLeftMinX, backLeftMinZ))){
                    assert(0);
                }
            }

            std::list<float> backLeftMaxX, backLeftMaxZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, -265.0f, maxR, (float)angle, backLeftMaxX, backLeftMaxZ))){
                backLeftMaxX.clear();
                backLeftMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, maxR, (float)angle, backLeftMaxX, backLeftMaxZ))){
                    assert(0);
                }
            }

            //-----------back right------------------------
            float radius3 = 265.0f / (sin(glm::radians(float(abs(angle-180)))) / cos(glm::radians(float(abs(angle-180))))) + 89.0f;
            minR = radius3-2.0f;
            maxR = radius3+2.0f;

            std::list<float> backRightMinX, backRightMinZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, -265.0f, minR, (float)angle, backRightMinX, backRightMinZ))){
                backRightMinX.clear();
                backRightMinZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, minR, (float)angle, backRightMinX, backRightMinZ))){
                    assert(0);
                }
            }

            std::list<float> backRightMaxX, backRightMaxZ;
            if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, -265.0f, maxR, (float)angle, backRightMaxX, backRightMaxZ))){
                backRightMaxX.clear();
                backRightMaxZ.clear();
                if(0.0f != (result = AppendVertexArray(-134.0f, -500.0f, result, maxR, (float)angle, backRightMaxX, backRightMaxZ))){
                    assert(0);
                }
            }

            generateAreaVertexArrayRear(vertexArray, areaVertexArray,
                                    frontRightMinX, frontRightMinZ,
                                    frontRightMaxX, frontRightMaxZ,
                                    backLeftMinX, backLeftMinZ,
                                    backLeftMaxX, backLeftMaxZ,
                                    backRightMinX, backRightMinZ,
                                    backRightMaxX, backRightMaxZ,
                                    (float)angle);

            if(angle == 135){
                generateMaxAngleLineVertexArray(maxAngleLineVertexArray,
                                        backRightMinX, backRightMinZ,
                                        backRightMaxX, backRightMaxZ,
                                        (float)angle);
            }

            frontRightMinX.clear();
            frontRightMinZ.clear();
            frontRightMaxX.clear();
            frontRightMaxZ.clear();
            backLeftMinX.clear();
            backLeftMinZ.clear();
            backLeftMaxX.clear();
            backLeftMaxZ.clear();
            backRightMinX.clear();
            backRightMinZ.clear();
            backRightMaxX.clear();
            backRightMaxZ.clear();
        }
    }

    unsigned int circleVBO, circleVAO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexArray.size(), vertexArray.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int maxAngleVBO, maxAngleVAO;
    glGenVertexArrays(1, &maxAngleVAO);
    glGenBuffers(1, &maxAngleVBO);
    glBindVertexArray(maxAngleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, maxAngleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxAngleLineVertexArray.size(), maxAngleLineVertexArray.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    glBindVertexArray(0);


    unsigned int areaVBO, areaVAO;
    glGenVertexArrays(1, &areaVAO);
    glGenBuffers(1, &areaVBO);
    glBindVertexArray(areaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, areaVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * areaVertexArray.size(), areaVertexArray.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    glBindVertexArray(0);

    // ---------------------------Dynimic Line vertex data------------------------------end


    // ---------------------------color block vertex data------------------------------begine
    float vertices[] = {  // x, y, z, alpha
        // // ju xing xian
        // 0.5f,  0.1f, -0.5f, 0.4f, // C
        // 0.5f,  1.0f, -0.5f, 0.4f, // B
        // 0.5f,  1.0f,  0.5f, 0.4f, // F
        // 0.5f,  0.1f,  0.5f, 0.4f, // G you

        // // shang ju xing
        // 0.5f,  0.1f,  0.5f, 0.1f, // G you
        // 0.5f,  0.1f, -0.5f, 0.1f, // C
        // 0.5f,  1.0f, -0.5f, 0.1f, // B
        // 0.5f,  1.0f, -0.5f, 0.1f, // B
        // 0.5f,  1.0f,  0.5f, 0.1f, // F
        // 0.5f,  0.1f,  0.5f, 0.1f, // G

        // //xia ju xing
        // 0.5f,  0.0f,  0.5f, 0.8f,  // G you
        // 0.5f,  0.0f, -0.5f, 0.8f,  // C
        // 0.5f,  0.1f, -0.5f, 0.8f,  // B
        // 0.5f,  0.1f, -0.5f, 0.8f,  // B
        // 0.5f,  0.1f,  0.5f, 0.8f,  // F
        // 0.5f,  0.0f,  0.5f, 0.8f,  // G

        //---------------------------line------------------
        -2.0f, 10.0f, 5.0f, 0.7f,
        -2.0f, 95.0f, 5.0f, 0.7f,
        2.0f, 95.0f, 5.0f, 0.7f,
        2.0f, 10.0f, 5.0f, 0.7f,

        2.0f, 95.0f, 5.0f, 0.7f,
        2.0f, 95.0f, -5.0f, 0.7f,
        2.0f, 10.0f, -5.0f, 0.7f,

        2.0f, 95.0f, -5.0f, 0.7f,
        -2.0f, 95.0f, -5.0f, 0.7f,
        -2.0f, 10.0f, -5.0f, 0.7f,

        -2.0f, 95.0f, -5.0f, 0.7f,
        -2.0f, 95.0f, 5.0f, 0.7f,

        //---------------------------down---------------------
        //front
        -2.0f, 10.0f, 5.0f, 1.0f,
         2.0f, 10.0f, 5.0f, 1.0f,
        -2.0f, 0.0f, 5.0f, 1.0f,
        -2.0f, 0.0f, 5.0f, 1.0f,
         2.0f, 0.0f, 5.0f, 1.0f,
         2.0f, 10.0f, 5.0f, 1.0f,

        //back
        -2.0f, 10.0f, -5.0f, 1.0f,
         2.0f, 10.0f, -5.0f, 1.0f,
        -2.0f, 0.0f, -5.0f, 1.0f,
        -2.0f, 0.0f, -5.0f, 1.0f,
         2.0f, 0.0f, -5.0f, 1.0f,
         2.0f, 10.0f, -5.0f, 1.0f,

        //left
        -2.0f, 10.0f, 5.0f, 1.0f,
        -2.0f, 10.0f, -5.0f, 1.0f,
        -2.0f, 0.0f, -5.0f, 1.0f,
        -2.0f, 0.0f, -5.0f, 1.0f,
        -2.0f, 0.0f, 5.0f, 1.0f,
        -2.0f, 10.0f, 5.0f, 1.0f,

        //right
        2.0f, 10.0f, 5.0f, 1.0f,
        2.0f, 10.0f, -5.0f, 1.0f,
        2.0f, 0.0f, -5.0f, 1.0f,
        2.0f, 0.0f, -5.0f, 1.0f,
        2.0f, 0.0f, 5.0f, 1.0f,
        2.0f, 10.0f, 5.0f, 1.0f,

        //---------------------------up---------------------
        //up
        -2.0f, 95.0f, 5.0f, 0.3f,
        -2.0f, 95.0f, -5.0f, 0.3f,
        2.0f,  95.0f, 5.0f, 0.3f,
        2.0f,  95.0f, 5.0f, 0.3f,
        -2.0f, 95.0f, -5.0f, 0.3f,
        2.0f,  95.0f, -5.0f, 0.3f,

        //front
        -2.0f, 95.0f, 5.0f, 0.3f,
         2.0f, 95.0f, 5.0f, 0.3f,
        -2.0f, 10.0f, 5.0f, 0.3f,
        -2.0f, 10.0f, 5.0f, 0.3f,
         2.0f, 10.0f, 5.0f, 0.3f,
         2.0f, 95.0f, 5.0f, 0.3f,

        //back
        -2.0f, 95.0f, -5.0f, 0.3f,
         2.0f, 95.0f, -5.0f, 0.3f,
        -2.0f, 10.0f, -5.0f, 0.3f,
        -2.0f, 10.0f, -5.0f, 0.3f,
         2.0f, 10.0f, -5.0f, 0.3f,
         2.0f, 95.0f, -5.0f, 0.3f,

        //left
        -2.0f, 95.0f, 5.0f, 0.3f,
        -2.0f, 95.0f, -5.0f, 0.3f,
        -2.0f, 10.0f, -5.0f, 0.3f,
        -2.0f, 10.0f, -5.0f, 0.3f,
        -2.0f, 10.0f, 5.0f, 0.3f,
        -2.0f, 95.0f, 5.0f, 0.3f,

        //right
        2.0f, 95.0f, 5.0f, 0.3f,
        2.0f, 95.0f, -5.0f, 0.3f,
        2.0f, 10.0f, -5.0f, 0.3f,
        2.0f, 10.0f, -5.0f, 0.3f,
        2.0f, 10.0f, 5.0f, 0.3f,
        2.0f, 95.0f, 5.0f, 0.3f,
    };
    // ---------------------------color block vertex data------------------------------end

    //----------------------------radar rectangular Area--------------------------------------begine
    unsigned int radarRectangularVBO, radarRectangularAreaVAO;
    glGenVertexArrays(1, &radarRectangularAreaVAO);
    glGenBuffers(1, &radarRectangularVBO);
    glBindVertexArray(radarRectangularAreaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, radarRectangularVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(48 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)((48 + 3) * sizeof(float)));
    glBindVertexArray(0);
    //----------------------------radar rectangular Area--------------------------------------end

    //----------------------------radar rectangular Line--------------------------------------begine
    unsigned int radarRectangularLineVAO;
    glGenVertexArrays(1, &radarRectangularLineVAO);
    glBindVertexArray(radarRectangularLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, radarRectangularVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    //----------------------------radar rectangular Line--------------------------------------end


    // ---------------------------ground------------------------------begine
    float planeVertices[] = {
        // positions x,y,z   //texture x, y
        5.0f,  0.0f,  5.0f,  2.0f, 0.0f,
        -5.0f, 0.0f,  5.0f,  0.0f, 0.0f,
        -5.0f, 0.0f, -5.0f,  0.0f, 2.0f,

        5.0f,  0.0f,  5.0f,  2.0f, 0.0f,
        -5.0f, 0.0f, -5.0f,  0.0f, 2.0f,
        5.0f,  0.0f, -5.0f,  2.0f, 2.0f
    };

    unsigned int groundVAO, groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());
    unsigned int transparentTexture = loadTexture(FileSystem::getPath("resources/textures/grass.png").c_str());

    unsigned int cameraNoChoosedTexture = loadTexture(FileSystem::getPath("resources/textures/camera_no_choosed.png").c_str());
    unsigned int cameraChoosedTexture = loadTexture(FileSystem::getPath("resources/textures/camera_choosed.png").c_str());

    unsigned int doolOpenNum1 = loadTexture(FileSystem::getPath("resources/textures/001.png").c_str());
    unsigned int doolOpenNum2 = loadTexture(FileSystem::getPath("resources/textures/002.png").c_str());
    unsigned int doolOpenNum3 = loadTexture(FileSystem::getPath("resources/textures/003.png").c_str());
    unsigned int doolOpenNum4 = loadTexture(FileSystem::getPath("resources/textures/004.png").c_str());
    unsigned int doolOpenNum5 = loadTexture(FileSystem::getPath("resources/textures/005.png").c_str());

    unsigned int navigationBarBg = loadTexture(FileSystem::getPath("resources/textures/navigationBar/bg.png").c_str());
    unsigned int navigation_01_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/01_normal.png").c_str());
    unsigned int navigation_01_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/01_disable.png").c_str());
    unsigned int navigation_02_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/02_normal.png").c_str());
    unsigned int navigation_02_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/02_disable.png").c_str());
    unsigned int navigation_03_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/03_normal.png").c_str());
    unsigned int navigation_03_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/03_disable.png").c_str());
    unsigned int navigation_04_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/04_normal.png").c_str());
    unsigned int navigation_04_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/04_disable.png").c_str());
    unsigned int navigation_05_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/05_normal.png").c_str());
    unsigned int navigation_05_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/05_disable.png").c_str());
    unsigned int navigation_06_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/06_normal.png").c_str());
    unsigned int navigation_06_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/06_disable.png").c_str());
    unsigned int navigation_07_normal = loadTexture(FileSystem::getPath("resources/textures/navigationBar/07_normal.png").c_str());
    unsigned int navigation_07_disable = loadTexture(FileSystem::getPath("resources/textures/navigationBar/07_disable.png").c_str());

    // ---------------------------ground------------------------------end

    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        1.0f, -1.0f,  0.0f,  1.0f,  1.0f,

        -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        1.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  0.0f,  1.0f,  0.0f
    };
    // transparent VAO
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    //if (FT_New_Face(ft, "resources/textures/Roboto-Light.ttf", 0, &face))  //Roboto-Light  arial
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configure VAO/VBO for texture quads
    unsigned int fontVAO, fontVBO;
    glGenVertexArrays(1, &fontVAO);
    glGenBuffers(1, &fontVBO);
    glBindVertexArray(fontVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // render loop
    float degree = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float wheelAngle = wheelAngleArray.front();
        wheelAngleArray.pop_front();

        _distance += 20.0f;
        r -= 0.1f;
        g += 0.1f;

        if (_distance > 450.0f) {
            _distance = 250.0f;
            r = 1.0f;
            g = 0.0f;
        }

    {
        glDisable(GL_DEPTH_TEST);

        shaderCamera.use();
        shaderCamera.setMat4("model", glm::mat4(1.0));

        //------------------------------------------navigation bar view--------------------------------------------------------------
        glViewport(TOP_VIEW_WIDTH, 0, (GLsizei)NAVIGATION_BAR_VIEW_WIDTH, (GLsizei)NAVIGATION_BAR_VIEW_HEIGHT);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationBarBg);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 614, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationOne == 1 ? navigation_01_normal : navigation_01_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 515, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationTwo == 1 ? navigation_02_normal : navigation_02_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 415, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationThree == 1 ? navigation_03_normal : navigation_03_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 315, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationFour == 1 ? navigation_04_normal : navigation_04_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 215, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationFive == 1 ? navigation_05_normal : navigation_05_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 115, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationSix == 1 ? navigation_06_normal : navigation_06_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //--------------------------
        glViewport(TOP_VIEW_WIDTH + 35, 16, (GLsizei)90, (GLsizei)90);

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, navigationSeven == 1 ? navigation_07_normal : navigation_07_disable);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
    }

    {
        // //------------------------------------------top view--------------------------------------------------------------
        const float Yaw_top = 180.0f;
        const float Pitch_top = 89.0f;
        const float radius_top = 900.0;
        const glm::vec3 at_top = glm::vec3(0.0f, 0.0f, 0.0f);


        glViewport(0, 0, (GLsizei)TOP_VIEW_WIDTH, (GLsizei)TOP_VIEW_HEIGHT);


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)TOP_VIEW_WIDTH / (float)TOP_VIEW_HEIGHT, 0.1f, 1000.0f);

        float camX = cos(glm::radians(Yaw_top)) * cos(glm::radians(Pitch_top)) * radius_top;
        float camY = sin(glm::radians(Pitch_top)) * radius_top;
        float camZ = sin(glm::radians(Yaw_top)) * cos(glm::radians(Pitch_top)) * radius_top;

        glm::vec3 camera = glm::vec3(camX, camY, camZ);

        glm::mat4 view = glm::lookAt(camera*0.07f, at_top*0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
        // // printf("camX = %f, camY = %f, camZ = %f, radius = %f, Yaw = %f, Pitch = %f, at.x = %f, at.z = %f, defaultAreaMin=%f, defaultAreaMax=%f\n",
        // //         camX, camY, camZ, radius, Yaw, Pitch, at.x, at.z,defaultAreaMin, defaultAreaMax);

        // --------------------draw ground------------------
        groundShader.use();
        //groundShader.setInt("texture1", 0);
        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);
        glBindVertexArray(groundVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 modelGround = glm::mat4(1.0);
        modelGround = glm::scale(modelGround, glm::vec3(60.0f, 60.0f, 60.0f));
        groundShader.setMat4("model", modelGround);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        //------------------------draw car---------------------
        carShader.use();
        carShader.setMat4("projection", projection);
        carShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        carShader.setMat4("model", model);
        ourModel.Draw_Car(carShader, wheelAngle, DOOR_L1, DOOR_L2, DOOR_R1, DOOR_R2, DOOR_B);

        //------------------------draw Maximum Angle of rotation---------------------
        shaderProgramMaxAngle.use();
        shaderProgramMaxAngle.setMat4("view", view);
        shaderProgramMaxAngle.setMat4("projection", projection);
        shaderProgramMaxAngle.setMat4("model", model);

        shaderProgramMaxAngle.setVec3("color", 1.0f, 0.0f, 0.0f);
        glBindVertexArray(maxAngleVAO);

        if(wheelAngle == 45.0f || wheelAngle == -45.0f || wheelAngle == 135.0f || wheelAngle == 225.0f){
            ;
        }else if(wheelAngle == 0.0f){
            shaderProgramMaxAngle.setFloat("Index", 45.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            shaderProgramMaxAngle.setFloat("Index", -45.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
        }else if(wheelAngle > 0.0f && wheelAngle < 45.0f){
            shaderProgramMaxAngle.setFloat("Index", 45.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
        }else if(wheelAngle < 0.0f && wheelAngle > -45.0f){
            shaderProgramMaxAngle.setFloat("Index", -45.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
        }else if(wheelAngle == 180.0f){
            shaderProgramMaxAngle.setFloat("Index", 135.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            shaderProgramMaxAngle.setFloat("Index", 225.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
        }else if(wheelAngle > 135.0f && wheelAngle < 180.0f){
            shaderProgramMaxAngle.setFloat("Index", 135.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
        }else if(wheelAngle < 225.0f && wheelAngle > 180.0f){
            shaderProgramMaxAngle.setFloat("Index", 225.0f);
            glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
        }else{
            assert(0);
        }

        //------------------------draw Dynimic Line---------------------
        shaderProgram.use();
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("model", model);
        shaderProgram.setFloat("Index", wheelAngle);
        shaderProgram.setVec3("color", 0.0f, 1.0f, 0.0f);
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLES, 0, RectangleNum*6);

        //------------------------draw Dynimic Line area---------------------
        // shaderProgram.setVec3("color", 0.0f, 0.0f, 1.0f);
        // glBindVertexArray(areaVAO);
        // glDrawArrays(GL_TRIANGLES, 0, areaRectangleNum*6);




        //--------------------draw color block-------------------
        radarShader.use();
        radarShader.setMat4("view", view);
        radarShader.setMat4("projection", projection);
        radarShader.setVec3("color", r, g, 0.0f);

        //--------------------------------- front -----------------------
        glm::mat4 model2;
        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -20.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 20.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -60.0f));
        model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * 5.0f));
        model2 = glm::rotate(model2, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
        model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * -5.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 60.0f));
        model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * -5.0f));
        model2 = glm::rotate(model2, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
        model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * 5.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        //--------------------------------- back -----------------------
        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, -20.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, 20.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, -60.0f));
        model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * 5.0f));
        model2 = glm::rotate(model2, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
        model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * -5.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        model2 = glm::mat4(1.0);
        model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
        model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, 60.0f));
        model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * -5.0f));
        model2 = glm::rotate(model2, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
        model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * 5.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 54);

        //--------------------------------- left -----------------------
        for(int i = 140; i >= -160; )
        {
            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3((float)i, 0.0f, -(_distance-140)));
            model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.5f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            i -= 60;
        }

        //--------------------------------- right -----------------------
        for(int i = 140; i >= -160; )
        {
            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3((float)i, 0.0f, (_distance-140)));
            model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.5f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            i -= 60;
        }

        shaderCamera.use();

        assert(cameraUpLeft + cameraUp + cameraUpRight + cameraDownLeft + cameraDown + cameraDownRight + cameraLeft + cameraRight + cameraLeftUp2Down + cameraRightUp2Down + cameraLeftDown2Up + cameraRightDown2Up <= 1);

        //left
        glm::mat4 modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(-6.0f, 0.0f, 0.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraLeft == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        // //right
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(6.0f, 0.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraRight == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //up
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, 8.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraUp == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //down
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, -8.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraDown == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //down left
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(-6.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, -8.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraDownLeft == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //down right
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(6.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, -8.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraDownRight == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //up right
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(6.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, 8.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraUpRight == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //up left
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(-6.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, 8.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(315.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraUpLeft == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //left up -> down
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(-4.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, 5.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraLeftUp2Down == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //right up -> down
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(4.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, 5.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraRightUp2Down == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //right down -> up
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(4.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, -5.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraRightDown2Up == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        //left down -> up
        modelCamera = glm::mat4(1.0);
        modelCamera = glm::scale(modelCamera, glm::vec3(0.1f, 0.1f, 0.1f));
        modelCamera = glm::translate(modelCamera, glm::vec3(-4.0f, 0.0f, 0.0f));
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, -5.0f, 0.0f));
        modelCamera = glm::rotate(modelCamera, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, (cameraLeftDown2Up == 1 ? cameraChoosedTexture : cameraNoChoosedTexture));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // x:[] y:[]

        modelCamera = glm::mat4(1.0);
        modelCamera = glm::translate(modelCamera, glm::vec3(-0.8f, -0.95f, 0.0f));
        modelCamera = glm::scale(modelCamera, glm::vec3(0.05f, 0.05f, 0.05f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, doolOpenNum1);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        modelCamera = glm::mat4(1.0);
        modelCamera = glm::translate(modelCamera, glm::vec3(-0.4f, -0.95f, 0.0f));
        modelCamera = glm::scale(modelCamera, glm::vec3(0.05f, 0.05f, 0.05f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, doolOpenNum2);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        modelCamera = glm::mat4(1.0);
        modelCamera = glm::translate(modelCamera, glm::vec3(0.0f, -0.95f, 0.0f));
        modelCamera = glm::scale(modelCamera, glm::vec3(0.05f, 0.05f, 0.05f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, doolOpenNum3);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        modelCamera = glm::mat4(1.0);
        modelCamera = glm::translate(modelCamera, glm::vec3(0.4f, -0.95f, 0.0f));
        modelCamera = glm::scale(modelCamera, glm::vec3(0.05f, 0.05f, 0.05f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, doolOpenNum4);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        modelCamera = glm::mat4(1.0);
        modelCamera = glm::translate(modelCamera, glm::vec3(0.8f, -0.95f, 0.0f));
        modelCamera = glm::scale(modelCamera, glm::vec3(0.05f, 0.05f, 0.05f));
        shaderCamera.setMat4("model", modelCamera);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, doolOpenNum5);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

    }

        //------------------------------------------right view--------------------------------------------------------------
        glViewport(TOP_VIEW_WIDTH + NAVIGATION_BAR_VIEW_WIDTH , 0, (GLsizei)FREE_VIEW_WIDTH, (GLsizei)FREE_VIEW_HEIGHT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)FREE_VIEW_WIDTH / (float)FREE_VIEW_HEIGHT, 0.1f, 1000.0f);

        float camX = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;
        float camY = sin(glm::radians(Pitch)) * radius;
        float camZ = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;

        glm::vec3 camera = glm::vec3(camX, camY, camZ);

        glm::mat4 view = glm::lookAt(camera*0.05f, at*0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
        // printf("camX = %f, camY = %f, camZ = %f, radius = %f, Yaw = %f, Pitch = %f, at.x = %f, at.z = %f, defaultAreaMin=%f, defaultAreaMax=%f\n",
        //         camX, camY, camZ, radius, Yaw, Pitch, at.x, at.z,defaultAreaMin, defaultAreaMax);

        // --------------------draw ground------------------
        groundShader.use();
        //groundShader.setInt("texture1", 0);
        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);
        glBindVertexArray(groundVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 modelGround = glm::mat4(1.0);
        modelGround = glm::scale(modelGround, glm::vec3(60.0f, 60.0f, 60.0f));
        groundShader.setMat4("model", modelGround);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        //------------------------draw car---------------------
        carShader.use();
        carShader.setMat4("projection", projection);
        carShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        carShader.setMat4("model", model);
        ourModel.Draw_Car(carShader, wheelAngle, /*DOOR_L1*/false, /*DOOR_L2*/false, /*DOOR_R1*/false, /*DOOR_R2*/false, /*DOOR_B*/false);

        //------------------------draw Maximum Angle of rotation---------------------
        if(navigationFour == 1) {
            shaderProgramMaxAngle.use();
            shaderProgramMaxAngle.setMat4("view", view);
            shaderProgramMaxAngle.setMat4("projection", projection);
            shaderProgramMaxAngle.setMat4("model", model);

            shaderProgramMaxAngle.setVec3("color", 1.0f, 0.0f, 0.0f);
            glBindVertexArray(maxAngleVAO);

            if(wheelAngle == 45.0f || wheelAngle == -45.0f || wheelAngle == 135.0f || wheelAngle == 225.0f){
                ;
            }else if(wheelAngle == 0.0f){
                shaderProgramMaxAngle.setFloat("Index", 45.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
                shaderProgramMaxAngle.setFloat("Index", -45.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            }else if(wheelAngle > 0.0f && wheelAngle < 45.0f){
                shaderProgramMaxAngle.setFloat("Index", 45.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            }else if(wheelAngle < 0.0f && wheelAngle > -45.0f){
                shaderProgramMaxAngle.setFloat("Index", -45.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            }else if(wheelAngle == 180.0f){
                shaderProgramMaxAngle.setFloat("Index", 135.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
                shaderProgramMaxAngle.setFloat("Index", 225.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            }else if(wheelAngle > 135.0f && wheelAngle < 180.0f){
                shaderProgramMaxAngle.setFloat("Index", 135.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            }else if(wheelAngle < 225.0f && wheelAngle > 180.0f){
                shaderProgramMaxAngle.setFloat("Index", 225.0f);
                glDrawArrays(GL_TRIANGLES, 0, maxAngleRectangleNum*6);
            }else{
                assert(0);
            }

            //------------------------draw Dynimic Line---------------------
            shaderProgram.use();
            shaderProgram.setMat4("view", view);
            shaderProgram.setMat4("projection", projection);
            shaderProgram.setMat4("model", model);
            shaderProgram.setFloat("Index", wheelAngle);
            shaderProgram.setVec3("color", 0.0f, 1.0f, 0.0f);
            glBindVertexArray(circleVAO);
            glDrawArrays(GL_TRIANGLES, 0, RectangleNum*6);

            //------------------------draw Dynimic Line area---------------------
            // shaderProgram.setVec3("color", 0.0f, 0.0f, 1.0f);
            // glBindVertexArray(areaVAO);
            // glDrawArrays(GL_TRIANGLES, 0, areaRectangleNum*6);
        }


        if(navigationFive == 1) {
            //--------------------draw color block-------------------
            radarShader.use();
            radarShader.setMat4("view", view);
            radarShader.setMat4("projection", projection);
            radarShader.setVec3("color", r, g, 0.0f);

            //--------------------------------- front -----------------------
            glm::mat4 model2;
            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -20.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 20.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -60.0f));
            model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * 5.0f));
            model2 = glm::rotate(model2, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
            model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * -5.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 60.0f));
            model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * -5.0f));
            model2 = glm::rotate(model2, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
            model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * 5.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            //--------------------------------- back -----------------------
            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, -20.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, 20.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, -60.0f));
            model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * 5.0f));
            model2 = glm::rotate(model2, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
            model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * -5.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            model2 = glm::mat4(1.0);
            model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
            model2 = glm::translate(model2, glm::vec3(-_distance, 0.0f, 60.0f));
            model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 3.6f * -5.0f));
            model2 = glm::rotate(model2, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.3f));
            model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 3.6f * 5.0f));
            radarShader.setMat4("model", model2);
            glBindVertexArray(radarRectangularLineVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 12);
            glBindVertexArray(radarRectangularAreaVAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);

            //--------------------------------- left -----------------------
            for(int i = 140; i >= -160; )
            {
                model2 = glm::mat4(1.0);
                model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
                model2 = glm::translate(model2, glm::vec3((float)i, 0.0f, -(_distance-140)));
                model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.5f));
                radarShader.setMat4("model", model2);
                glBindVertexArray(radarRectangularLineVAO);
                glDrawArrays(GL_LINE_STRIP, 0, 12);
                glBindVertexArray(radarRectangularAreaVAO);
                glDrawArrays(GL_TRIANGLES, 0, 54);

                i -= 60;
            }

            //--------------------------------- right -----------------------
            for(int i = 140; i >= -160; )
            {
                model2 = glm::mat4(1.0);
                model2 = glm::scale(model2, glm::vec3(0.05f, 0.05f, 0.05f));
                model2 = glm::translate(model2, glm::vec3((float)i, 0.0f, (_distance-140)));
                model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.5f));
                radarShader.setMat4("model", model2);
                glBindVertexArray(radarRectangularLineVAO);
                glDrawArrays(GL_LINE_STRIP, 0, 12);
                glBindVertexArray(radarRectangularAreaVAO);
                glDrawArrays(GL_TRIANGLES, 0, 54);

                i -= 60;
            }
        }

        glm::mat4 fontprojection = glm::ortho(0.0f, static_cast<GLfloat>(FREE_VIEW_WIDTH), 0.0f, static_cast<GLfloat>(FREE_VIEW_HEIGHT));
        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(fontprojection));

        RenderText(shader, "Please pay attention to the surrounding environment", 200.0f, 650.0f, 0.7f, glm::vec3(1.0, 0.0f, 0.0f), fontVAO, fontVBO);
        RenderText(shader, string("Steering wheel Angle : [") + std::to_string(wheelAngle) + "]", 15.0f, 25.0f, 0.3f, glm::vec3(0.0, 1.0f, 0.0f), fontVAO, fontVBO);


        //--------------------on screen-------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        wheelAngleArray.push_back(wheelAngle);
    }

    glDeleteVertexArrays(1, &radarRectangularAreaVAO);
    glDeleteVertexArrays(1, &radarRectangularLineVAO);
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteVertexArrays(1, &areaVAO);
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteVertexArrays(1, &transparentVAO);
    glDeleteVertexArrays(1, &fontVAO);

    glDeleteBuffers(1, &circleVBO);
    glDeleteBuffers(1, &radarRectangularVBO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteBuffers(1, &areaVBO);
    glDeleteBuffers(1, &transparentVBO);
    glDeleteBuffers(1, &fontVBO);


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, unsigned int VAO, unsigned int VBO)
{
    // Activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        radius -= 50.0;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        radius += 50.0;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        at.x += 20.0;
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        at.x -= 20.0;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        at.z += 20.0;
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        at.z -= 20.0;
    }

//----------------------------------------------------------------
    // if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    //     DOOR_L1 = !DOOR_L1;
    // }

    // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    //     DOOR_L2 = !DOOR_L2;
    // }

    // if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    //     DOOR_R1 = !DOOR_R1;
    // }

    // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    //     DOOR_R2 = !DOOR_R2;
    // }

    // if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
    //     DOOR_B = !DOOR_B;
    // }

    // if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
    //     //qian
    //     Yaw = 0.0f;
    //     Pitch = 119.0f;
    //     radius = 400.0;
    //     at = glm::vec3(-366.0f, 0.0f, 0.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
    //     //hou
    //     Yaw = 180.0f;
    //     Pitch = 119.0f;
    //     radius = 400.0;
    //     at = glm::vec3(366.0f, 0.0f, 0.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
    //     //ding
    //     Yaw = 90.0f;
    //     Pitch = 45.0f;
    //     radius = 700.0;
    //     at = glm::vec3(0.0f, 0.0f, 0.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
    //     //ding
    //     Yaw = -90.0f;
    //     Pitch = 45.0f;
    //     radius = 700.0;
    //     at = glm::vec3(0.0f, 0.0f, 0.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
    //     //ding
    //     Yaw = 194.0f;
    //     Pitch = 15.0f;
    //     radius = 500.0;
    //     at = glm::vec3(0.0f, 0.0f, -300.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    //     //ding
    //     Yaw = 166.0f;
    //     Pitch = 15.0f;
    //     radius = 500.0;
    //     at = glm::vec3(0.0f, 0.0f, 300.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    //     //ding
    //     Yaw = 346.0f;
    //     Pitch = 15.0f;
    //     radius = 500.0;
    //     at = glm::vec3(0.0f, 0.0f, -300.0f);
    // }

    // if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    //     //ding
    //     Yaw = 14.0f;
    //     Pitch = 15.0f;
    //     radius = 500.0;
    //     at = glm::vec3(0.0f, 0.0f, 300.0f);
    // }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        //hou
        Yaw = 180.0f;
        Pitch = 51.0f;
        radius = 700.0;
        at = glm::vec3(-14.0f, 0.0f, 0.0f);
    }

    //------------------------------------------------------------------------

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        // at = glm::vec3(0.0f, 0.0f, 0.0f);
        // radius = 700.0f;
        Yaw -= 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        // at = glm::vec3(0.0f, 0.0f, 0.0f);
        // radius = 700.0f;
        Yaw += 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        // at = glm::vec3(0.0f, 0.0f, 0.0f);
        // radius = 700.0f;
        Pitch -= 1.0f;
        if (Pitch < 1.0f) Pitch = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        // at = glm::vec3(0.0f, 0.0f, 0.0f);
        // radius = 700.0f;
        Pitch += 1.0f;
        if (Pitch > 89.0f) Pitch = 89.0f;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    currentCursorX = int(xpos);
    currentCursorY = int(ypos);

    //printf("Mouse position move to [x=%d : y=%d]\n",int(xpos),int(ypos));
}

void processButtonLeftPress()
{
    if(currentCursorX >= 100 && currentCursorX <= 150){
        if(currentCursorY >= 50 && currentCursorY <= 100){
            cameraUpLeft = 1; cameraUp = cameraUpRight = 0;
            cameraDownLeft = cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 319.0f;
            Pitch = 28.0f;
            radius = 700.0f;
            at = glm::vec3(0.0f, 0.0f, 0.0f);
        }else if(currentCursorY >= 330 && currentCursorY <= 380){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft = cameraDown = cameraDownRight = 0;
            cameraLeft = 1;  cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = -90.0f;
            Pitch = 45.0f;
            radius = 700.0;
            at = glm::vec3(0.0f, 0.0f, 0.0f);
        }else if(currentCursorY >= 620 && currentCursorY <= 670){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft = 1; cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 220.0f;
            Pitch = 28.0f;
            radius = 700.0f;
            at = glm::vec3(0.0f, 0.0f, 0.0f);
        }else{
            ;
        }

    }else if(currentCursorX >= 160 && currentCursorX <= 210){
        if(currentCursorY >= 150 && currentCursorY <= 200){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft =  cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = 1;  cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 346.0f;
            Pitch = 15.0f;
            radius = 500.0;
            at = glm::vec3(0.0f, 0.0f, -300.0f);
        }else if(currentCursorY >= 510 && currentCursorY <= 560){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft =  cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = 1;  cameraRightDown2Up = 0;

            Yaw = 194.0f;
            Pitch = 15.0f;
            radius = 500.0;
            at = glm::vec3(0.0f, 0.0f, -300.0f);
        }else{
            ;
        }
    }else if(currentCursorX >= 300 && currentCursorX <= 350){
        if(currentCursorY >= 50 && currentCursorY <= 100){
            cameraUpLeft = 0; cameraUp = 1;  cameraUpRight = 0;
            cameraDownLeft =  cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up =  cameraRightDown2Up = 0;

            Yaw = 180.0f;
            Pitch = 119.0f;
            radius = 400.0;
            at = glm::vec3(366.0f, 0.0f, 0.0f);
        }else if(currentCursorY >= 620 && currentCursorY <= 670){
            cameraUpLeft = cameraUp =  cameraUpRight = 0;
            cameraDownLeft = 0;  cameraDown = 1; cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up =  cameraRightDown2Up = 0;

            Yaw = 0.0f;
            Pitch = 119.0f;
            radius = 400.0;
            at = glm::vec3(-366.0f, 0.0f, 0.0f);
        }else{
            ;
        }
    }else if(currentCursorX >= 420 && currentCursorX <= 470){
        if(currentCursorY >= 150 && currentCursorY <= 200){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft =  cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = 0;  cameraRightUp2Down = 1;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 14.0f;
            Pitch = 15.0f;
            radius = 500.0;
            at = glm::vec3(0.0f, 0.0f, 300.0f);
        }else if(currentCursorY >= 510 && currentCursorY <= 560){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft =  cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = 0; cameraRightDown2Up = 1;

            Yaw = 166.0f;
            Pitch = 15.0f;
            radius = 500.0;
            at = glm::vec3(0.0f, 0.0f, 300.0f);
        }else{
            ;
        }
    }else if(currentCursorX >= 500 && currentCursorX <= 550){
        if(currentCursorY >= 50 && currentCursorY <= 100){
            cameraUpLeft = cameraUp = 0; cameraUpRight = 1;
            cameraDownLeft = cameraDown = cameraDownRight = 0;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 41.0f;
            Pitch = 28.0f;
            radius = 700.0f;
            at = glm::vec3(0.0f, 0.0f, 0.0f);
        }else if(currentCursorY >= 330 && currentCursorY <= 380){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft = cameraDown = cameraDownRight = 0;
            cameraLeft = 0; cameraRight = 1;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 90.0f;
            Pitch = 45.0f;
            radius = 700.0;
            at = glm::vec3(0.0f, 0.0f, 0.0f);

        }else if(currentCursorY >= 620 && currentCursorY <= 670){
            cameraUpLeft = cameraUp = cameraUpRight = 0;
            cameraDownLeft = cameraDown = 0; cameraDownRight = 1;
            cameraLeft = cameraRight = 0;
            cameraLeftUp2Down = cameraRightUp2Down = 0;
            cameraLeftDown2Up = cameraRightDown2Up = 0;

            Yaw = 140.0f;
            Pitch = 28.0f;
            radius = 700.0f;
            at = glm::vec3(0.0f, 0.0f, 0.0f);
        }else{
            ;
        }
    }else{
        ;
    }

    if(currentCursorY >= 680 && currentCursorY <= 720){
        if(currentCursorX >= 50 && currentCursorX <= 80){
            DOOR_L1 = !DOOR_L1;
        }else if(currentCursorX >= 180 && currentCursorX <= 210){
            DOOR_L2 = !DOOR_L2;
        }else if(currentCursorX >= 300 && currentCursorX <= 330){
            DOOR_R1 = !DOOR_R1;
        }else if(currentCursorX >= 430 && currentCursorX <= 460){
            DOOR_R2 = !DOOR_R2;
        }else if(currentCursorX >= 560 && currentCursorX <= 590){
            DOOR_B = !DOOR_B;
        }else{
            ;
        }
    }

    if(currentCursorX >= 675 && currentCursorX <= 765){
        if(currentCursorY >= 16 && currentCursorY <= 109){
            navigationOne = !navigationOne;
        }else if(currentCursorY >= 115 && currentCursorY <= 208){
            navigationTwo = !navigationTwo;
        }else if(currentCursorY >= 215 && currentCursorY <= 310){
            navigationThree = !navigationThree;
        }else if(currentCursorY >= 315 && currentCursorY <= 408){
            navigationFour = !navigationFour;
        }else if(currentCursorY >= 415 && currentCursorY <= 509){
            navigationFive = !navigationFive;
        }else if(currentCursorY >= 515 && currentCursorY <= 608){
            navigationSix = !navigationSix;
        }else if(currentCursorY >= 614 && currentCursorY <= 709){
            navigationSeven = !navigationSeven;
        }else{
            ;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS){
        switch(button){
            case GLFW_MOUSE_BUTTON_LEFT:
                //std::cout << "Mosue left button clicked!" << std::endl;
                processButtonLeftPress();
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                //std::cout << "Mosue middle button clicked!" << std::endl;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                //std::cout << "Mosue right button clicked!" << std::endl;
                break;
            default:
                return;
        }
    }
    return;
}
