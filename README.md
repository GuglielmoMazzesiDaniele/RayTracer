# Ray Tracer

This repository contains a ray tracer implemented from scratch in **C++**, leveraging the **GLM (OpenGL Mathematics)** 
library for mathematical operations.

The project began as a series of assignments during the **2024 edition of the *Computer Graphics* course** as part of 
the **Bachelor of Informatics** at [USI](https://www.usi.ch/en/education/bachelor/informatics). Over time, I expanded its capabilities, ultimately leading to my victory
in the **4th edition of USI’s Rendering Competition**.

Below is the final rendering I submitted for the competition.

[![Watch the video](Images/Rendering%20Competition%20Still.png)]([https://www.youtube.com/watch?v=dQw4w9WgXcQ](https://www.youtube.com/watch?v=noWANu9wKBg&ab_channel=GuglielmoMazzesi))

## Features

### **Basic Features**

As of **January 30, 2025**, the ray tracer includes the following core functionalities:

- **Primitive Shapes**: Supports rendering of spheres, cones, planes, cylinders, and triangles.
- **Light Sources**: Supports all basic type of light sources, including:
  - **Point Lights**
  - **Directional Lights**
  - **Area Lights**
- **Reflection & Refraction**: Implements both reflection and refraction to achieve realistic light interactions.
- **Meshes**: Supports parsing and rendering of **OBJ** files along with associated **MTL** files.
- **Stochastic Ray Tracing**: Enables effects such as:
    - Depth of field
    - Semi-gloss surfaces (e.g., opaque mirrors)
    - Adaptive super-sampling anti-aliasing

### **Advanced Features**

In addition to the basics, the ray tracer includes several advanced techniques:

- **Bounding Volume Hierarchy (BVH)**
    - Implements **Surface Area Heuristic (SAH)**-based BVH for efficient ray-object intersection tests.
    - Inspired by *Chapter 4* of [Physically Based Rendering (PBR)](https://pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration) (3rd edition).

- **Perlin Noise**
    - Full implementation based on the original paper: ["An Image Synthesizer"](https://dl.acm.org/doi/10.1145/280811.280986).
    - Additional refinements from ["Improving Noise"](https://dl.acm.org/doi/10.1145/566654.566636).
    - Used for procedural textures, normal maps, and displacement mapping.



- **Cook-Torrance BRDF Model**
    - Replaces the **Blinn-Phong shading model** for more physically accurate materials.
    - Default distribution set to GGX. 
    - Supports anisotropic reflections using anisotropic GGX distribution.

- **Texturing Pipeline**
    - Integrates high-quality textures from [Poly Haven](https://polyhaven.com/).
    - Supports multiple texture types, including:
        - **Albedo**
        - **AORM (Ambient Occlusion, Roughness, Metallic)**
        - **Displacement Maps**
        - **Normal Maps**

- **Photon Mapping**
    - Implements **global illumination techniques** based on ["A Practical Guide to Global Illumination using Photon Maps"](https://graphics.stanford.edu/courses/cs348b-01/course8.pdf).
    - Allows for accurate **caustic rendering**, especially for glass objects.

- **Participating Media (Volume Rendering)**
    - Only simulates basic **light absorption in volumetric media**.
    - Supports both **simple primitives** and **complex mesh-based volumes**.

## Installation & Usage

🔧 **Requirements**:
- C++17 (or later)
- GLM (OpenGL Mathematics)
- A C++ compiler (e.g., GCC, Clang, MSVC)

### **Compilation**

To compile the project, use:

```sh
g++ -std=c++17 -O3 -fopenmp -o raytracer main.cpp
