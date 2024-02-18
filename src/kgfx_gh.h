#ifndef KRISVERS_KGFX_GH_H
#define KRISVERS_KGFX_GH_H

#include "kgfx/kgfx.h"

#define GLFW_NO_INCLUDES
#include <GLFW/glfw3.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window);

#endif