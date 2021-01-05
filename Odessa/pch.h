// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <stdexcept>
#include <vector>
#include <iostream>
#include <optional>
#include <cstdlib>
#include <set>
#include <memory>
#include <fstream>
#include <string>
#include <array>

#define WIDTH 800
#define HEIGHT 600
#define ImageCount 3

#define WIN32_LEAN_AND_MEAN
#define GLFW_EXPOSE_NATIVE_WIN32

#define GLM_FORCE_SSE42
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include <Windows.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#endif //PCH_H
