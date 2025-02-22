#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // use version 3 - what we want according to book
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // use version 3 - what we want according to book
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile - only modern features are supported
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // only needed for mac

    // Create a window object - width, height, name, ignore for now
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad before calling any OpenGL function as required
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // specify viewport size and position
    glViewport(0, 0, 800, 600);

    // view port should be adjusted when the user adjust window size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // which shader type we are using

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // compile the shader
    glCompileShader(vertexShader); // compile the shader

    // check if compilation was successful
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // which shader type we are using
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // compile the shader
    glCompileShader(fragmentShader); // compile the shader

    // link the shaders into a program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); // create a new program

    glAttachShader(shaderProgram, vertexShader); // attach vertex shader to program
    glAttachShader(shaderProgram, fragmentShader); // attach fragment shader to program
    glLinkProgram(shaderProgram); // link the program

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram); // use the program

    glDeleteShader(vertexShader); // delete the shaders as they're no longer needed
    glDeleteShader(fragmentShader); //


    float vertices[] = {
            0.5f, -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            0.0f, 0.5f, 0.0f,
    };

    unsigned int VBO, VAO; // vertex buffer object - sends many vertices to gpu when told to
    glGenVertexArrays(1, &VAO); // creates new object
    glGenBuffers(1, &VBO); // creates new object

    // 1. bind vertex Array Object (VAO)
    glBindVertexArray(VAO); // bind the VAO

    // 2. copy our vertices array to the buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // send vertices to GPU

    // 3. set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0); // enable the position attribute

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop - making sure window won't close until user closes it
    while(!glfwWindowShouldClose(window)) // do we need to close?
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 4. draw the object
        glUseProgram(shaderProgram); // use the program
        glBindVertexArray(VAO); // bind the VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw a triangle with 3 vertices
        glBindVertexArray(0); // unbind the VAO

        glfwSwapBuffers(window); // double buffer cool concept - page 23
        glfwPollEvents(); // any events were triggered? update accordingly
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // update viewport with the new width and height
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // check if user pressed escape key to close the window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}