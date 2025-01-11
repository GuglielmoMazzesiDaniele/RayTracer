//
// Created by Guglielmo Mazzesi on 12/3/2024.
//

#ifndef VAPORWAVE_PLANE_H
#define VAPORWAVE_PLANE_H
#include "../Static Data/Materials.h"

class ChessboardPlane : public Plane {
public:
    ChessboardPlane(
        const glm::mat4 & transform,
        const Material * material)
    : Plane(transform, material){}

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

        // Computing the intersection point
        const glm::vec3 local_intersection = local_ray.origin + lambda * local_ray.direction;

        // Extracting the integer part of x and z coordinates
        const int unit_x = static_cast<int>(floor(local_intersection.x));
        const int unit_z = static_cast<int>(floor(local_intersection.z));

        // Initializing the new material of the plane
        Material * plane_material;

        // Case in which I intersected a white square
        if((unit_x + unit_z) % 2 == 0)
            plane_material = & grey_material;
        // Case in which I intersected a black square
        else
            plane_material = & black_reflective_material;

        // Filling the interaction struct
        interaction.hit = true;
        interaction.normal = normal;
        interaction.intersection = local_ray.origin + lambda * local_ray.direction;
        interaction.material = plane_material;
        interaction.primitive = this;

        // De-localizing the interaction
        delocalizeInteraction(interaction, global_ray.origin);
    }
};

#endif //VAPORWAVE_PLANE_H
