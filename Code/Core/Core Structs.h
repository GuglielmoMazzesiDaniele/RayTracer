//
// Created by Guglielmo Mazzesi on 10/17/2024.
//

#ifndef CORE_STRUCTS_H
#define CORE_STRUCTS_H

// Classes declaration
class Primitive;
class Material;
struct Ray;

using namespace std;

/**
 Structure representing the even of hitting an object
 */
struct Interaction {
    bool hit; ///< Boolean indicating whether there was or there was no intersection with an object
    glm::vec3 normal; ///< Normal vector of the intersected object at the intersection point
    glm::vec3 intersection; ///< Surface point in global coordinates
    glm::vec2 uv_coordinates; ///< Surface point in UV coordinates
    float distance; ///< Distance from the origin of the ray to the intersection point
    Primitive * primitive; ///< A pointer to the intersected object
    const Material * material; ///< The material of the surface hit
};

/**
 Struct representing a single ray.
 */
struct Ray {
    glm::vec3 origin; ///< Origin of the ray
    glm::vec3 direction; ///< Direction of the ray

    float current_medium_refraction_index = 1; ///< Refraction index of the medium the ray is travelling in
};

/**
* Struct representing a single photon
*/
struct Photon {
    Ray ray; ///< The abstract ray the photon is travelling on

    float travelled_distance = 0.0f; ///< The distance the ray have travelled in its lifespan

    photon_type type = INDIRECT; ///< The type of the photon, used to store it in the correct data structure

    glm::vec3 position; ///< The position of the photon in global coordinates
    glm::vec3 intensity; ///< The photon intensity
};

/**
* Struct representing a vertex
*/
struct Vertex {
    const glm::vec3 * coordinates = nullptr; ///< Coordinates in object space
    const glm::vec2 * uv_coordinates = nullptr; ///< Coordinates in UV space
    glm::vec3 normal; ///< Normal in object space
    glm::vec3 tangent; ///< Tangent in object space
    glm::vec3 bitangent; ///< Bitangent in object space
};

#endif //CORE_STRUCTS_H
