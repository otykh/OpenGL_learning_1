#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// very simple vertexShader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // callback which tells the OpenGL that the window has been recised so the viewPort should be changed
{
    glViewport(0, 0, width, height);
}
int main()
{
    float vertices[] = { // test triangle vertices 
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

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

    unsigned int VBO;
    glGenBuffers(1, &VBO); // creating space for the Vertex Buffer Object (VBO) is used for fast access to the memory where the objects are stores

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // this will tell the openGL how to interpret the vertices values and how to look at them
    // 1. 0 - is the starting location of the position in array, in the shader it's position is (location=0)
    // 2. 3 - the ammount of values we store, which is vec3 that has 3 values
    // 3. tells which type the value is, which is float
    // 4. weather the data should be normalized, in our case - no
    // 5. determines the stride, could be 0 to make OpenGL determine how long the stride is only when the values are tightly packed together
    // 6. this is just an offset with a weird cast
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // creating the vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // pass the shader source
    glCompileShader(vertexShader); // and compile the shader

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // the next few lines will check weather the shader compiled successfully with no errors
    int  success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); // this will create a shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // we delete the shaders because they are already stored in the shaderProgram but also we do not need them in the future
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window)) // this is a very simple draw loop
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents(); // this checks the events (keyboard, mouse, recising, etc.)
    }

    glfwTerminate(); // terminated the glfw and cleans it
    return 0;
}