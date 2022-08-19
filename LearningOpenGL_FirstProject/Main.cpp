#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // callback which tells the OpenGL that the window has been recised so the viewPort should be changed
{
    glViewport(0, 0, width, height);
}
int main()
{
    glfwInit(); // Init the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // init the glfw with some values
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for macOS

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); // create a window
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // glad is doing the dirty work of identifying some stuff that is directly used by my video card
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600); // this tells the OpenGL how big the render is
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // this will call the callback function to change the viewPort

    while (!glfwWindowShouldClose(window)) // this is a very simple draw loop
    {
        glfwSwapBuffers(window);
        glfwPollEvents(); // this checks the events (keyboard, mouse, recising, etc.)
    }

    glfwTerminate(); // terminated the glfw and cleans it
    return 0;
}