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

const float PI = 3.14159265359;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 30.0f));
//bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float radius = 700.0f;//30.0f;
float Yaw = 90.0f;
float Pitch = 45.0f;

float _distance = 9.0f;
float r = 1.0f;
float g = 0.0f;

float wheelAngle = -45.0f;

glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);

int RectangleNum = 0;

int RiangleNum = 0;

std::vector<float> vertexArray;

enum Radius{
    NO,
    MIN,
    MAX
};

std::vector<float> areaVertexArray;

float caculateX(float x0, float z0, float z, float radius)
{
    float x = pow(pow(radius, 2) - pow(z - z0, 2), 0.5);

    if(x + x0 >= 0.0f)  {
        x = x + x0;
    }else if(-x + x0 >= 0.0f){
        x = -x + x0;
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
    assert(beginX < endX);

    //yuan xin
    float x0 = -134.0f;
    float z0 = 265.0f / (sin(glm::radians(float(_angle))) / cos(glm::radians(float(_angle))));


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
    if(_endX + x0 >= beginX)  {
        _endX = _endX + x0;
    }else if(-_endX + x0 >= beginX){
        _endX = -_endX + x0;
    }else{
        assert(0);
    }

    assert((_endX > 0.0f && endX > 0.0f) || (_endX < 0.0f && endX < 0.0f));

    if(_endX > 0.0f && endX > 0.0f){
        _endX = min(_endX, endX);
    }

    if(_endX < 0.0f && endX < 0.0f){
        _endX = max(_endX, endX);
    }

    float lengthX = abs(_endX - beginX);


    std::cout << " x0 = " << x0 << " z0 = " << z0 << " _radius = " << _radius << " angle = " << _angle << endl;
    std::cout << " beginZ = " << beginZ <<  " endZ = " << endZ <<  " lengthZ = " << lengthZ << endl;
    std::cout << " beginX = " << beginX <<  " _endX = " << _endX <<  " lengthX = " << lengthX << endl;



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
            std::cout << "lengthZ <=  lengthX ==> x = " << x << " z = " << z << std::endl;
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
            float x = caculateX(x0, z0, z, _radius);
            if(x > endX) return zStart + (i-1) * dif;
            arrayX.push_back(x);
            arrayZ.push_back(z);
            std::cout << "lengthZ >  lengthX ==> x = " << x << " z = " << z << std::endl;
        }
    }


    //assert(0);

    return 0.0f;
}

void generateLineVertexArray(std::vector<float>& array,
                            std::list<float>& XList,
                            std::list<float>& ZList,
                            float _angle)
{
    assert(XList.size() == ZList.size());

    for(int i = 0; i < XList.size(); i++)
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

void generateAreaVertexArray(std::vector<float>& areaVertexArray,
                            std::list<float>& _frontLeftMinX, std::list<float>& _frontLeftMinZ,
                            std::list<float>& _frontLeftMaxX, std::list<float>& _frontLeftMaxZ,
                            std::list<float>& _frontRightMinX, std::list<float>& _frontRightMinZ,
                            std::list<float>& _frontRightMaxX, std::list<float>& _frontRightMaxZ,
                            float _angle)
{

}

void TestPasFunLine()
{
    while (1) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        wheelAngle += 1.0f;

        if(wheelAngle == 0.0f) wheelAngle += 1.0f;

        if(wheelAngle > 45.0f){
            wheelAngle = -45.0f;
        }
    }
}


