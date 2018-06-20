#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>

#include <unistd.h>
#include <random>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

//#include "VertexGenerator.h"

const float PI = 3.14159265359;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 720;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec4 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "out vec4 ourColor;\n"
    "uniform  vec4  colorTrigleIndex;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.y, aPos.z, aPos.w, 1.0);\n"
    "   if(colorTrigleIndex.x == aPos.x){\n"
    "       ourColor = aColor;\n"
    "   }\n"
    "   if(colorTrigleIndex.y == aPos.x){\n"
    "       ourColor = aColor;\n"
    "       ourColor.a = 0.1f;\n"
    "   }\n"
    "   if(colorTrigleIndex.z == aPos.x){\n"
    "       ourColor = aColor;\n"
    "       ourColor.a = 0.1f;\n"
    "   }\n"
    "   if(colorTrigleIndex.w == aPos.x){\n"
    "       ourColor = aColor;\n"
    "       ourColor.a = 0.1f;\n"
    "   }\n"
    "   if(colorTrigleIndex.x != aPos.x && colorTrigleIndex.y != aPos.x && colorTrigleIndex.z != aPos.x && colorTrigleIndex.w != aPos.x){\n"
    "       ourColor = vec4(0.234f, 0.234f, 0.406f, 0.1f);\n"
    "   }\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

static int indexnum = 0;
static int degreeNum = 0;

static int degreeIndex = 0;

int totalNumber = 0;


std::vector<float> vertexArray;
std::vector<unsigned int> indexArray;

typedef enum RADAR {
    FRONT_ONE = 0,
    FRONT_TWO,
    FRONT_THREE,
    FRONT_FOUR,

    REAR_ONE,
    REAR_TWO,
    REAR_THREE,
    REAR_FOUR,

    RIGHT_ONE,
    RIGHT_TWO,
    RIGHT_THREE,
    RIGHT_FOUR,

    LEFT_ONE,
    LEFT_TWO,
    LEFT_THREE,
    LEFT_FOUR,
} RADAR_ENUM;

std::map<RADAR_ENUM, std::map<int, float> > radarMap;

std::map<RADAR_ENUM, int > radarDegree;

RADAR_ENUM radarIndex = FRONT_ONE;

void AppendVertexArrayWithSector(std::vector<float>& arrayVertex,
                                 std::vector<unsigned int>& arrayIndex,
                                 float beginAngle,
                                 float endAngle,
                                 float rMin,
                                 float rMax,
                                 int rectanglePeerRadarWithoutDegree, // need how many big rectangle each radar
                                 int radarNumber, // has how many radars
                                 int degreeNumber)  //degree about the radar(each degree is one little rectangle)
{
    float angle = beginAngle;
    int generateDegreeNum = 0;

    for(int i = 0; i < radarNumber; i++)
    {
        for(int j = 0; j < degreeNumber; j++)
        {
            for(int k = 0; k < rectanglePeerRadarWithoutDegree; k++)
            {
                float _rMin=rMin+(j*(rMax-rMin))/degreeNumber;
                float _rMax=rMin+((j+1)*(rMax-rMin))/degreeNumber;
                float a = (_rMin-rMin)/(rMax-rMin);
                angle = beginAngle + ((i*rectanglePeerRadarWithoutDegree+k)*(endAngle-beginAngle))/(rectanglePeerRadarWithoutDegree*radarNumber);

                // bottom right
                arrayVertex.push_back((float)(degreeNum+i*degreeNumber+j));
                arrayVertex.push_back((float) (_rMax*sin(angle*PI/180.0f)) );
                arrayVertex.push_back((float) (_rMax*cos(angle*PI/180.0f)) );
                arrayVertex.push_back(0.0f);

                arrayVertex.push_back(1-a);
                arrayVertex.push_back(a);
                arrayVertex.push_back(0.0f);
                arrayVertex.push_back(0.5f);

                // bottom left
                arrayVertex.push_back((float)(degreeNum+i*degreeNumber+j));
                arrayVertex.push_back((float) (_rMin*sin(angle*PI/180.0f)));
                arrayVertex.push_back((float) (_rMin*cos(angle*PI/180.0f)));
                arrayVertex.push_back(0.0f);

                arrayVertex.push_back(1-a);
                arrayVertex.push_back(a);
                arrayVertex.push_back(0.0f);
                arrayVertex.push_back(0.5f);


                angle = beginAngle+((i*rectanglePeerRadarWithoutDegree+k+1)*(endAngle-beginAngle))/(rectanglePeerRadarWithoutDegree*radarNumber);
                // top right
                arrayVertex.push_back((float)(degreeNum+i*degreeNumber+j));
                arrayVertex.push_back((float) (_rMax*sin(angle*PI/180.0f)));
                arrayVertex.push_back((float) (_rMax*cos(angle*PI/180.0f)));
                arrayVertex.push_back(0.0f);

                arrayVertex.push_back(1-a);
                arrayVertex.push_back(a);
                arrayVertex.push_back(0.0f);
                arrayVertex.push_back(0.5f);

                // top left
                arrayVertex.push_back((float)(degreeNum+i*degreeNumber+j));
                arrayVertex.push_back((float) (_rMin*sin(angle*PI/180.0f)));
                arrayVertex.push_back((float) (_rMin*cos(angle*PI/180.0f)));
                arrayVertex.push_back(0.0f);

                arrayVertex.push_back(1-a);
                arrayVertex.push_back(a);
                arrayVertex.push_back(0.0f);
                arrayVertex.push_back(0.5f);

                //------------------------
                arrayIndex.push_back(0+indexnum*4);
                arrayIndex.push_back(1+indexnum*4);
                arrayIndex.push_back(3+indexnum*4);
                arrayIndex.push_back(0+indexnum*4);
                arrayIndex.push_back(2+indexnum*4);
                arrayIndex.push_back(3+indexnum*4);
                indexnum++;
            }
            generateDegreeNum++;
            radarMap[radarIndex][j] = degreeIndex++;
            printf("radarIndex = %d, j = %d, radarMap[radarIndex][j] = %f\n",radarIndex, j, radarMap[radarIndex][j]);
        }
        radarDegree[radarIndex] = degreeNumber;
        radarIndex = (RADAR_ENUM)((int)radarIndex + 1);
    }

    degreeNum += generateDegreeNum;
}

