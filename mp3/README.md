The code is modified from <http://learnopengl.com/>

# Environment

- OSX El Capitan Version 10.11.3
- Xcode 7.2.1
- C++
- External Libraries Used: GLFW3, GLEW, GLM, SOIL

# Setup

1. Install glfw3, glew, and glm using Homebrew. Or run 'sh install_glx.sh'
2. Install SOIL by following the instructions in 'install_soil.sh'
3. Open project in Xcode
4. In Xcode, go to:
  Project Navigator/mp3 -> TARGETS/mp3 -> Build Phases -> Link Binary with Libraries
     * Click the + sign to add 'OpenGL.framework'
     * Click the + sign, and then 'Add Other ...' to add '/usr/local/Cellar/glfw3/3.1.2/lib/libglfw3.3.1.dylib'
     * Click the + sign, and then 'Add Other ...' to add '/usr/local/Cellar/glew/1.12.0/lib/libGLEW.1.12.0.dylib'
     * Click the + sign, and then 'Add Other ...' to add '(path to cloned SOIL repo in Step 2)/libSOIL/libSOIL.dylib'

5. In Xcode, go to:
  Project Navigator/mp3 -> TARGETS/mp3 -> Build Phases -> Copy Files
     * Select 'Destination: Products directory'
     * Clear 'Subpath: ' input box
     * Don't check 'Copy only when installing'
         * Click the + sign to add 'mp3/teapot_0.obj' and check 'Copy Sign On Copy' on the right
         * Repeat the previous step to add all these shader files:
            - advanced.frag
            - advanced.vs
            - light.frag      
            - light.vs
            - skybox.frag     
            - skybox.vs
         * Repeat the previous step to add all these image files:
            - container.png
            - top.jpg
            - bottom.jpg
            - front.jpg
            - back.jpg
            - right.jpg
            - left.jpg

5. In Xcode, go to:
  Project Navigator/mp3 -> TARGETS/mp3 -> Build Settings -> Search Paths:

     * Add '/usr/local/include' to Header Search Paths
     * Add '/opt/local/include/SOIL' to Header Search Paths
     * Add '/opt/local/lib' to Library Search Paths


# Usage

1. Click 'Build and Run' in Xcode
2. <kbd>A</kbd><kbd>W</kbd><kbd>S</kbd><kbd>D</kbd> to move
3. <kbd>mouse</kbd> to change view angle
4. <kbd>Esc</kbd> to quit

# Features

* Blinn-Phong lighting model with gamma correction and simple attenuation in the fragment shader
* One light source (as a white cube)

# Demo
See Youtube video here: <https://youtu.be/8ecdJMq8wG8>
