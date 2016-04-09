//
//  load.h
//  mp1
//
//  Created by Chia-Hao Hsieh on 4/9/16.
//  Copyright © 2016 Chia-Hao Hsieh. All rights reserved.
//

#ifndef load_h
#define load_h

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

glm::vec3 computeFaceNormal(glm::uvec3 face, std::vector<glm::vec3> vertices);
void loadObj(GLchar* path, GLfloat* vertices, int vectices_size);
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

glm::vec3 computeFaceNormal(glm::uvec3 face, std::vector<glm::vec3> vertices)
{
    glm::vec3 a = vertices.at((face)[0]);
    glm::vec3 b = vertices.at((face)[1]);
    glm::vec3 c = vertices.at((face)[2]);
    glm::vec3 normal = glm::normalize(glm::cross(c - a, b - a));
    return normal;
}

// load object file
void loadObj(GLchar* path, GLfloat* cubeVertices, int cube_vectices_size)
{
    // load file data
    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec3> faces;
    
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file ! Are you in the right path ?\n");
        getchar();
        return;
    }
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        
        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            //printf("v %f %f %f\n", vertex.x, vertex.y, vertex.z );
            vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            glm::uvec3 face;
            fscanf(file, "%d %d %d\n", &face[0], &face[1], &face[2] );
            //printf("raw f %d %d %d\n", face[0], face[1], face[2]);
            
            // file vertec index starts from 1
            // make it 0-based by subtracting 1 from all indices
            face = face - glm::uvec3(1);
            
            //printf("f %d %d %d\n", face[0], face[1], face[2]);
            faces.push_back(face);
        }else{
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    
    // If you load the vertices into a vertex position array,
    // and the triangle faces into a face array whose elements are triples of vertex indices,
    // then a loop through the faces computes the normal of each face.
    std::vector<glm::vec3> face_normals;
    for (std::vector<glm::uvec3>::const_iterator face = faces.begin(); face != faces.end(); ++face){
        glm::vec3 face_normal = computeFaceNormal((*face), vertices);
        face_normals.push_back(face_normal);
        //printf("fn %f %f %f\n", face_normal.x, face_normal.y, face_normal.z );
    }
    
    // You can then create a normal accumulator entry for every vertex and initialize it to zero.
    std::vector<glm::vec3> normals;
    
    for(int i=0; i<vertices.size(); ++i){
        normals.push_back(glm::vec3(0.0));
    }
    // Then loop through every face and add its normal to
    // the normal accumulator of each of its three vertices.
    for(int i=0; i<faces.size(); ++i){
        glm::uvec3 face = faces.at(i);
        glm::vec3 face_normal = face_normals.at(i);
        printf("face_normal length: %f\n", glm::length(face_normal));
        
        for(int j=0; j<3; j++){
            unsigned int vertex_idx = face[j];
            normals.at(vertex_idx) = normals.at(vertex_idx) + face_normal;
        }
    }
    // Then for each vertex, normalize (make unit length) its accumulated normal vector.
    for (std::vector<glm::vec3>::iterator normal = normals.begin(); normal != normals.end(); ++normal){
        (*normal) = normalize((*normal));
        //printf("length: %f\n", glm::length((*normal)));
    }
    
    
    // The attached model does not come with any texture coordinates.
    // For texture mapping, you can use cylindrical coordinates as texture coordinates.
    
    // The bowl of the teapot is a surface of revolution about the y axis.
    // Given a vertex's position (x,y,z),
    // its cylindrical coordinates would be (r cos(theta), y, r sin(theta))
    // for a radius r, an angle theta, and an altitude y,
    // where the angle theta is atan2(z,x).
    // Then you can set texture coordinates (s,t) as
    // s = (theta+PI)/2PI and t = y/ymax (since the base of the teapot is at y = 0)
    
    // You can also use simple orthographic rectilinear texture coordinates s = x, t = y
    // (or set OpenGL to texgen those coordinates automatically).
    
    
    GLfloat givenCubeVertices[] = {
        // Positions          // Normal            //Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    for(int i = 0; i < cube_vectices_size; ++i)
        cubeVertices[i] = givenCubeVertices[i];
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width,height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    return textureID;
}


// This function loads a texture from file. Note: texture loading functions like these are usually
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio).
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

#endif /* load_h */
