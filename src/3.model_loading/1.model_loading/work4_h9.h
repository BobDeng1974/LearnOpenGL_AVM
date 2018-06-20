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

float radius = 30.0f;
float Yaw = 90.0f;
float Pitch = 45.0f;

float _distance = 8.0f;
float r = 1.0f;
float g = 0.0f;


glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);

typedef struct _ColorBlock
{
    int index;
    float distance;
    float r;
    float g;

    float x;
    float y;
    float z;
} ColorBlock;

void TestPasFun()
{
    while (1) {
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _distance += 0.2f;
        r -= 0.02f;
        g += 0.02f;

        if (_distance > 18.0f) {
            _distance = 8.0f;
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

int main()
{
    thread TestPASThread = thread{TestPasFun};
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
    Model ourModel(FileSystem::getPath("resources/objects/nanosuit/Car.obj"));
    // ------------------------------load car obj--------------------------------------end

    // ------------------------------build and compile shaders------------------------begin
    Shader carShader("car.vs", "car.fs");
    Shader radarShader("radar.vs", "radar.fs");
    Shader groundShader("ground.vs", "ground.fs");
    Shader dynimicLineShader("dynimic_line.vs", "dynimic_line.fs");
    // ------------------------------build and compile shaders------------------------end

    // ---------------------------Dynimic Line vertex data------------------------------begine
    float dynimicLineVertices[] = {
        // D dang : qian lun dong tai xian
        0.38f,  0.01f,  -0.6f,  //zuo qian
        0.38f,  0.01f,  -0.45f,
        1.38f,  0.01f,  -0.6f,
        1.38f,  0.01f,  -0.6f,
        0.38f,  0.01f,  -0.45f,
        1.38f,  0.01f,  -0.45f,

        0.38f,  0.01f,  0.6f,  //you qian
        0.38f,  0.01f,  0.45f,
        1.38f,  0.01f,  0.6f,
        1.38f,  0.01f,  0.6f,
        0.38f,  0.01f,  0.45f,
        1.38f,  0.01f,  0.45f,

        // R dang : hou lun dong tai xian
        -0.45f,  0.01f,  -0.6f,  //zuo hou
        -0.45f,  0.01f,  -0.45f,
        -1.45f,  0.01f,  -0.6f,
        -1.45f,  0.01f,  -0.6f,
        -0.45f,  0.01f,  -0.45f,
        -1.45f,  0.01f,  -0.45f,

        -0.45f,  0.01f,  0.6f,  //you hou
        -0.45f,  0.01f,  0.45f,
        -1.45f,  0.01f,  0.6f,
        -1.45f,  0.01f,  0.6f,
        -0.45f,  0.01f,  0.45f,
        -1.45f,  0.01f,  0.45f,



        //----------------------------area---------------------------
        0.38f,  0.01f,  -0.45f, //area qian
        1.38f,  0.01f,  -0.45f,
        0.38f,  0.01f,  0.45f,
        1.38f,  0.01f,  -0.45f,
        0.38f,  0.01f,  0.45f,
        1.38f,  0.01f,  0.45f,

        -0.45f,  0.01f,  -0.45f, //area hou
        -1.45f,  0.01f,  -0.45f,
        -0.45f,  0.01f,  0.45f,
        -1.45f,  0.01f,  -0.45f,
        -0.45f,  0.01f,  0.45f,
        -1.45f,  0.01f,  0.45f,
    };
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

        // D dang : qian lun dong tai xian
        0.5f,  0.0f,  -0.5f, 0.6,  //zuo qian
        0.5f,  0.0f,  -0.4f, 0.6,
        0.5f,  0.0f,  0.4f,  0.6,
        0.5f,  0.0f,  0.5f,  0.6,

        0.5f,  0.0f,  -0.5f, 0.6,  //you qian
        0.5f,  0.0f,  -0.4f, 0.6,
        0.5f,  0.0f,  0.4f,  0.6,
        0.5f,  0.0f,  0.5f,  0.6,
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
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        float camX = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;
        float camY = sin(glm::radians(Pitch)) * radius;
        float camZ = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), at, glm::vec3(0.0f, 1.0f, 0.0f));
        // printf("camX = %f \t camY = %f \t camZ = %f \t radius = %f \t Yaw = %f\t Pitch = %f \t at.x = %f\t at.z = %f\n",
        //         camX, camY, camZ, radius, Yaw, Pitch, at.x, at.z);

        // --------------------draw ground------------------
        groundShader.use();
        groundShader.setInt("texture1", 0);
        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);

        glBindVertexArray(groundVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 modelGround = glm::mat4(1.0);
        modelGround = glm::scale(modelGround, glm::vec3(6.0f, 6.0f, 6.0f));   // it's a bit too big for our scene, so scale it down
        groundShader.setMat4("model", modelGround);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        //------------------------draw car---------------------
        carShader.use();
        carShader.setMat4("projection", projection);

        // glm::mat4 view = glm::lookAt(glm::vec3(-20.0f, 20.0f, 0.0f),
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, 1.0f, 0.0f));
        // carShader.setMat4("view", view);

        // float radius = 30.0f;
        // float camX   = sin(glfwGetTime()) * radius;
        // float camZ   = cos(glfwGetTime()) * radius;
        // glm::mat4 view = glm::lookAt(glm::vec3(camX, 30.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // carShader.setMat4("view", view);
        carShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0);
        //model = glm::translate(model, glm::vec3(0.0f, 2.0f/*-1.75f*/, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.0045f, 0.0045f, 0.0045f));   // it's a bit too big for our scene, so scale it down
        carShader.setMat4("model", model);
        ourModel.Draw(carShader);

        //--------------------draw Dynimic Line-------------------
        dynimicLineShader.use();
        dynimicLineShader.setMat4("view", view);
        dynimicLineShader.setMat4("projection", projection);
        dynimicLineShader.setVec3("color", 0.0f, 1.0f, 0.0f);
        dynimicLineShader.setFloat("_alpa", 1.0f);

        glm::mat4 model1;
        model1 = glm::mat4(1.0);
        model1 = glm::translate(model1, glm::vec3(1, 0.1f, 0.0f));
        model1 = glm::scale(model1, glm::vec3(15.0f, 0.1f, 6.0f));
        radarShader.setMat4("model", model1);
        glBindVertexArray(dynimicLineVAO);
        glDrawArrays(GL_TRIANGLES, 0, 24);
        glBindVertexArray(dynimicLineAreaVAO);
        dynimicLineShader.setVec3("color", 0.0f, 0.0f, 1.0f);
        dynimicLineShader.setFloat("_alpa", 0.3f);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //--------------------draw color block-------------------
        radarShader.use();
        radarShader.setMat4("view", view);
        radarShader.setMat4("projection", projection);
        radarShader.setVec3("color", r, g, 0.0f);

        //glLineWidth(1);

        //--------------------------------- front -----------------------
        glm::mat4 model2;
        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -3.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);


        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, -1.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 1.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3(_distance, 0.0f, 3.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //--------------------------------- back -----------------------

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5, 0.0f, -3.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);


        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5, 0.0f, -1.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5, 0.0f, 1.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
        radarShader.setMat4("model", model2);
        glBindVertexArray(radarRectangularLineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glBindVertexArray(radarRectangularAreaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        model2 = glm::mat4(1.0);
        model2 = glm::translate(model2, glm::vec3((-1) * _distance - 5, 0.0f, 3.0f));
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 1.5f));
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
    glDeleteBuffers(1, &radarRectangularVBO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteBuffers(1, &dynimicLineVBO);

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
        radius -= 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        radius += 1.0;
    }

    // if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    //     at.x += 1.0;
    // }

    // if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    //     at.x -= 1.0;
    // }

    // if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
    //     at.z += 1.0;
    // }

    // if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
    //     at.z -= 1.0;
    // }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        //hou zuo
        Yaw = 220.0f;
        Pitch = 28.0f;
        radius = 30.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        //hou you
        Yaw = 140.0f;
        Pitch = 28.0f;
        radius = 30.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        //qian zuo
        Yaw = 319.0f;
        Pitch = 28.0f;
        radius = 30.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        //qian you
        Yaw = 41.0f;
        Pitch = 28.0f;
        radius = 30.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        //ding
        Yaw = 180.0f;
        Pitch = 89.0f;
        radius = 40.0;
        at = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        //qian
        Yaw = 0.0f;
        Pitch = 51.0f;
        radius = 15.0;
        at = glm::vec3(14.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        //hou
        Yaw = 180.0f;
        Pitch = 51.0f;
        radius = 15.0;
        at = glm::vec3(-14.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        at = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 30.0;
        Yaw -= 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        at = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 30.0;
        Yaw += 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        at = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 30.0;
        Pitch -= 1.0f;
        if (Pitch < 1.0f) Pitch = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        at = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 30.0;
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


