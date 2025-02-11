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

const char *firstFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

const char *secondFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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

    unsigned int firstFragmentShader;
    firstFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // which shader type we are using
    glShaderSource(firstFragmentShader, 1, &firstFragmentShaderSource, NULL); // compile the shader
    glCompileShader(firstFragmentShader); // compile the shader

    unsigned int secondFragmentShader;
    secondFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // which shader type we are using
    glShaderSource(secondFragmentShader, 1, &secondFragmentShaderSource, NULL); // compile the shader
    glCompileShader(secondFragmentShader); // compile the shader

    // link the shaders into a program
    unsigned int firstShaderProgram, secondShaderProgram;
    firstShaderProgram = glCreateProgram(); // create a new program
    secondShaderProgram = glCreateProgram(); // create a new program

    glAttachShader(firstShaderProgram, vertexShader); // attach vertex shader to program
    glAttachShader(firstShaderProgram, firstFragmentShader); // attach fragment shader to program
    glLinkProgram(firstShaderProgram); // link the program

    glAttachShader(secondShaderProgram, vertexShader); // attach vertex shader to program
    glAttachShader(secondShaderProgram, secondFragmentShader); // attach fragment shader to program
    glLinkProgram(secondShaderProgram); // link the program

    glGetProgramiv(firstShaderProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(secondFragmentShader, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(firstShaderProgram, sizeof(infoLog), NULL, infoLog);
        glGetProgramInfoLog(secondFragmentShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(firstShaderProgram); // use the program
    glUseProgram(secondShaderProgram); // use the program

    glDeleteShader(vertexShader); // delete the shaders as they're no longer needed
    glDeleteShader(firstFragmentShader); //
    glDeleteShader(secondFragmentShader); //


    float firstTriangle[] = {
            0.5f, -0.4f, 0.0f,
            -0.5f, -0.4f, 0.0f,
            0.0f, 0.75f, 0.0f,
    };

    float secondTriangle[] = {
            -0.5f, 0.4f, 0.0f,
            0.5f, 0.4f, 0.0f,
            0.0f, -0.75f, 0.0f,
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);


    // First triangle
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0); // enable the position attribute

    // Second triangle
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0); // enable the position attribute


    // uncomment this call to draw in wireframe polygons.
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop - making sure window won't close until user closes it
    while(!glfwWindowShouldClose(window)) // do we need to close?
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 4. draw the object
        glUseProgram(firstShaderProgram); // use the program

        glBindVertexArray(VAOs[0]); // bind the VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw a triangle with 3 vertices

        glUseProgram(secondShaderProgram); // use the program
        glBindVertexArray(VAOs[1]); // bind the VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw a triangle with 3 vertices

        glfwSwapBuffers(window); // double buffer cool concept - page 23
        glfwPollEvents(); // any events were triggered? update accordingly
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(firstShaderProgram);
    glDeleteProgram(secondShaderProgram);

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