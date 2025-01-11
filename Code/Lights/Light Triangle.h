//
// Created by Guglielmo Mazzesi on 12/11/2024.
//

#ifndef LIGHT_TRIANGLE_H
#define LIGHT_TRIANGLE_H

/**
* Class representing a light emitting triangle
*/
class LightTriangle : public Triangle {

    glm::vec3 intensity; ///< The intensity of the light emitted by the triangle

    void createLight() const {
        // Initializing epsilon
        const float epsilon = 5e-3;

        // TODO: Fix bug
        const glm::vec3 correct_normal = isnan(this->normal.x) ? glm::vec3(0, 1, 0) : this->normal;

        // Creating the position of the light in world space
        const auto light_translation = this->getCentroid() + epsilon * correct_normal;

        // Creating the transform of the light
        const glm::mat4 T = translate(glm::mat4(1), light_translation);
        const glm::mat4 R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
        const glm::mat4 S = scale(glm::mat4(1), glm::vec3(1));
        const auto light_transform = T * R * S;

        // Adding a point light at the center of the triangle
        lights.push_back(new Light(light_transform, this->intensity));
    }

public:
    /**
    * Constructor that initialized the triangle without the vertices normals
    * @param transform The triangle transform
    * @param material The triangle material
    * @param albedo Pointer to the albedo texture
    * @param intensity The triangle light intensity
    * @param vertices 3 sized array of vertices
    * @param phong_shading A boolean indicating weather or not use the phong shading
    */
    LightTriangle(const glm::mat4 & transform,
        const glm::vec3 intensity,
        const Vertex vertices [3],
        const bool phong_shading = false,
        const Material * material = nullptr,
        const Texture * albedo = nullptr
    )
    : Triangle(transform, vertices, phong_shading, material, albedo),
        intensity(intensity) {
        // Creating the lights
        createLight();
    }

    /**
    *  Function that verifies if a global ray intersects the triangle
    *  @param global_ray Ray in global coordinates
    *  @param interaction The interaction struct that will be filled with all the data
    */
    void Intersect(const Ray & global_ray, Interaction & interaction) override {
        // Computing the intersection
        computeIntersection(global_ray, interaction);
    }
};

#endif //LIGHT_TRIANGLE_H