void AppendVertexArrayWithRectangle(std::vector<float>& arrayVertex,
                                 std::vector<unsigned int>& arrayIndex,
                                 float beginAngle,
                                 float endAngle,
                                 float rMin,
                                 float rMax,
                                 int rectanglePeerRadarWithoutDegree, // need how many big rectangle each radar
                                 int radarNumber, // has how many radars
                                 int degreeNumber)  //degree about the radar(each degree is one little rectangle)
{
    // bottom right
    float bottomRightX = rMax*sin(endAngle*PI/180.0f);
    float bottomRightY = rMax*cos(endAngle*PI/180.0f);

    // bottom left
    float bottomLeftX = rMin*sin(endAngle*PI/180.0f);
    float bottomLeftY = rMin*cos(endAngle*PI/180.0f);

    // top right
    float topRightX = rMax*sin(beginAngle*PI/180.0f);
    float topRightY = rMax*cos(beginAngle*PI/180.0f);

    // top left
    float topLeftX = rMin*sin(beginAngle*PI/180.0f);
    float topLeftY = rMin*cos(beginAngle*PI/180.0f);

    float angle = beginAngle;
    int generateDegreeNum = 0;

    for(int i = 0; i < radarNumber; i++)
    {
        float _topLeftX = topLeftX;
        float _topLeftY = topLeftY > 0.0 ? (topLeftY - i*(fabs(topLeftY-bottomLeftY)/radarNumber)) : (topLeftY + i*(fabs(topLeftY-bottomLeftY)/radarNumber));

        float _bottomLeftX = bottomLeftX;
        float _bottomLeftY = topLeftY > 0.0 ? (topLeftY - (i+1)*(fabs(topLeftY-bottomLeftY)/radarNumber) + 0.01) : (topLeftY + (i+1)*(fabs(topLeftY-bottomLeftY)/radarNumber) - 0.01);

        float _topRightX = topRightX;
        float _topRightY = topRightY > 0.0 ? (topRightY - i*(fabs(topRightY-bottomRightY)/radarNumber)) : (topRightY + i*(fabs(topRightY-bottomRightY)/radarNumber));

        float _bottomRightX = bottomRightX;
        float _bottomRightY = topRightY > 0.0 ? (topRightY - (i+1)*(fabs(topRightY-bottomRightY)/radarNumber) + 0.01) : (topRightY + (i+1)*(fabs(topRightY-bottomRightY)/radarNumber) - 0.01);

        for(int j = 0; j < degreeNumber; j++)
        {
            float __topLeftX = _topLeftX > 0.0 ? _topLeftX + j*(fabs(_topRightX-_topLeftX)/degreeNumber) : _topLeftX - j*(fabs(_topRightX-_topLeftX)/degreeNumber);
            float __topLeftY = _topLeftY == 0.0 ? _topLeftY : (_topLeftY > 0.0 ? _topLeftY + j*(fabs(_topLeftY-_topRightY)/degreeNumber) : _topLeftY - j*(fabs(_topLeftY-_topRightY)/degreeNumber));

            float __bottomLeftX = _bottomLeftX > 0.0 ? _bottomLeftX + j*(fabs(_bottomRightX-_bottomLeftX)/degreeNumber) : _bottomLeftX - j*(fabs(_bottomRightX-_bottomLeftX)/degreeNumber);
            float __bottomLeftY = _bottomLeftY == 0.0 ? _bottomLeftY : (_bottomLeftY > 0.0 ? _bottomLeftY + j*(fabs(_bottomLeftY-_bottomRightY)/degreeNumber) : _bottomLeftY - j*(fabs(_bottomLeftY-_bottomRightY)/degreeNumber));

            float __topRightX = _topLeftX > 0.0 ? _topLeftX + (j+1)*(fabs(_topRightX-_topLeftX)/degreeNumber) : _topLeftX - (j+1)*(fabs(_topRightX-_topLeftX)/degreeNumber);
            float __topRightY = _topLeftY == 0.0 ? _topLeftY : (_topLeftY > 0.0 ? _topLeftY + (j+1)*(fabs(_topLeftY-_topRightY)/degreeNumber) : _topLeftY - (j+1)*(fabs(_topLeftY-_topRightY)/degreeNumber));

            float __bottomRightX = _bottomLeftX > 0.0 ? _bottomLeftX + (j+1)*(fabs(_bottomRightX-_bottomLeftX)/degreeNumber) : _bottomLeftX - (j+1)*(fabs(_bottomRightX-_bottomLeftX)/degreeNumber);
            float __bottomRightY = _bottomLeftY == 0.0 ? _bottomLeftY : (_bottomLeftY > 0.0 ? _bottomLeftY + (j+1)*(fabs(_bottomLeftY-_bottomRightY)/degreeNumber) : _bottomLeftY - (j+1)*(fabs(_bottomLeftY-_bottomRightY)/degreeNumber));

            float index = (float)(degreeNum+i*degreeNumber+j);

            float _rMin=rMin+(j*(rMax-rMin))/degreeNumber;
            float a = (_rMin-rMin)/(rMax-rMin);

            // bottom right
            arrayVertex.push_back(index);
            arrayVertex.push_back(__bottomRightX);
            arrayVertex.push_back(__bottomRightY);
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);

            // bottom left
            arrayVertex.push_back(index);
            arrayVertex.push_back(__bottomLeftX);
            arrayVertex.push_back(__bottomLeftY);
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);

            // top right
            arrayVertex.push_back(index);
            arrayVertex.push_back(__topRightX);
            arrayVertex.push_back(__topRightY);
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);

            // top left
            arrayVertex.push_back(index);
            arrayVertex.push_back(__topLeftX);
            arrayVertex.push_back(__topLeftY);
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);

            //------------------------
            arrayIndex.push_back(0+indexnum*4);
            arrayIndex.push_back(1+indexnum*4);
            arrayIndex.push_back(3+indexnum*4);
            arrayIndex.push_back(0+indexnum*4);
            arrayIndex.push_back(2+indexnum*4);
            arrayIndex.push_back(3+indexnum*4);

            indexnum++;

            generateDegreeNum++;
            radarMap[radarIndex][j] = degreeIndex++;
            printf("radarIndex = %d, j = %d, radarMap[radarIndex][j] = %f\n",radarIndex, j, radarMap[radarIndex][j] );
        }
        radarDegree[radarIndex] = degreeNumber;
        radarIndex = (RADAR_ENUM)((int)radarIndex + 1);
    }

    degreeNum += generateDegreeNum;
}

