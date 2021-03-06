/* Modified from:
http://www.tomdalling.com/blog/category/modern-opengl/
 */

/*
 main

 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "platform.hpp"

// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/ext.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <list>

// tdogl classes
#include "tdogl/Program.h"
#include "tdogl/Camera.h"

/*
 Represents a geometry asset

 Contains everything necessary to draw arbitrary geometry:

  - shaders
  - a VBO
  - a VAO
  - the parameters to glDrawArrays (drawType, drawStart, drawCount)
 */
struct ModelAsset {
    tdogl::Program* shaders;
    GLuint vbo;
    GLuint vao;
    GLenum drawType;
    GLint drawStart;
    GLint drawCount;

    ModelAsset() :
        shaders(NULL),
        vbo(0),
        vao(0),
        drawType(GL_TRIANGLES),
        drawStart(0),
        drawCount(0)
    {}
};

/*
 Represents an instance of an `ModelAsset`

 Contains a pointer to the asset, and a model transformation matrix to be used when drawing.
 */
struct ModelInstance {
    ModelAsset* asset;
    glm::mat4 transform;

    ModelInstance() :
        asset(NULL),
        transform()
    {}
};

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow* gWindow = NULL;
tdogl::Camera gCamera;
ModelAsset gBox;
std::list<ModelInstance> gInstances;
GLfloat flyingSpeed = 1.5f;//units per second


// returns a new tdogl::Program created from the given vertex and fragment shader filenames
static tdogl::Program* LoadShaders(const char* vertFilename, const char* fragFilename) {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath(vertFilename), GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath(fragFilename), GL_FRAGMENT_SHADER));
    return new tdogl::Program(shaders);
}


// initialises the gBox global
static void LoadBoxAsset() {
    // set all the elements of 
    gBox.shaders = LoadShaders("vertex-shader.txt", "fragment-shader.txt");
    gBox.drawType = GL_TRIANGLES;
    gBox.drawStart = 0;
    gBox.drawCount = 6*2*3;
    glGenBuffers(1, &gBox.vbo);
    glGenVertexArrays(1, &gBox.vao);

    // bind the VAO
    glBindVertexArray(gBox.vao);

    // bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, gBox.vbo);

    // Make a cube out of triangles (two triangles per side)
    GLfloat vertexData[] = {
        //  X     Y     Z    Colors
        // bottom
        -1.0f,-1.0f,-1.0f,   1.0f, 0.0f, 0.0f, // Red
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,

        // top
        -1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 0.0f, // Yellow
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,

        // front
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f, 1.0f, // Cyan
         1.0f,-1.0f, 1.0f,   0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f,   0.5f, 0.0f, 1.0f, // Purple
        -1.0f, 1.0f,-1.0f,   0.5f, 0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   0.5f, 0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   0.5f, 0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   0.5f, 0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   0.5f, 0.0f, 1.0f,

        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 0.5f, 0.0f, // Green
        -1.0f, 1.0f,-1.0f,   0.0f, 0.5f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.5f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 0.5f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 0.5f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 0.5f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f,   1.0f, 0.75f, 0.8f, // Pink
         1.0f,-1.0f,-1.0f,   1.0f, 0.75f, 0.8f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.75f, 0.8f,
         1.0f,-1.0f, 1.0f,   1.0f, 0.75f, 0.8f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.75f, 0.8f,
         1.0f, 1.0f, 1.0f,   1.0f, 0.75f, 0.8f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gBox.shaders->attrib("vert"));
    glVertexAttribPointer(gBox.shaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), NULL);

    // connect the color to the "color" attribute of the vertex shader
    glEnableVertexAttribArray(gBox.shaders->attrib("color"));
    glVertexAttribPointer(gBox.shaders->attrib("color"), 3, GL_FLOAT, GL_TRUE, 6*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    
    // unbind the VAO
    glBindVertexArray(0);
}


// convenience function that returns a translation matrix
glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z) {
    return glm::translate(glm::mat4(), glm::vec3(x,y,z));
}

// convenience function that returns a scaling matrix
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z) {
    return glm::scale(glm::mat4(), glm::vec3(x,y,z));
}

// convenience function that returns a random generated translation matrix
glm::mat4 random_translate(GLfloat min, GLfloat max) {
    return translate(glm::linearRand(min, max),
                     glm::linearRand(min, max),
                     glm::linearRand(min, max));
}

