
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OS X

int window_width, window_height;
glfwGetFramebufferSize(window, &window_width, &window_height);
glViewport(0, 0, window_width, window_height);