void TestPasFun()
{
    while (1) {
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _distance += 0.2f;
        r -= 0.02f;
        g += 0.02f;

        if (_distance > 19.0f) {
            _distance = 9.0f;
            r = 1.0f;
            g = 0.0f;
        }
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

    thread TestPASThread2 = thread{TestPasFunLine};

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

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
    // ------------------------------load car obj--------------------------------------end

    // ------------------------------build and compile shaders------------------------begin
    Shader carShader("car.vs", "car.fs");
    Shader radarShader("radar.vs", "radar.fs");
    Shader groundShader("ground.vs", "ground.fs");
    Shader dynimicLineShader("dynimic_line.vs", "dynimic_line.fs");
    Shader shaderProgram("dynimic_line_circle.vs", "dynimic_line_circle.fs");
    // ------------------------------build and compile shaders------------------------end

    // ---------------------------Dynimic Line vertex data------------------------------begine
    float dynimicLineVertices[] = {
        // D dang : qian lun dong tai xian
        148.0f,  0.1f,  -90.0f,  // a   //zuo qian
        148.0f,  0.1f,  -70.0f,  // c
        500.0f,  0.1f,  -90.0f,  //b
        500.0f,  0.1f,  -90.0f,  //b
        148.0f,  0.1f,  -70.0f,  //c
        500.0f,  0.1f,  -70.0f,  //d

        148.0f,  0.1f,  90.0f,  //you qian
        148.0f,  0.1f,  70.0f,
        500.0f,  0.1f,  90.0f,
        500.0f,  0.1f,  90.0f,
        148.0f,  0.1f,  70.0f,
        500.0f,  0.1f,  70.0f,

        // R dang : hou lun dong tai xian
        -148.0f,  0.1f,  -90.0f,  //zuo hou
        -148.0f,  0.1f,  -70.0f,
        -500.0f,  0.1f,  -90.0f,
        -500.0f,  0.1f,  -90.0f,
        -148.0f,  0.1f,  -70.0f,
        -500.0f,  0.1f,  -70.0f,

        -148.0f,  0.1f,  90.0f,  //you hou
        -148.0f,  0.1f,  70.0f,
        -500.0f,  0.1f,  90.0f,
        -500.0f,  0.1f,  90.0f,
        -148.0f,  0.1f,  70.0f,
        -500.0f,  0.1f,  70.0f,


        //----------------------------area---------------------------
        148.0f,  0.1f,  -70.0f, //area qian
        500.0f,  0.1f,  -70.0f,
        148.0f,  0.1f,  70.0f,
        500.0f,  0.1f,  -70.0f,
        148.0f,  0.1f,  70.0f,
        500.0f,  0.1f,  70.0f,

        -148.0f,  0.1f,  -70.0f, //area hou
        -500.0f,  0.1f,  -70.0f,
        -148.0f,  0.1f,  70.0f,
        -500.0f,  0.1f,  -70.0f,
        -148.0f,  0.1f,  70.0f,
        -500.0f,  0.1f,  70.0f,
    };

    std::vector<float> lineVertexArray;

    for(int angle = 45; angle >= 1; angle--)
    {
        if(angle == 0) continue;

        if(angle > 0) {
            //-----------front left------------------------
            radius = pow(pow((265.0f / (sin(glm::radians(float(angle))) / cos(glm::radians(float(angle)))) + 89), 2) + pow(265, 2), 0.5);
            float minR = radius-5.0f;
            float maxR = radius+5.0f;

            std::list<float> frontLeftMinX, frontLeftMinZ;
            float result;
            if(0.0f != (result = AppendVertexArray(134.0f, 400.0f, 265.0f, minR, (float)angle, frontLeftMinX, frontLeftMinZ))){
                frontLeftMinX.clear();
                frontLeftMinZ.clear();
                if(0.0f != (result = AppendVertexArray(134.0f, 400.0f, result, minR, (float)angle, frontLeftMinX, frontLeftMinZ))){
                    assert(0);
                }
            }

            // std::list<float> frontLeftMaxX, frontLeftMaxZ;
            // AppendVertexArray(134.0f, 400.0f, 265.0f, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ);

            // //-----------front right------------------------
            // radius = pow(pow((265.0f / (sin(glm::radians(float(angle))) / cos(glm::radians(float(angle)))) - 89), 2) + pow(265, 2), 0.5);
            // minR = radius-5.0f;
            // maxR = radius+5.0f;

            // std::list<float> frontRightMinX, frontRightMinZ;
            // AppendVertexArray(134.0f, 400.0f, 265.0f, minR, (float)angle, frontRightMinX, frontRightMinZ);
            // std::list<float> frontRightMaxX, frontRightMaxZ;
            // AppendVertexArray(134.0f, 400.0f, 265.0f, maxR, (float)angle, frontRightMaxX, frontRightMaxZ);

            // //-----------back right------------------------
            // radius = 265.0f / (sin(glm::radians(float(angle))) / cos(glm::radians(float(angle)))) - 89.0f;
            // minR = radius-5.0f;
            // maxR = radius+5.0f;

            // std::list<float> backRightMinX, backRightMinZ;
            // AppendVertexArray(-134.0f, 100.0f, 265.0f, minR, (float)angle, backRightMinX, backRightMinZ);
            // std::list<float> backRightMaxX, backRightMaxZ;
            // AppendVertexArray(-134.0f, 100.0f, 265.0f, maxR, (float)angle, backRightMaxX, backRightMaxZ);

            generateLineVertexArray(lineVertexArray, frontLeftMinX, frontLeftMinZ, (float)angle);


            // generateAreaVertexArray(areaVertexArray,
            //                         frontLeftMinX, frontLeftMinZ,
            //                         frontLeftMaxX, frontLeftMaxZ,
            //                         frontRightMinX, frontRightMinZ,
            //                         frontRightMaxX, frontRightMaxZ,
            //                         (float)angle);

            frontLeftMinX.clear();
            frontLeftMinZ.clear();
            // frontLeftMaxX.clear();
            // frontLeftMaxZ.clear();
            // frontRightMinX.clear();
            // frontRightMinZ.clear();
            // frontRightMaxX.clear();
            // frontRightMaxZ.clear();
            // backRightMinX.clear();
            // backRightMinZ.clear();
            // backRightMaxX.clear();
            // backRightMaxZ.clear();
        }


        // if(angle < 0) {
        //     //front left
        //     float radius1 = pow(pow((265.0f / (sin(glm::radians(float(-angle))) / cos(glm::radians(float(-angle)))) - 89), 2) + pow(265, 2), 0.5);
        //     float minR = radius1-5.0f;
        //     float maxR = radius1+5.0f;
        //     //std::cout << " radius1 = " << radius1 << " minR = " << minR << " maxR = " << maxR <<endl;
        //     std::list<float> frontLeftMaxX, frontLeftMaxZ;
        //     AppendVertexArray(vertexArray, 134.0f, 400.0f, 0.0f, -265.0f, minR, maxR, (float)angle, frontLeftMaxX, frontLeftMaxZ, MAX);

        //     //front right
        //     float radiu2 = pow(pow((265.0f / (sin(glm::radians(float(-angle))) / cos(glm::radians(float(-angle)))) + 89), 2) + pow(265, 2), 0.5);
        //     minR = radiu2-5.0f;
        //     maxR = radiu2+5.0f;
        //     std::list<float> frontRightMinX, frontRightMinZ;
        //     AppendVertexArray(vertexArray, 134.0f, 400.0f, 100.0f, -265.0f, minR, maxR, (float)angle, frontRightMinX, frontRightMinZ, MIN);

        //     //back left
        //     float radiu4 = 265.0f / (sin(glm::radians(float(-angle))) / cos(glm::radians(float(-angle)))) - 89.0f;
        //     minR = radiu4-5.0f;
        //     maxR = radiu4+5.0f;
        //     std::list<float> backLeftMaxX, backLeftMaxZ;
        //     AppendVertexArray(vertexArray, -134.0f, 100.0f, 0.0f, -265.0f, minR, maxR, (float)angle, backLeftMaxX, backLeftMaxZ, NO);

        //     generateAreaVertexArray(areaVertexArray, frontLeftMaxX, frontLeftMaxZ, frontRightMinX, frontRightMinZ, (float)angle);
        // }
    }

    //----------test line --------------------
    unsigned int testLineVBO, testLineVAO;
    glGenVertexArrays(1, &testLineVAO);
    glGenBuffers(1, &testLineVBO);
    glBindVertexArray(testLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, testLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lineVertexArray.size(), lineVertexArray.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    glBindVertexArray(0);
    //----------test line --------------------

    //vertexArray.insert(vertexArray.end(),areaVertexArray.begin(),areaVertexArray.end());

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

    //----------------------------Dynimic Line--------------------------------------begine
    unsigned int dynimicLineVBO, dynimicLineVAO;
    glGenVertexArrays(1, &dynimicLineVAO);
    glGenBuffers(1, &dynimicLineVBO);
    glBindVertexArray(dynimicLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dynimicLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dynimicLineVertices), dynimicLineVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    //----------------------------Dynimic Line--------------------------------------end

    //----------------------------Dynimic Line middle area--------------------------------------begine
    unsigned int dynimicLineAreaVAO;
    glGenVertexArrays(1, &dynimicLineAreaVAO);
    glBindVertexArray(dynimicLineAreaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dynimicLineVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(24*3 * sizeof(float)));
    glBindVertexArray(0);
    //----------------------------Dynimic Line middle area--------------------------------------end


    // ---------------------------zhou ju vertex data------------------------------begine
    float zhoujuvertices[] = {
        -134.0f,  0.11f, -89.0f,
        131.0f,   0.11f, -89.0f,

        131.0f,   0.11f, -89.0f,
        131.0f,   0.11f,  89.0f,

        131.0f,   0.11f,  89.0f,
        -134.0f,  0.11f,  89.0f,

        -134.0f,  0.11f,  89.0f,
        -134.0f,  0.11f, -89.0f,

        -134.0f,  0.11f,  89.0f,
        -134.0f,  0.11f,  265.0f / (sin(glm::radians(wheelAngle)) / cos(glm::radians(wheelAngle))),

        -134.0f,  0.11f,  265.0f / (sin(glm::radians(wheelAngle)) / cos(glm::radians(wheelAngle))),
        131.0f,   0.11f,  89.0f,

        -134.0f,  0.11f,  265.0f / (sin(glm::radians(wheelAngle)) / cos(glm::radians(wheelAngle))),
        131.0f,   0.11f, -89.0f,

        -134.0f,  0.11f,  89.0f,
        -134.0f,  0.11f,  265.0f / (sin(glm::radians(25.0f)) / cos(glm::radians(25.0f))),

        -134.0f,  0.11f,  265.0f / (sin(glm::radians(25.0f)) / cos(glm::radians(25.0f))),
        131.0f,   0.11f,  89.0f,

        -134.0f,  0.11f,  265.0f / (sin(glm::radians(25.0f)) / cos(glm::radians(25.0f))),
        131.0f,   0.11f, -89.0f,
    };

    unsigned int zhouJuLineVBO, zhouJuLineVAO;
    glGenVertexArrays(1, &zhouJuLineVAO);
    glGenBuffers(1, &zhouJuLineVBO);
    glBindVertexArray(zhouJuLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, zhouJuLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zhoujuvertices), zhoujuvertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // ---------------------------zhou ju vertex data------------------------------begine


    // ---------------------------color block vertex data------------------------------begine
    float vertices[] = {  // x, y, z, alpha
        // ju xing xian
        0.5f,  0.1f, -0.5f, 0.4f, // C
        0.5f,  1.0f, -0.5f, 0.4f, // B
        0.5f,  1.0f,  0.5f, 0.4f, // F
        0.5f,  0.1f,  0.5f, 0.4f, // G you

        // shang ju xing
        0.5f,  0.1f,  0.5f, 0.1f, // G you
        0.5f,  0.1f, -0.5f, 0.1f, // C
        0.5f,  1.0f, -0.5f, 0.1f, // B
        0.5f,  1.0f, -0.5f, 0.1f, // B
        0.5f,  1.0f,  0.5f, 0.1f, // F
        0.5f,  0.1f,  0.5f, 0.1f, // G

        //xia ju xing
        0.5f,  0.0f,  0.5f, 0.8f,  // G you
        0.5f,  0.0f, -0.5f, 0.8f,  // C
        0.5f,  0.1f, -0.5f, 0.8f,  // B
        0.5f,  0.1f, -0.5f, 0.8f,  // B
        0.5f,  0.1f,  0.5f, 0.8f,  // F
        0.5f,  0.0f,  0.5f, 0.8f,  // G
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(16 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)((16 + 3) * sizeof(float)));
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


    // ---------------------------ground vertex data & texture data------------------------------begine
    float planeVertices[] = {
        // positions x,y,z   //texture x, y
        5.0f,  0.0f,  5.0f,  2.0f, 0.0f,
        -5.0f, 0.0f,  5.0f,  0.0f, 0.0f,
        -5.0f, 0.0f, -5.0f,  0.0f, 2.0f,

        5.0f,  0.0f,  5.0f,  2.0f, 0.0f,
        -5.0f, 0.0f, -5.0f,  0.0f, 2.0f,
        5.0f,  0.0f, -5.0f,  2.0f, 2.0f
    };
    // ---------------------------ground vertex data & texture data------------------------------begine

    // ---------------------------ground------------------------------begine
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
    // ---------------------------ground------------------------------end

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


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        float camX = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;
        float camY = sin(glm::radians(Pitch)) * radius;
        float camZ = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;

        glm::vec3 camera = glm::vec3(camX, camY, camZ);

        glm::mat4 view = glm::lookAt(camera*0.05f, at*0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
        // printf("camX = %f, camY = %f, camZ = %f, radius = %f, Yaw = %f, Pitch = %f, at.x = %f, at.z = %f, defaultAreaMin=%f, defaultAreaMax=%f\n",
        //         camX, camY, camZ, radius, Yaw, Pitch, at.x, at.z,defaultAreaMin, defaultAreaMax);

        // --------------------draw ground------------------
        groundShader.use();
        groundShader.setInt("texture1", 0);
        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);

        glBindVertexArray(groundVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 modelGround = glm::mat4(1.0);
        modelGround = glm::scale(modelGround, glm::vec3(60.0f, 60.0f, 60.0f));   // it's a bit too big for our scene, so scale it down
        groundShader.setMat4("model", modelGround);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        //------------------------draw car---------------------
        carShader.use();
        carShader.setMat4("projection", projection);
        carShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));   // it's a bit too big for our scene, so scale it down
        carShader.setMat4("model", model);
        ourModel.Draw(carShader);

        //--------------------draw Dynimic Line-------------------
        // dynimicLineShader.use();
        // dynimicLineShader.setMat4("view", view);
        // dynimicLineShader.setMat4("projection", projection);
        // dynimicLineShader.setVec3("color", 0.0f, 1.0f, 0.0f);
        // dynimicLineShader.setFloat("_alpa", 1.0f);
        // dynimicLineShader.setMat4("model", model);
        // glBindVertexArray(dynimicLineVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 24);
        // glBindVertexArray(dynimicLineAreaVAO);
        // dynimicLineShader.setVec3("color", 0.0f, 0.0f, 1.0f);
        // dynimicLineShader.setFloat("_alpa", 0.3f);
        // glDrawArrays(GL_TRIANGLES, 0, 12);

        // draw Dynimic Line circle
        shaderProgram.use();
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setVec3("color", 1.0f, 0.0f, 0.0f);
        //shaderProgram.setFloat("_alpa", 1.0f);
        shaderProgram.setFloat("Index", /*wheelAngle*/ 45.0f);
        shaderProgram.setFloat("u_alpha", 0.8f);
        //model3 = glm::translate(model3, glm::vec3(0.0f, 0.0f, 265.0f / (sin(glm::radians(wheelAngle)) / cos(glm::radians(wheelAngle)))));
        shaderProgram.setMat4("model", model);
        // glBindVertexArray(circleVAO);
        // glDrawArrays(GL_TRIANGLES, 0, RectangleNum*6/* + RiangleNum*3*/);
        // shaderProgram.setVec3("color", 0.0f, 0.0f, 1.0f);
        // shaderProgram.setFloat("u_alpha", 0.2f);
        // glBindVertexArray(areaVAO);
        // glDrawArrays(GL_TRIANGLES, 0, areaRectangleNum*6/* + areaRiangleNum*3*/);
        glLineWidth(3);
        //--------------testline-------------------
        // glBindVertexArray(testLineVAO);
        // glDrawArrays(GL_LINE_STRIP, 0, lineVertexArray.size()/5);
        //--------------testline-------------------



        glLineWidth(3);

        //--------------------------zhou ju----------------------------------
        // dynimicLineShader.use();
        // dynimicLineShader.setMat4("view", view);
        // dynimicLineShader.setMat4("projection", projection);
        // dynimicLineShader.setVec3("color", 1.0f, 0.0f, 0.0f);
        // dynimicLineShader.setFloat("_alpa", 1.0f);
        // dynimicLineShader.setMat4("model", model);
        // glBindVertexArray(zhouJuLineVAO);
        // glDrawArrays(GL_LINES, 0, 20);

        glLineWidth(1);

        //--------------------draw color block-------------------
        radarShader.use();
        radarShader.setMat4("view", view);
        radarShader.setMat4("projection", projection);
        radarShader.setVec3("color", r, g, 0.0f);

        //--------------------------------- front -----------------------
        glm::mat4 model2;
        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance-0.5f, 0.0f, -3.0f));
        model2 = glm::rotate(model2, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 2.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance-0.2f, 0.0f, -1.7f));
        model2 = glm::rotate(model2, glm::radians(9.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -0.5f));
        model2 = glm::rotate(model2, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 0.5f));
        model2 = glm::rotate(model2, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance-0.2f, 0.0f, 1.7f));
        model2 = glm::rotate(model2, glm::radians(-9.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance-0.5f, 0.0f, 3.0f));
        model2 = glm::rotate(model2, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 2.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //--------------------------------- back -----------------------
        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 4.5, 0.0f, -5.0f));
        model2 = glm::rotate(model2, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 2.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5.2, 0.0f, -2.7f));
        model2 = glm::rotate(model2, glm::radians(-9.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5.4, 0.0f, -1.0f));
        model2 = glm::rotate(model2, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5.4, 0.0f, 1.0f));
        model2 = glm::rotate(model2, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5.2, 0.0f, 2.7f));
        model2 = glm::rotate(model2, glm::radians(9.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 4.5, 0.0f, 5.0f));
        model2 = glm::rotate(model2, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 2.0f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //--------------------------------- left -----------------------
        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(9.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(7.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(5.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(3.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(1.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-3.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-5.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-7.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-9.0f, 0.0f, (-1) * _distance + 7));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);



        //--------------------------------- right -----------------------
        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(9.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(7.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(5.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(3.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(1.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-3.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-5.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-7.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(-9.0f, 0.0f, _distance - 1));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //--------------------on screen-------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &radarRectangularAreaVAO);
    glDeleteVertexArrays(1, &radarRectangularLineVAO);
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteVertexArrays(1, &dynimicLineVAO);
    glDeleteVertexArrays(1, &dynimicLineAreaVAO);
    glDeleteVertexArrays(1, &zhouJuLineVAO);
    glDeleteVertexArrays(1, &areaVAO);
    glDeleteBuffers(1, &radarRectangularVBO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteBuffers(1, &dynimicLineVBO);
    glDeleteBuffers(1, &zhouJuLineVBO);
    glDeleteBuffers(1, &areaVBO);

    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        //hou zuo
        Yaw = 220.0f;
        Pitch = 28.0f;
        radius = 700.0f;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        //hou you
        Yaw = 140.0f;
        Pitch = 28.0f;
        radius = 700.0f;
        //at = glm::vec3(-1556.0f, 214.0f, -657.0f);
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        //qian zuo
        Yaw = 319.0f;
        Pitch = 28.0f;
        radius = 700.0f;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        //qian you
        Yaw = 41.0f;
        Pitch = 28.0f;
        radius = 700.0f;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        //ding
        Yaw = 180.0f;
        Pitch = 89.0f;
        radius = 900.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        //qian
        Yaw = 0.0f;
        Pitch = 119.0f;
        radius = 400.0;
        at = glm::vec3(-366.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        //hou
        Yaw = 180.0f;
        Pitch = 119.0f;
        radius = 400.0;
        at = glm::vec3(366.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        //ding
        Yaw = 90.0f;
        Pitch = 45.0f;
        radius = 700.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        //ding
        Yaw = -90.0f;
        Pitch = 45.0f;
        radius = 700.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        //ding
        Yaw = 194.0f;
        Pitch = 15.0f;
        radius = 500.0;
        at = glm::vec3(0.0f, 0.0f, -300.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        //ding
        Yaw = 166.0f;
        Pitch = 15.0f;
        radius = 500.0;
        at = glm::vec3(0.0f, 0.0f, 300.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        //ding
        Yaw = 346.0f;
        Pitch = 15.0f;
        radius = 500.0;
        at = glm::vec3(0.0f, 0.0f, -300.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        //ding
        Yaw = 14.0f;
        Pitch = 15.0f;
        radius = 500.0;
        at = glm::vec3(0.0f, 0.0f, 300.0f);
    }

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


