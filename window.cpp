#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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


    // render loop - making sure window won't close until user closes it
    while(!glfwWindowShouldClose(window)) // do we need to close?
    {
        processInput(window);

        glfwPollEvents(); // any events were triggered? update accordingly
        glfwSwapBuffers(window); // double buffer cool concept - page 23
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }


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