void update(GLint vertexColorLocation, unsigned int radar, unsigned int degree)
{
    //glUniform4f(vertexColorLocation, radarMap[radar][degree], radarMap[radar][degree] + 1.0f, radarMap[radar][degree] + 2.0f, radarMap[radar][degree] + 3.0f);


    printf("radar = %u, degree = %u, radarMap[(RADAR_ENUM)radar][degree] = %f\n",radar, degree, radarMap[(RADAR_ENUM)radar][degree]);
    glUniform4f(vertexColorLocation, radarMap[(RADAR_ENUM)radar][degree], radarMap[(RADAR_ENUM)radar][degree] + 1.0f, radarMap[(RADAR_ENUM)radar][degree] + 2.0f, radarMap[(RADAR_ENUM)radar][degree] + 3.0f);
}

int main()
{
// -------------------------------------glfw: initialize and configure-----------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
// -------------------------------------glfw: initialize and configure-----------------------------------

// -------------------------------------glEnable-----------------------------------
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
// -------------------------------------glEnable-----------------------------------


//--------------------------------------build and compile our shader program--------------------------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
//--------------------------------------build and compile our shader program--------------------------------------------------------


//--------------------------------------generate vertex array-----1---------------------------------------------------
    // int degreeNumber = 0;
    // int radarNum = 0;
    // int rectanglePeerRadarWithoutDegree = 0;

    // // draw area : front
    // rectanglePeerRadarWithoutDegree = 5;
    // radarNum = 1;
    // degreeNumber = 20;
    // AppendVertexArrayWithSector(vertexArray, indexArray, -45.0f, -22.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // rectanglePeerRadarWithoutDegree = 5;
    // radarNum = 2;
    // degreeNumber = 20;
    // AppendVertexArrayWithSector(vertexArray, indexArray, -22.5f, 22.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // rectanglePeerRadarWithoutDegree = 5;
    // radarNum = 1;
    // degreeNumber = 20;
    // AppendVertexArrayWithSector(vertexArray, indexArray, 22.5f, 45.0f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // // draw area : rear
    // rectanglePeerRadarWithoutDegree = 5;
    // radarNum = 1;
    // degreeNumber = 20;
    // AppendVertexArrayWithSector(vertexArray, indexArray, 135.0f, 157.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // rectanglePeerRadarWithoutDegree = 5;
    // radarNum = 2;
    // degreeNumber = 20;
    // AppendVertexArrayWithSector(vertexArray, indexArray, 157.5f, 202.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // rectanglePeerRadarWithoutDegree = 5;
    // radarNum = 1;
    // degreeNumber = 20;
    // AppendVertexArrayWithSector(vertexArray, indexArray, 202.5f, 225.0f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // //draw area : right
    // rectanglePeerRadarWithoutDegree = 1;
    // radarNum = 4;
    // degreeNumber = 15;
    // AppendVertexArrayWithRectangle(vertexArray, indexArray, 45.0f, 135.0f, 0.4f, 0.7f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;


    // // draw area : left
    // rectanglePeerRadarWithoutDegree = 1;
    // radarNum = 4;
    // degreeNumber = 15;
    // AppendVertexArrayWithRectangle(vertexArray, indexArray, 225.0f, 315.0f, 0.4f, 0.7f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    // totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

//--------------------------------------generate vertex array-----1---------------------------------------------------


//--------------------------------------generate vertex array-----2---------------------------------------------------

    int degreeNumber = 0;
    int radarNum = 0;
    int rectanglePeerRadarWithoutDegree = 0;

    // draw area : front
    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 10;
    AppendVertexArrayWithSector(vertexArray, indexArray, -44.5f, -24.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 14;
    AppendVertexArrayWithSector(vertexArray, indexArray, -21.5f, -1.5f, 0.4f, 0.9f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 14;
    AppendVertexArrayWithSector(vertexArray, indexArray, 1.5f, 21.5f, 0.4f, 0.9f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 10;
    AppendVertexArrayWithSector(vertexArray, indexArray, 24.5f, 44.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // draw area : rear
    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 10;
    AppendVertexArrayWithSector(vertexArray, indexArray, 135.5f, 155.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 19;
    AppendVertexArrayWithSector(vertexArray, indexArray, 158.5f, 178.5f, 0.4f, 1.0f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 19;
    AppendVertexArrayWithSector(vertexArray, indexArray, 181.5f, 201.5f, 0.4f, 1.0f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 1;
    degreeNumber = 10;
    AppendVertexArrayWithSector(vertexArray, indexArray, 204.5f, 224.5f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    //draw area : right
    rectanglePeerRadarWithoutDegree = 1;
    radarNum = 4;
    degreeNumber = 10;
    AppendVertexArrayWithRectangle(vertexArray, indexArray, 47.5f, 132.5f, 0.4f, 0.7f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;


    // draw area : left
    rectanglePeerRadarWithoutDegree = 1;
    radarNum = 4;
    degreeNumber = 10;
    AppendVertexArrayWithRectangle(vertexArray, indexArray, 227.5f, 312.5f, 0.4f, 0.7f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

//--------------------------------------generate vertex array-----2---------------------------------------------------

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexArray.size(), vertexArray.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexArray.size(), indexArray.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//---------------------------------------------------------VA0------------------------------------------------------------------------------


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    RADAR_ENUM radar = FRONT_ONE;
    int degree = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);

        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "colorTrigleIndex");

        // printf("degree = %d\n", degree);
        // glUniform4f(vertexColorLocation, degree, degree + 1.0f, degree + 2.0f, degree + 3.0f);
        if(radar > LEFT_FOUR) radar = FRONT_ONE;

        printf("degree = %d, radarDegree[%d]=%d\n",degree,  (int)radar, radarDegree[radar]);

        if(degree > radarDegree[radar] - 4) {
            degree = 0;
            radar = (RADAR_ENUM)((int)radar + 1);
        }
        update(vertexColorLocation, radar, degree++);
        usleep(100000);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, totalNumber*6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}