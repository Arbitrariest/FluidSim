// want to make it take in array of positions and display the particles in 3d
// can only access opengl objects by references (value), including the shaders
//TODO check whether shaders should be in shaders folder
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";

int main()
{
    //initialises glfw
    glfwInit(); 

    //context is like the collection of opengl partss, so thius tells it what version to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //use glfloat instead of normal float, but normal float not always same size as opengl vesion. 
    //every 3 floats = 1 coordinate
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 
        0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f 
    };

    GLFWwindow* window = glfwCreateWindow(800, 800, "Particle Renderer", NULL, NULL);
    //gives error if windwo fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //loads opengl functions
    gladLoadGL();

    //tells opengl where it should reder to
    glViewport(0, 0, 800, 800);

    //opengl version of unsigned int (pos int)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //pass the refernce value, tell it we only use 1 string, then point to source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //compile it, so it can be used by the gpu. needs to be compiled before we can use it
    glCompileShader(vertexShader);

    //same as with vertex, just for fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
    glCompileShader(fragmentShader);

    //will always need to use reference value to access objects
    GLuint shaderProgram = glCreateProgram();

    //as stated, attatches shaders to shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //wraps up the program, allegedly
    glLinkProgram(shaderProgram);

    //delete shaders, as they are now linked to the program, keeps it tidy
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //create batch to send to gpu, called a buffer
    //vbo, vertex buffer object
    //vbo is an array, but we only have one object so its one
    //vao stores pointer to one or more vbo, so can swap between easily

    //vbo can be used for positions and textures ect. pro tip, can store textures in stativ vbo and positions in dynamic vbo to be efficient
    GLuint VAO, VBO;

    //must always be before vbo (create container first)
    glGenVertexArrays(1, &VAO);

    //creat buffer object
    glGenBuffers(1, &VBO);

    //binding means make certain object, cur object. if use func to mod that type of obj, it changes cur obj, aka the binded obj
    glBindVertexArray(VAO);
    //use array since that is type of vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    //stream = vertices modified once, used a few times (usually once)
    //static = vertices modified once, used many many times
    //dynamic = modified multiple times, used many many times.
    //specify to improve performance
    //draw = vertices modified once, used to draw image on screen
    //read and copy are the other options
    //TODO figure out read and copy
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //configures, so opengl knows how to read the vbo
    //vertex atribute is how comunicate to vertex shader from the outisde
    //pos of attribute, values per vertex, type of data, only matters if coords as int, stride of vertices (amount of data between ech vertex), offset = pointer to where verteces begins in array (the beginning in our case)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //enables the attribute at certain pos
    glEnableVertexAttribArray(0);   

    //not mandatory, but ensure we dont change vbo, vao witha function accidentally.
    //the order is very important again (i think bind from inside to out)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //sets the colour to clear the screen to navy. last number is transparency, 1 opaque, 0 transparent
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    //tells it to use execute the command we told it to prepare for
    glClear(GL_COLOR_BUFFER_BIT);
    //swaps the back and front buffers, so we can see the cleared screen
    glfwSwapBuffers(window);


    //keeps it open until you close the window, or another function tells it to close
    while(!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //activate the shader
        glUseProgram(shaderProgram);
        //bind the vao so opengl knows we want to use that one
        glBindVertexArray(VAO);
        //to draw, specify type of primative we want to use, starting index of the vertices, and how many vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //remember to swap
        glfwSwapBuffers(window);
        //checks all events to update, else unresponsive and handles the glfw events
        glfwPollEvents();
    }


    //deletes all our objects before ending program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    //destroys window before ending program
    glfwDestroyWindow(window);
    //terminates glfw before ending program
    glfwTerminate();
    return 0;
}