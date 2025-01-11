//
// Created by Guglielmo Mazzesi on 12/11/2024.
//

#ifndef FORWARD_DECLARATION_H
#define FORWARD_DECLARATION_H

// Classes
class Light;
class DirectionalLight;
class Primitive;
class Camera;
class Plane;
class BVH;

// Structs
struct Ray;
struct Interaction;
struct KDTreeNode;

// Functions
glm::vec3 BlinnPhong(const Interaction & interaction, const Ray & ray);
glm::vec3 CookTorrance(const Interaction & interaction, const Ray & camera_ray);

#endif //FORWARD_DECLARATION_H
