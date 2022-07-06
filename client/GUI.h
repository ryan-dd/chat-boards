#ifndef SAMPLE_CLASS_H
#define SAMPLE_CLASS_H

#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"
#include <GL/glew.h> 
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

class GUI
{
public:
  static void start();
private:
  static GLFWwindow* initGraphics();
  static void shutdownGraphics(GLFWwindow* window);
  static void handleGraphicsOnLoopStart();
  static void handleGraphicsOnLoopEnd(GLFWwindow* window);
};

#endif // SAMPLE_CLASS_H
