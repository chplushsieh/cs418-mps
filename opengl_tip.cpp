
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OS X

int width, height;
glfwGetFramebufferSize(window, &width, &height);
glViewport(0, 0, width, height);
