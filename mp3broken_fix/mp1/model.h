#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

// tdogl classes
#include "Texture.h"
#include "Bitmap.h"

class Model
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    Model(GLchar* path)
    {
        this->loadModel(path);
    }
    
    // Draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
        this->mesh.Draw(shader);
    }
    
private:
    /*  Model Data  */
    Mesh mesh;
    string directory;
    tdogl::Texture* gTexture = NULL;
    
    /*  Functions   */
    // Loads a model and store the resulting mesh
    void loadModel(string path)
    {
        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));
        
        // Data to fill
        vector<Vertex> vertices;
        vector<GLuint> indices;
        
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
                vertices.push_back(vertex);
            }else if ( strcmp( lineHeader, "f" ) == 0 ){
                GLuint vertexIndex[3];
                
                fscanf(file, "%d %d %d\n", &vertexIndex[1], &vertexIndex[2], &vertexIndex[3] );
                indices.push_back(vertexIndex[0]);
                indices.push_back(vertexIndex[1]);
                indices.push_back(vertexIndex[2]);
        }
            
        //TODO set texture
        // loads the file "hazard.png" into gTexture
        static void LoadTexture() {
            tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(ResourcePath("hazard.png"));
            bmp.flipVertically();
            gTexture = new tdogl::Texture(bmp);
        }
        
        // Return a mesh object created from the extracted mesh data
        this.mesh = Mesh(vertices, indices, gTexture);
    }
