#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// tdogl classes
#include "Texture.h"

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    tdogl::Texture* gTexture = NULL;
    
    /*  Functions  */
    // Constructor
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, tdogl::Texture* gTexture)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->gTexture = gTexture;
        
        // Now that we have all the required data, set the vertex buffers and its attribute pointers.
        this->setupMesh();
    }
    
    // Render the mesh
    void Draw(Shader shader)
    {
        // Bind texture
        glActiveTexture(GL_TEXTURE0); // Active proper texture unit before binding
        
        // bind the texture and set the "tex" uniform in the fragment shader
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, gTexture->object());
        //gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
        
        // Now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.Program, "tex", 0);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, gTexture->object());
        //glBindTexture(GL_TEXTURE_2D, 0);
        
        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;
    
    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);
        
        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
        
        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        
        glBindVertexArray(0);
    }
};
