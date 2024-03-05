#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void CreateTriangle(GLuint* VAO, GLuint* VBO);
void CompileShaders(GLuint& shader);
void AddShader(GLuint& theProgram, const char* shaderCode, GLenum shaderType);

// Vertex Shader
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main(){                                                    \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
}";

// Fragment Shader
static const char* fShader = "                      \n\
#version 330                                        \n\
                                                    \n\
out vec4 colour;                                    \n\
                                                    \n\
void main(){                                        \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);              \n\
}";

int main(int argc, char** argv){
    const GLint WIDTH = 800;
    const GLint HEIGHT = 600;

    GLuint VAO;
    GLuint VBO;
    GLuint shader;
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

    // Configuration Process
    CreateTriangle(&VAO, &VBO);
    CompileShaders(shader);

    // Main loop
    do{
        // Get and handle input events
        // This handle every event in the window
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // The drawing
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);

    }while(!glfwWindowShouldClose(mainWindow));

    return 0;
}

void CreateTriangle(GLuint* VAO, GLuint* VBO){
    // Triangle vertices
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Create the VAO and VBO
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    // Set the value for the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the VAO. This sets how the VBO data should be interpreted
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Unbind the current VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CompileShaders(GLuint& shader){
    // Actually store the shader program into the GPU
    shader = glCreateProgram();

    if(!shader){
        std::cerr<<"Error Creating Shader Program!"<<std::endl;
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    // Debug variables.
    // This section handles the validation and debug configuration for
    // the shader program.
    // Because the shader program is stored, compiled and linked in the GPU
    // the debug process is hard without this configuration.
    GLint result = 0;
    GLchar eLog[1024] = {0};

    // This creates the actual executables in the GPU, links all the program
    // together and makes sure everything is working correctly
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cerr<<"Error Linking Program: "<<eLog<<std::endl;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cerr<<"Error Validating Program: "<<eLog<<std::endl;
        return;
    }
}

void AddShader(GLuint& theProgram, const char* shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetProgramiv(theShader, GL_COMPILE_STATUS, &result);

    if(!result){
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cerr<<"Error Compiling the "<<shaderType<<" Shader: "<<eLog<<std::endl;
        return;
    }

    glAttachShader(theProgram, theShader);
    
    return;
}