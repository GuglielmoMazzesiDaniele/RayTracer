//
// Created by Guglielmo Mazzesi on 11/27/2024.
//

#ifndef DISK_H
#define DISK_H

#include "../Core/Core Structs.h"

class Disk : public Primitive {
    float radius = 1.0f;
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    Disk(const glm::mat4 & transform,
        const Material * material = nullptr,
        const Texture * albedo = nullptr)
    : Primitive(transform, material, albedo) {
        min_local_coord = glm::vec3(-radius, 0, -radius);
        max_local_coord = glm::vec3(radius, 0, radius);
        Primitive::computeMinMaxGlobal();
    }

    void Intersect(const Ray & global_ray, Interaction & interaction) override {
        // Localizing the ray
        const Ray local_ray = this->localizeRay(global_ray);

        // Computing the dot products
        const float dot_normal_ray = dot(local_ray.direction, normal);

        // Verifying that ray and disk are not parallels
        if(abs(dot_normal_ray) < 1e-6)
            return;

        // Computing the lambda (ie ray = ray_origin + lambda * ray_direction)
        const float lambda = dot(normal, (this->local_origin - local_ray.origin)) / dot_normal_ray;

        // Computing the coordinates of the intersection
        const glm::vec3 intersection = local_ray.origin + lambda * local_ray.direction;

        // If lambda is negative or the length of intersection is greater than the disk radius, return
        if(const float intersection_distance = length(intersection); lambda < 1e-6 || intersection_distance > radius)
            return;

        // Filling the hit object
        interaction.hit = true;
        interaction.normal = normal;
        interaction.intersection = intersection;
        interaction.material = material;
        interaction.primitive = this;

        // De-localizing the interaction
        delocalizeInteraction(interaction, global_ray.origin);
    }

    [[nodiscard]] glm::vec3 computeTangent(const glm::vec3 normal, const glm::vec3 surface_point) override {
        return normalize(this->global_origin - surface_point);
    }
};

#endif //DISK_H
