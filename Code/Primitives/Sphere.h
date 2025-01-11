//
// Created by Guglielmo Mazzesi on 02.10.24.
//

#ifndef Sphere_h
#define Sphere_h
#include <complex>

constexpr float SPHERE_RADIUS = 1;

/**
 Implementation of the class Object for sphere shape.
 */
class Sphere : public Primitive {

protected:
    void computeIntersection(const Ray & global_ray, Interaction & interaction) {
        // Converting the ray in local coordinates
        Ray localized_ray = localizeRay(global_ray);

        // Computing vector c (see notes)
        const glm::vec3 c = this->local_origin - localized_ray.origin;

        // Computing vector a (see notes)
        const float a_magnitude = dot(c, localized_ray.direction);

        // Computing distance D (see notes)
        const float d_magnitude = sqrt(pow(length(c), 2) - pow(a_magnitude, 2));

        // Case in which the ray does not intersect the sphere OR the sphere is fully behind the camera
        if(d_magnitude > SPHERE_RADIUS || a_magnitude < - SPHERE_RADIUS) {
            return;
        }

        // Initializing the intersection
        glm::vec3 intersection_point;

        // Computing intersection point
        // Case in which the ray is tangent to the sphere
        if(d_magnitude == SPHERE_RADIUS) {
            intersection_point = localized_ray.direction * a_magnitude + localized_ray.origin;
        } else {
            // Case in which there are 2 intersection points
            // Computing b (see documentation)
            const float b_magnitude = sqrt(SPHERE_RADIUS * SPHERE_RADIUS - pow(d_magnitude, 2));

            // Computing the intersection points
            const glm::vec3 intersection_1 = localized_ray.direction * a_magnitude
                                             - localized_ray.direction * b_magnitude + localized_ray.origin;
            const glm::vec3 intersection_2 = localized_ray.direction * a_magnitude
                                             + localized_ray.direction * b_magnitude + localized_ray.origin;

            // Computing the distance between the ray origin and the intersections
            const float distance_1 = glm::distance(localized_ray.origin, intersection_1);
            const float distance_2 = glm::distance(localized_ray.origin, intersection_2);

            // Computing if the intersection points are in front of the camera
            const bool isFirstIntersectionInFront = dot(localized_ray.direction,
                                                        intersection_1 - localized_ray.origin) > 0;
            const bool isSecondIntersectionInFront = dot(localized_ray.direction,
                                                         intersection_2 - localized_ray.origin) > 0;

            // Storing the closest intersection
            if (isFirstIntersectionInFront && isSecondIntersectionInFront) {
                // Case in which both intersections are in front of the ray (store closest)
                intersection_point = distance_1 < distance_2 ? intersection_1 : intersection_2;
            } else if (isFirstIntersectionInFront) {
                // Case in which only first intersection is in front of the ray
                intersection_point = intersection_1;
            } else if (isSecondIntersectionInFront) {
                // Case in which only second intersection is in front of the ray
                intersection_point = intersection_2;
            } else {
                // Additionally security measure
                return;
            }
        }

        // Computing azimuthal and polar angle of the intersection point
        const float azimuthal = atan2(intersection_point.y, intersection_point.x);
        const float polar = acos(intersection_point.z);

        // Computing the UV coordinates of intersection point
        const glm::vec2 uv_coordinates (azimuthal / (2 * M_PI) + 0.5f, polar / M_PI);

        // Computing the normal
        glm::vec3 normal = intersection_point;

        // Computing the tangent (obtain from first order derivative of UV coordinates)
        const glm::vec3 tangent (- intersection_point.y, intersection_point.x, 0);

        // Computing the bitangent using cross product between normal and tangent
        const glm::vec3 bitangent = normalize(cross(normal, tangent));

        // If the normal map is present, use it to apply perturbance to the normal
        if(this->normal_map) {
            // Extracting the value of the normal map at the current UV coordinates
            const glm::vec3 normal_map_value = normal_map->getPixel(uv_coordinates);

            // Converting the RGB value into a directional vector
            glm::vec3 perturbed_normal (2 * normal_map_value.x - 1,
                2 * normal_map_value.y - 1, 2 * normal_map_value.z - 1);

            // Converting the perturbed normal from tangent space to object space using the tangent space basis
            perturbed_normal = normalize(perturbed_normal.x * tangent +
                perturbed_normal.y * bitangent +
                        perturbed_normal.z * normal);

            // Assigning the new normal to the intersection normal
            normal = perturbed_normal;
        }

        // Updating the intersection struct
        interaction.hit = true;
        // Assigning the intersection
        interaction.intersection = intersection_point;
        // Since the sphere has radius 1, the normal is equal to the intersection point
        interaction.normal = intersection_point;
        // Assigning the UV coordinates of the surface point
        interaction.uv_coordinates = uv_coordinates;
        // Saving a reference to this sphere
        interaction.primitive =  this;
        // Assigning the material
        interaction.material = material;

        // Globalizing the interaction
        delocalizeInteraction(interaction, global_ray.origin);
    }

public:
    /**
     * The constructor of the sphere
     * @param transform Transform of the sphere
     * @param material Material of the sphere
     * @param albedo Pointer to the albedo texture
     * @param normal Pointer to the normal map
     * @param AO_R_M Pointer to the ambient occlusion/roughness/metallix texture

     */
    explicit Sphere(const glm::mat4 & transform,
        const Material * material = nullptr,
        const Texture * albedo = nullptr,
        const Texture * normal = nullptr,
        const Texture * AO_R_M = nullptr)
    : Primitive(transform, material, albedo, normal, AO_R_M) {
        min_local_coord = glm::vec3(-1, -1, -1);
        max_local_coord = glm::vec3(1, 1, 1);
        Primitive::computeMinMaxGlobal();
    }

    /** Implementation of the intersection function*/
    void Intersect(const Ray & global_ray, Interaction & interaction) override {
        // Computing the intersection
        computeIntersection(global_ray, interaction);
    }


    [[nodiscard]] glm::vec3 computeTangent(const glm::vec3 normal, const glm::vec3 surface_point) override {
        // Initializing the y-axis
        constexpr glm::vec3 y_axis (0, 1, 0);

        // Computing the tangent
        return normalize(y_axis - dot(y_axis, normal) * normal);
    }
};

#endif //Sphere_h
