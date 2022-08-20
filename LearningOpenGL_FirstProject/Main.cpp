#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// very simple vertexShader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor; // the color variable has attribute position 1\n"
"out vec3 vertexColor; // specify a color output to the fragment shader\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   vertexColor = aColor; // set ourColor to the input color we got from the vertex data\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(vertexColor, 1.0);\n"
"}\0";

float vertices[] = {
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f   // top left 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // the 3 * sizeof(float) is just an offset of 3 values after the start of the stride
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
unsigned int compileBoringShaders()
{
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

    return shaderProgram;
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

    unsigned int shaderProgram = compileBoringShaders();

    while (!glfwWindowShouldClose(window)) // this is a very simple draw loop
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // this checks the events (keyboard, mouse, recising, etc.)
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); // terminated the glfw and cleans it
    return 0;
}