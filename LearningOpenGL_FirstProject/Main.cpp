#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


float vertices[] = {
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // top right
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // bottom left
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f      // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // callback which tells the OpenGL that the window has been recised so the viewPort should be changed
{
    glViewport(0, 0, width, height);
}
int createOpenGLWindow(GLFWwindow*& win)
{
    glfwInit(); // Init the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // init the glfw with some values
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for macOS

    win = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); // create a window
    if (win == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // glad is doing the dirty work of identifying some stuff that is directly used by my video card
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600); // this tells the OpenGL how big the render is
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback); // this will call the callback function to change the viewPort

    return 0;
}
void setUpTheTriangleVAO(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO)
{
    glGenBuffers(1, &VBO); // creating space for the Vertex Buffer Object (VBO) is used for fast access to the memory where the objects are stores
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // this is just something that tells the VAO where the vertecies start or end and how to draw them
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // this will tell the openGL how to interpret the vertices values and how to look at them
    // 1. 0 - is the starting location of the position in array, in the shader it's position is (location=0)
    // 2. 3 - the ammount of values we store, which is vec3 that has 3 values
    // 3. tells which type the value is, which is float
    // 4. weather the data should be normalized, in our case - no
    // 5. determines the stride (how many steps we need to take to get to the same value in the list), could be 0 to make OpenGL determine how long the stride is only when the values are tightly packed together
    // 6. this is just an offset with a weird cast
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // the 3 * sizeof(float) is just an offset of 3 values after the start of the stride
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
int main()
{
    GLFWwindow* window;
    int resultCode = createOpenGLWindow(window);
    if (resultCode != 0) { return resultCode; }

    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;
    setUpTheTriangleVAO(VBO, VAO, EBO);

    Shader ourShader("shaders/simple.vs", "shaders/simple.fs");

    stbi_set_flip_vertically_on_load(true);
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("dog.jpg", &width, &height, &nrChannels, 0);
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

    while (!glfwWindowShouldClose(window)) // this is a very simple draw loop
    {
        ourShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // this checks the events (keyboard, mouse, recising, etc.)
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate(); // terminated the glfw and cleans it
    return 0;
}