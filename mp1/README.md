
Demo:

1. See demo at:
https://youtu.be/15lcSltNXRk

Environment Setup:

1. Install glfw3, glew, and glw using Homebrew. Or run 'bash mp1.sh'
2. Open project in Xcode
3. In Xcode, go to:
  Project Navigator/mp1 -> TARGETS/mp1 -> Build Phases -> Link Binary with Libraries

  Click the + sign to add 'OpenGL.framework'

  Click the + sign, and then 'Add Other ...' to add '/usr/local/Cellar/glfw3/3.1.2/lib/libglfw3.3.1.dylib'

  Click the + sign, and then 'Add Other ...' to add '/usr/local/Cellar/glew/1.12.0/lib/libGLEW.1.12.0.dylib'

4. In Xcode, go to:
  Project Navigator/mp1 -> TARGETS/mp1 -> Build Phases -> Copy Files

  Select 'Destination: Products directory'

  Clear 'Subpath: ' input box

  Click the + sign to add 'mp1/shaders/shader.vs'
  Click the + sign to add 'mp1/shaders/shader.frag'

5. In Xcode, go to:
  Project Navigator/mp1 -> TARGETS/mp1 -> Build Settings -> Search Paths -> Header Search Paths
  Add '/usr/local/include'

Usage:
1. Click 'Build and Run' in Xcode
2. Click 'w' to turn on/off wireframe mode
3. Click 'Esc' to quit
