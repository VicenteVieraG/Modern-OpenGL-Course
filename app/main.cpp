#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv){
    const GLint WIDTH = 800;
    const GLint HEIGHT = 600;
    int bufferWidth;
    int bufferHeight;

    // Initialise GLFW
    if(!glfwInit()){
        std::cerr<<"GLFW initialisation failed!"<<std::endl;
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    // Core Profile means no backward compatibility for OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window object
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
    if(!mainWindow){
        std::cerr<<"GLFW window creation failed!"<<std::endl;
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set the OpenGL context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if(glewInit() != GLEW_OK){
        std::cerr<<"GLEW initialisation failed!"<<std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup the Viewport size
    // The Viewport is the part of the screen on wich OpenGL will draw
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Main loop
    do{
        // Get and handle input events
        // This handle every event in the window
        glfwPollEvents();

        // Clear window
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);

    }while(!glfwWindowShouldClose(mainWindow));

    return 0;
}