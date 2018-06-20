#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <unistd.h>

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
     "   }else{\n"
     "       ourColor = vec4(0.234f, 0.234f, 0.406f, 0.5f);\n"
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

int totalNumber = 0;


std::vector<float> vertexArray;
std::vector<unsigned int> indexArray;

static int num = 0;

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
    for(int i = 0; i < (rectanglePeerRadarWithoutDegree*radarNumber); i++)
    {
        for(int j = 0; j < degreeNumber; j++)
        {
            float _rMin=rMin+(j*(rMax-rMin))/degreeNumber;
            float _rMax=rMin+((j+1)*(rMax-rMin))/degreeNumber;
            float a = (_rMin-rMin)/(rMax-rMin);
            angle = beginAngle + (i*(endAngle-beginAngle))/(rectanglePeerRadarWithoutDegree*radarNumber);

            // bottom right
            arrayVertex.push_back((float)(((num/50)*degreeNumber) + (num%50%degreeNumber)));
            arrayVertex.push_back((float) (_rMax*sin(angle*PI/180.0f)) );
            arrayVertex.push_back((float) (_rMax*cos(angle*PI/180.0f)) );
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);

            // bottom left
            arrayVertex.push_back((float)(((num/50)*degreeNumber) + (num%50%degreeNumber)));
            arrayVertex.push_back((float) (_rMin*sin(angle*PI/180.0f)));
            arrayVertex.push_back((float) (_rMin*cos(angle*PI/180.0f)));
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);


            angle = beginAngle+((i+1)*(endAngle-beginAngle))/(rectanglePeerRadarWithoutDegree*radarNumber);
            // top right
            arrayVertex.push_back((float)(((num/50)*degreeNumber) + (num%50%degreeNumber)));
            arrayVertex.push_back((float) (_rMax*sin(angle*PI/180.0f)));
            arrayVertex.push_back((float) (_rMax*cos(angle*PI/180.0f)));
            arrayVertex.push_back(0.0f);

            arrayVertex.push_back(1-a);
            arrayVertex.push_back(a);
            arrayVertex.push_back(0.0f);
            arrayVertex.push_back(0.5f);

            // top left
            arrayVertex.push_back((float)(((num/50)*degreeNumber) + (num%50%degreeNumber)));
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

            num++;
        }
    }
}

//AppendVertexArrayWithRectangle

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

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
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


//--------------------------------------generate vertex array--------------------------------------------------------
    int degreeNumber = 0;
    int radarNum = 0;
    int rectanglePeerRadarWithoutDegree = 0;

    // draw area : front
    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 4;
    degreeNumber = 10;
    AppendVertexArrayWithSector(vertexArray, indexArray, -45.0f, 45.0f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // draw area : rear
    rectanglePeerRadarWithoutDegree = 5;
    radarNum = 4;
    degreeNumber = 10;
    AppendVertexArrayWithSector(vertexArray, indexArray, 135.0f, 225.0f, 0.4f, 0.8f, rectanglePeerRadarWithoutDegree, radarNum, degreeNumber);
    totalNumber += rectanglePeerRadarWithoutDegree*radarNum*degreeNumber;

    // draw area : left

    // draw area : right
//--------------------------------------generate vertex array--------------------------------------------------------

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

    float degree = 0.0f;
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
        glUniform4f(vertexColorLocation, degree, 1.0f, 1.0f, 1.0f);
        if(++degree == num/5) degree = 0.0f;
        usleep(100000);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, totalNumber*6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        //printf("num = %d\n", num);

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