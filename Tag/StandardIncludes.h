#ifndef STANDARD_INCLUDES_H
#define STANDARD_INCLUDES_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#define M_ASSERT(_cond, _msg) \
	if (!(_cond)) { OutputDebugStringA(_msg); std::abort(); glfwTerminate(); }
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Singleton.h"
#include "Resolution.h"
using namespace std;

#endif STANDARD_INCLUDES_H

