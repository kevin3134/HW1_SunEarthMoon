#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Sphere.h"
#include "Shader.h"

#include <vector>
#include <iostream>
#include "Sphere.h"

// GLU lib, could use gluSphere(), no need to draw VAO (GLU does not work in mobile)
// need windows.h before glu lib, glu lib is old and not suitable for android
//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//projection
glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
//model
glm::mat4 modelMatrix(1.0f);
// timing
float deltaTime = 0.001f;	// time between current frame and last frame
float lastFrame = 0.0f;
//center point
GLfloat origin[3] = { 0, 0, 0 };
GLfloat angle = 0;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    
    // input shaders and initiate sphere
    Shader sunShader("../shader/sun.vs", "../shader/sun.fs");
    int sunProgram = sunShader.ID;
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(sunProgram, "vertexPosition_modelspace");
    Sphere sun;
    sun.init(vertexPosition_modelspaceID,1.5f);

    Shader earthShader("../shader/earth.vs", "../shader/earth.fs");
    int earthProgram = earthShader.ID;
    GLuint vertexPosition_modelspaceID2 = glGetAttribLocation(earthProgram, "vertexPosition_modelspace");
    Sphere earth;
    earth.init(vertexPosition_modelspaceID2, 0.6f);

    Shader moonShader("../shader/earth.vs", "../shader/moon.fs");
    int moonProgram = moonShader.ID;
    GLuint vertexPosition_modelspaceIDmoon = glGetAttribLocation(moonProgram, "vertexPosition_modelspace");
    Sphere moon;
    moon.init(vertexPosition_modelspaceIDmoon, 0.2f);


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.f, 0.f, 0.f, 2.f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw sun
        sunShader.use();
        GLuint projMatrixID = glGetUniformLocation(sunProgram, "projMatrix");
        GLuint mvMatrixID = glGetUniformLocation(sunProgram, "mvMatrix");
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(mvMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glm::mat4 view = glm::mat4(1.0f); 
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        GLuint viewID = glGetUniformLocation(sunProgram, "view");
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
        sun.draw(GL_LINE_LOOP);

        //draw earth
        earthShader.use();
        GLfloat rEarthSun = 5.f;
        GLuint earthprojMatrixID = glGetUniformLocation(earthProgram, "projMatrix");
        glUniformMatrix4fv(earthprojMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLuint earthmvMatrixID = glGetUniformLocation(earthProgram, "mvMatrix");
        float xEarthSun = sin(glfwGetTime()*0.1) * rEarthSun;
        float yEarthSun = cos(glfwGetTime()*0.1) * rEarthSun;
        glm::mat4 tempmodel = glm::translate(modelMatrix, glm::vec3(xEarthSun, yEarthSun, 0.f));
        glUniformMatrix4fv(earthmvMatrixID, 1, GL_FALSE, glm::value_ptr(tempmodel));

        glm::mat4 earthview = glm::mat4(1.0f); 
        earthview = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        GLuint earthviewID = glGetUniformLocation(earthProgram, "view");
        glUniformMatrix4fv(earthviewID, 1, GL_FALSE, glm::value_ptr(earthview));
        earth.draw(GL_LINE_LOOP);

        //draw moon
        moonShader.use();
        GLfloat rEarthMoon = 1.5f;
        GLuint moonprojMatrixID = glGetUniformLocation(moonProgram, "projMatrix");
        glUniformMatrix4fv(moonprojMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLuint moonmvMatrixID = glGetUniformLocation(moonProgram, "mvMatrix");
        float xEarthMoon = sin(glfwGetTime()) * rEarthMoon;
        float yEarthMoon = cos(glfwGetTime()) * rEarthMoon;
        tempmodel = glm::translate(tempmodel, glm::vec3(xEarthMoon, yEarthMoon, 0.f));
        glUniformMatrix4fv(moonmvMatrixID, 1, GL_FALSE, glm::value_ptr(tempmodel));

        glm::mat4 moonview = glm::mat4(1.0f); 
        moonview = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        GLuint moonviewID = glGetUniformLocation(moonProgram, "view");
        glUniformMatrix4fv(moonviewID, 1, GL_FALSE, glm::value_ptr(moonview));
        moon.draw(GL_LINE_LOOP);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}