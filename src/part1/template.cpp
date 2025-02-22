#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"
#include <../includes/shader_s.h>
#include <iostream>
#include <vector>
#include <string>
#include <exception>

#include <../includes/glm/glm/glm.hpp>
#include <../includes/glm/glm/gtc/matrix_transform.hpp>
#include <../includes/glm/glm/gtc/type_ptr.hpp>
#include <../includes/MyError.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void createTexture(const char* imgPath, unsigned int &textureID, bool should_flip);
void createGPUComponents(unsigned int &VBO, unsigned int &VAO, unsigned int &EBO,
                         const std::vector<float> &vertices, const std::vector<unsigned int> &indices);
void renderLoop(GLFWwindow *window, Shader ourShader, unsigned int &texture1, unsigned int &texture2, unsigned int &VAO);

GLFWwindow* createWindow(int width, int height);
void checkForWindowError(GLFWwindow *window);

int main()
{
    GLFWwindow* window = createWindow(800, 600);

    try {
        checkForWindowError(window);
    } catch (const MyError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    Shader ourShader("../src/shader.vs", "../src/shader.fs");

    std::vector<float> vertices = {
            // Positions        // Texture Coords
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right 0
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right 1
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left 2
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f // top left 3
    };

    std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };

    unsigned int VBO, VAO, EBO;
    createGPUComponents(VBO, VAO, EBO, vertices, indices);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;

    createTexture("../resources/container.jpg", texture1, true);
    createTexture("../resources/awesomeface.png", texture2, false);


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // render loop
    // -----------
    renderLoop(window, ourShader, texture1, texture2, VAO);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
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

GLFWwindow* createWindow(int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // use version 3 - what we want according to book
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // use version 3 - what we want according to book
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile - only modern features are supported
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // only needed for mac

    // Create a window object - width, height, name, ignore for now
    return glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
}

void createTexture(const char* imgPath, unsigned int &textureID, bool should_flip) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(should_flip); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;  // Use correct format
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

void createGPUComponents(unsigned int &VBO, unsigned int &VAO, unsigned int &EBO,
                         const std::vector<float> &vertices, const std::vector<unsigned int> &indices) {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void renderLoop(GLFWwindow *window, Shader ourShader, unsigned int &texture1, unsigned int &texture2, unsigned int &VAO) {

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.2f, -0.2f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID,"transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // draw first container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // second matrix
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

        // draw second container
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void checkForWindowError(GLFWwindow *window) {
    if (window == NULL)
    {
        glfwTerminate();
        throw MyError("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // init glad before calling any OpenGL function as required
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw MyError("Failed to initialize GLAD");
    }
}