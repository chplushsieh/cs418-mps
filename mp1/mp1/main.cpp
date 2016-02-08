#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for OS X
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    
    // Build and compile our shader program
    Shader ourShader("shader.vs", "shader.frag");
    
    // Set up vertex data (and buffer(s)) and attribute pointers
//    GLfloat vertices[] = {
//        // Positions         // Colors
//        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
//        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
//        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
//    };
    
    GLfloat vertices[] = {
        // Positions         // Colors
        -0.6f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
        -0.6f, -0.6f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
        -0.6f,  0.6f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
        -0.6f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
        
        -0.2f, -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, // Yellow
        -0.2f, -0.6f, 0.0f,  1.0f, 1.0f, 0.0f, // Yellow
        -0.2f,  0.6f, 0.0f,  1.0f, 1.0f, 0.0f, // Yellow
        -0.2f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f, // Yellow
        
        0.2f, -1.0f, 0.0f,  0.0f, 1.0f, 1.0f, // Cyan
        0.2f, -0.6f, 0.0f,  0.0f, 1.0f, 1.0f, // Cyan
        0.2f,  0.6f, 0.0f,  0.0f, 1.0f, 1.0f, // Cyan
        0.2f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f, // Cyan
        
        0.6f, -1.0f, 0.0f,  0.5f, 0.0f, 1.0f, // Purple
        0.6f, -0.6f, 0.0f,  0.5f, 0.0f, 1.0f, // Purple
        0.6f,  0.6f, 0.0f,  0.5f, 0.0f, 1.0f, // Purple
        0.6f,  1.0f, 0.0f,  0.5f, 0.0f, 1.0f  // Purple
    };
    GLuint indices[] = {
        0, 1, 4,
        1, 4, 5,
        4, 5, 8,
        5, 8, 9,
        8, 9, 12,
        9, 12,13,
        
        5, 6, 9,
        6, 9, 10,
        
        2, 3, 6,
        3, 6, 7,
        6, 7, 10,
        7, 10,11,
        10,11,14,
        11,14,15
    };
    
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0); // Unbind VAO
    
    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw the triangle
        ourShader.Use();
        
        // Create transformations
        glm::mat4 transform;
        transform = glm::translate(transform, glm::vec3(0.3f, -0.3f, 0.0f));
        transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        
        // Get matrix's uniform location and set matrix
        GLint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
