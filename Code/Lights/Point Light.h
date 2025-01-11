//
// Created by Guglielmo Mazzesi on 12/7/2024.
//

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLight : public Light {
public:
    PointLight(const glm::mat4 & transform, const glm::vec3 color_intensity, const glm::vec3 normalized_intensity)
        : Light(transform, color_intensity) {
        // Initializing the sphere transform
        const auto sphere_transform = scale(transform, glm::vec3(0.35));

        // Creating the sphere material
        Material sphere_material;
        sphere_material.self_illuminance = normalized_intensity;
        sphere_material.transparency = 1;


        // Creating the area light sphere
        // primitives.push_back(new Sphere(sphere_transform, sphere_material));
    }
};

#endif //POINT_LIGHT_H