// convenience function that returns a random generated scaling matrix
glm::mat4 random_scale(GLfloat min, GLfloat max) {
    return scale(glm::linearRand(min, max),
                 glm::linearRand(min, max),
                 glm::linearRand(min, max));
}

// convenience function that returns a random generated transform matrix
glm::mat4 random_transform() {
    return random_translate(-80, 80) * random_scale(0.1, 2.0);
}

// convenience function that returns a random generated instance
static void CreateRandomInstance() {
    ModelInstance instance;
    instance.asset = &gBox;
    instance.transform = random_transform();
    gInstances.push_back(instance);
}

//create all the `instance` structs for the 3D scene, and add them to `gInstances`
static void CreateInstances() {
    
    // create a lot of box instances
    int numOfBoxes = 800;
    for(int i = 0; i < numOfBoxes; i = i + 1 ){
        CreateRandomInstance();
    }
    
}


//renders a single `ModelInstance`
static void RenderInstance(const ModelInstance& inst) {
    ModelAsset* asset = inst.asset;
    tdogl::Program* shaders = asset->shaders;

    //bind the shaders
    shaders->use();

    //set the shader uniforms
    shaders->setUniform("camera", gCamera.matrix());
    shaders->setUniform("model", inst.transform);

    //bind VAO and draw
    glBindVertexArray(asset->vao);
    glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount);

    //unbind everything
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shaders->stopUsing();
}


// draws a single frame
static void Render() {
    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render all the instances
    std::list<ModelInstance>::const_iterator it;
    for(it = gInstances.begin(); it != gInstances.end(); ++it){
        RenderInstance(*it);
    }

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}


// update the scene based on the time elapsed since last update
static void Update(float secondsElapsed) {
    
    //constantly move position of camera forward
    gCamera.fly(secondsElapsed * flyingSpeed);
    
    //accelerate flying speed based on XZ keys
    const GLfloat accelerateFlyingPerSecond = 1.3f; //units per second
    if(glfwGetKey(gWindow, 'X')){
        flyingSpeed += secondsElapsed * accelerateFlyingPerSecond;
    } else if(glfwGetKey(gWindow, 'Z')){
        flyingSpeed -= secondsElapsed * accelerateFlyingPerSecond;
    }
    
    //rotate camera based on WASDQE keys
    const GLfloat pitchDegreesPerSecond = 25.0f; //degrees per second
    const GLfloat yawDegreesPerSecond   = 25.0f; //degrees per second
    const GLfloat rollDegreesPerSecond  = 25.0f; //degrees per second
    if(glfwGetKey(gWindow, 'S')){
        gCamera.pitch(secondsElapsed * -pitchDegreesPerSecond);
    } else if(glfwGetKey(gWindow, 'W')){
        gCamera.pitch(secondsElapsed * pitchDegreesPerSecond);
    }
    if(glfwGetKey(gWindow, 'A')){
        gCamera.yaw(secondsElapsed * yawDegreesPerSecond);
    } else if(glfwGetKey(gWindow, 'D')){
        gCamera.yaw(secondsElapsed * -yawDegreesPerSecond);
    }
    if(glfwGetKey(gWindow, 'Q')){
        gCamera.roll(secondsElapsed * -rollDegreesPerSecond);
    } else if(glfwGetKey(gWindow, 'E')){
        gCamera.roll(secondsElapsed * rollDegreesPerSecond);
    }
    
}

// callback for keyboard events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        // reset all the instances
        std::list<ModelInstance>::iterator it;
        for(it = gInstances.begin(); it != gInstances.end(); ++it){
            (*it).transform = random_transform();
        }
    }
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");

    // GLFW settings
    glfwSetCursorPos(gWindow, 0, 0);
    glfwMakeContextCurrent(gWindow);
    glfwSetKeyCallback(gWindow, key_callback);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialise the gBox asset
    LoadBoxAsset();

    // create all the instances in the 3D scene based on the gBox asset
    CreateInstances();

    // setup gCamera
    gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);

    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;

        // draw one frame
        Render();

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;

        //exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }

    // clean up and exit
    glfwTerminate();
}


int main(int argc, char *argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
