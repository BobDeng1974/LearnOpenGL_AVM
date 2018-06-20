#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

float radius = 50.0f;
float Yaw = 90.0f;
float Pitch = 45.0f;

int main()
{
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

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------glEnable-----------------------------------
    // glEnable(GL_MULTISAMPLE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
// -------------------------------------glEnable-----------------------------------

    // build and compile shaders
    // -------------------------
    Shader modleShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
    //Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));  Car  nanosuit_little
    Model ourModel(FileSystem::getPath("resources/objects/nanosuit/Car.obj"));

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.2.coordinate_systems.vs", "6.2.coordinate_systems.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.4f,  1.0f, -0.5f,  0.0f, 0.0f, // A  HOU
         0.5f,  1.0f, -0.5f,  1.0f, 0.0f, // B
         0.5f,  0.0f, -0.5f,  1.0f, 1.0f, // C
         0.5f,  0.0f, -0.5f,  1.0f, 1.0f, // C
         0.4f,  0.0f, -0.5f,  0.0f, 1.0f, // D
         0.4f,  1.0f, -0.5f,  0.0f, 0.0f, // A

         0.4f,  1.0f,  0.5f,  0.0f, 0.0f, // E  QIAN
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f, // F
         0.5f,  0.0f,  0.5f,  1.0f, 1.0f, // G
         0.5f,  0.0f,  0.5f,  1.0f, 1.0f, // G
         0.4f,  0.0f,  0.5f,  0.0f, 1.0f, // H
         0.4f,  1.0f,  0.5f,  0.0f, 0.0f, // E

         0.4f,  0.0f,  0.5f,  1.0f, 0.0f, // H zuo
         0.4f,  0.0f, -0.5f,  1.0f, 1.0f, // D
         0.4f,  1.0f, -0.5f,  0.0f, 1.0f, // A
         0.4f,  1.0f, -0.5f,  0.0f, 1.0f, // A
         0.4f,  1.0f,  0.5f,  0.0f, 0.0f, // E
         0.4f,  0.0f,  0.5f,  1.0f, 0.0f, // H

         0.5f,  0.0f,  0.5f,  1.0f, 0.0f, // G you
         0.5f,  0.0f, -0.5f,  1.0f, 1.0f, // C
         0.5f,  1.0f, -0.5f,  0.0f, 1.0f, // B
         0.5f,  1.0f, -0.5f,  0.0f, 1.0f, // B
         0.5f,  1.0f,  0.5f,  0.0f, 0.0f, // F
         0.5f,  0.0f,  0.5f,  1.0f, 0.0f, // G

         0.4f,  1.0f, -0.5f,  0.0f, 1.0f, // A xia
         0.5f,  1.0f, -0.5f,  1.0f, 1.0f, // B
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f, // F
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f, // F
         0.4f,  1.0f,  0.5f,  0.0f, 0.0f, // E
         0.4f,  1.0f, -0.5f,  0.0f, 1.0f, // A

         0.4f,  0.0f, -0.5f,  0.0f, 1.0f, // D shang
         0.5f,  0.0f, -0.5f,  1.0f, 1.0f, // C
         0.5f,  0.0f,  0.5f,  1.0f, 0.0f, // G
         0.5f,  0.0f,  0.5f,  1.0f, 0.0f, // G
         0.4f,  0.0f,  0.5f,  0.0f, 0.0f, // H
         0.4f,  0.0f, -0.5f,  0.0f, 1.0f // D
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);



    int degree = 0;
    float distance = 8.0f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // don't forget to enable shader before setting uniforms
        modleShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        modleShader.setMat4("projection", projection);

        //glm::mat4 view = camera.GetViewMatrix();

        // glm::mat4 view = glm::lookAt(glm::vec3(-20.0f, 20.0f, 0.0f),
        //             glm::vec3(0.0f, 0.0f, 0.0f),
        //             glm::vec3(0.0f, 1.0f, 0.0f));
        // modleShader.setMat4("view", view);

        // float radius = 30.0f;
        // float camX   = sin(glfwGetTime()) * radius;
        // float camZ   = cos(glfwGetTime()) * radius;
        // glm::mat4 view = glm::lookAt(glm::vec3(camX, 30.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // modleShader.setMat4("view", view);


        float camX = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;
        float camY = sin(glm::radians(Pitch)) * radius;
        float camZ = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modleShader.setMat4("view", view);


        // render the loaded model
        glm::mat4 model = glm::mat4(1.0);
        //model = glm::translate(model, glm::vec3(0.0f, 2.0f/*-1.75f*/, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));   // it's a bit too big for our scene, so scale it down
        modleShader.setMat4("model", model);
        ourModel.Draw(modleShader);


        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model2;
        glm::mat4 view2;
        glm::mat4 projection2;

        //model2 = glm::rotate(model2, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        if(distance >15) {
            distance = 8.0f;
        }
        model2 = glm::translate(model2, glm::vec3(distance, 0.0f, 0.0f));
        distance += 0.1f;
        model2 = glm::scale(model2, glm::vec3(5.5f, 4.0f, 5.5f));
        view2  = glm::translate(view2, glm::vec3(0.0f, 0.0f, -3.0f));
        projection2 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        modleShader.setMat4("model", model2);
        modleShader.setMat4("view", view);
        ourShader.setMat4("projection", projection2);

        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //usleep(300000);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        radius -= 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        radius += 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }


    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        Yaw += 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        Yaw -= 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        Pitch -= 1.0f;
        if (Pitch < 1.0f) Pitch = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
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

// // glfw: whenever the mouse moves, this callback is called
// // -------------------------------------------------------
// void mouse_callback(GLFWwindow* window, double xpos, double ypos)
// {
//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

//     lastX = xpos;
//     lastY = ypos;

//     camera.ProcessMouseMovement(xoffset, yoffset);
// }

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// // ----------------------------------------------------------------------
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(yoffset);
// }
