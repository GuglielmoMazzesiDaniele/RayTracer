//
// Created by Guglielmo Mazzesi on 02.10.24.
//

#ifndef IMPORT_H
#define IMPORT_H

// C++ libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
#include <vector>
#include <iomanip>
#include <cmath>
#include <functional>
#include <algorithm>
#include <map>
#include <omp.h>
#include <random>
#include <queue>
#include <limits>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/norm.hpp"

// STB
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// High level settings
#include "Static Data/Enums.h"
#include "Static Data/Settings.h"
#include "Static Data/Forward Declaration.h"

// Auxiliary
#include "Auxiliary/Printing.h"
#include "Auxiliary/Random.h"
#include "Auxiliary/Math.h"

// Colors
#include "Static Data/Colors.h"

// Noise
#include "Noise/Perlin Noise.h"

// Core libraries
#include "Core/Core Structs.h"
#include "Core/Entity.h"
#include "Core/Scene.h"
#include "Core/Material.h"
#include "Core/Camera.h"

// Materials
#include "Materials/Marble.h"

// Shared data, like materials
#include "Static Data/Resolutions.h"
#include "Static Data/Materials.h"

// Image Processing
#include "Core/Image.h"

// Texture
#include "Texture/Texture.h"

// SDS
#include "SDS/BVH.h"
#include "Photon Mapping/Photon KDTree.h"

// Primitives implementation
#include "Primitives/Primitive.h"
#include "Primitives/Plane.h"
#include "Primitives/Triangle.h"
#include "Primitives/Disk.h"
#include "Primitives/Cone.h"
#include "Primitives/Sphere.h"

// Lights
#include "Lights/Light.h"
#include "Lights/Point Light.h"
#include "Lights/Directional Light.h"
#include "Lights/Gaussian Directional Light.h"
#include "Lights/Area Light.h"

// Light Primitives
#include "Lights/Light Triangle.h"

// BRDFs
#include "BRDFs/BRDFs.h"

// Tracing
#include "Tracing/Tracer.h"

// Bounding Box
#include "Bounds/Bounding Box 3D.h"

// Photon Mapping
#include "Photon Mapping/Photon Mapping.h"

// Participating Media
#include "Participating Media/Partecipating Media.h"
#include "Participating Media/Fog.h"

// Mesh implementation
#include "Mesh/Mesh.h"
#include "Mesh/Sphere.h"
#include "Mesh/Plane.h"
#include "Mesh/Perlin Sphere.h"
#include "Mesh/Perlin Terrain.h"
#include "Mesh/Mesh Builders.h"

// Vaporwave
#include "Vaporwave/Chessboard Plane.h"

// Scenes
#include "Scenes/Photon Mapping Scene.h"
#include "Scenes/Default Scene.h"
#include "Scenes/USI Competition Scene.h"

#endif //IMPORT_H