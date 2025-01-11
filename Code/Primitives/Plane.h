//
// Created by Guglielmo Mazzesi on 10/3/2024.
//

#ifndef PLANE_H
#define PLANE_H

#include "Primitive.h"

class Plane : public Primitive {
protected:
    glm::vec3 normal = glm::vec3(0, 1, 0);

public:
    /**
     * The constructor of the plane
     * @param transform The plane transform
     * @param material The plane material
     * @param albedo
     */
    explicit Plane(const glm::mat4 & transform,
        const Material * material = nullptr,
        const Texture * albedo = nullptr)
    : Primitive(transform, material, albedo) {
        min_local_coord = glm::vec3(-INFINITY, INFINITY, INFINITY);
        max_local_coord = glm::vec3(INFINITY, INFINITY, INFINITY);
        Primitive::computeMinMaxGlobal();
    }

    /**
    * Function that verify if a given ray intersects the plane
    * @param global_ray The ray trying to intersect the plane
    * @param interaction The interaction struct
    * @return The hit struct
    */
    void Intersect(const Ray & global_ray, Interaction & interaction) override {
        // Localizing the ray
        const Ray local_ray = localizeRay(global_ray);

        // Dot products
        const float dot_normal_ray = dot(local_ray.direction, this->normal);

        // Verifying that ray and plane are not parallels
        if(abs(dot_normal_ray) < 1e-6)
            return;

        // Computing the lambda (ie ray = ray_origin + lambda * ray_direction)
        const float lambda = dot(normal, (this->local_origin - local_ray.origin)) / dot_normal_ray;

        // If lambda is negative, then intersection point is behind the ray origin
        if(lambda < 1e-6)
            return;

        interaction.hit = true;
        interaction.normal = normal;
        interaction.intersection = local_ray.origin + lambda * local_ray.direction;
        interaction.material = material;
        interaction.primitive = this;

        // De-localizing the interaction
        delocalizeInteraction(interaction, global_ray.origin);
    }


    [[nodiscard]] glm::vec3 computeTangent(glm::vec3 normal, const glm::vec3 surface_point) override {
        return normalize(inverse_transform * glm::vec4(surface_point, 1));
    }
};


#endif //Plane_h
