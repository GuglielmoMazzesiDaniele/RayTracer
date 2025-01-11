//
// Created by Guglielmo Mazzesi on 10/10/2024.
//

#ifndef CONE_H
#define CONE_H

class Cone : public Primitive {
    glm::vec3 normal = glm::vec3(0, 1, 0); ///< Normal of the cone (0, 1, 0)

    float cone_height = 1.0f;
    float base_radius = 1.0f;

    Plane * base; ///< The base of the cone, implemented with a plane

    /**
    * Auxiliary function that create the cone base
    */
    void createBase() const {
        // Initializing the base's transform
        const glm::mat4 T = translate(this->transform, glm::vec3(0, 1, 0));
        const glm::mat4 R = rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
        const glm::mat4 S = scale(glm::mat4(1), glm::vec3(1));
        const glm::mat4 base_transform = T * R * S;
        // Initializing the base plane
        primitives.push_back(new Disk(base_transform, this->material, this->albedo_texture));
    }

public:
    /**
     * The constructor of the cone
     * @param transform The cone's transform
     * @param material The cone's material
     * @param albedo Pointer to the albedo texture
     */
    Cone(const glm::mat4 & transform,
        const Material * material,
        const Texture * albedo)
    : Primitive(transform, material, albedo) {
        // Initializing the cone base
        createBase();

        // Generating the max and min coordinates both locally and globally
        min_local_coord = glm::vec3(-1, 0, -1);
        max_local_coord = glm::vec3(1, 1, 1);
        Primitive::computeMinMaxGlobal();
    }


    /**
     * Function that verify if a given localized_ray intersects the plane
     * @param global_ray The ray in global coordinate system
     * @return The hit object
     */
    void Intersect(const Ray & global_ray, Interaction & interaction) override {
        // Converting the localized_ray in local coordinates
        Ray localized_ray = localizeRay(global_ray);

        // Computing a (see notes)
        const float a = pow(localized_ray.direction.x, 2) + pow(localized_ray.direction.z, 2)
                        - pow(localized_ray.direction.y, 2);
        // Computing b (see notes)
        const float b = 2 * (localized_ray.origin.x * localized_ray.direction.x + localized_ray.origin.z * localized_ray.direction.z
                             - localized_ray.origin.y * localized_ray.direction.y);
        // Computing c (see notes)
        const float c = pow(localized_ray.origin.x, 2) + pow(localized_ray.origin.z, 2)
                        - pow(localized_ray.origin.y, 2);

        // Computing the components of the quadratic equation
        float radicand = pow(b, 2) - 4 * a * c;

        // Case in which the equation has no solutions (no intersections points)
        if (radicand < 0)
            return;

        // Defining lambda
        float lambda;
        // Case in which the equation has just a solution
        if (radicand == 0) {
            lambda = -b / (2 * a);
        }
            // Case in which there are multiple intersection points
        else {
            const float lambda_1 = (-b + sqrt(radicand)) / (2 * a);
            const float lambda_2 = (-b - sqrt(radicand)) / (2 * a);
            lambda = lambda_1 > lambda_2 ? lambda_2 : lambda_1;
        }

        // Verifying if lambda is positive, if it is not then the cone is behind the camera
        if (lambda < 0)
            return;

        // Computing to intersection point
        const glm::vec3 intersection_point = localized_ray.origin + lambda * localized_ray.direction;

        // Computing the value of the intersection
        float dot_intersection_normal = dot(intersection_point, normal);

        // Verifying if the intersection is behind specular or invalid
        if (dot_intersection_normal > 1 || dot_intersection_normal < 0)
            return;

        // Computing the direction of the normal using ChatGPT solution
        const glm::vec3 intersection_normal=  normalize(glm::vec3(2 * intersection_point.x,
                                                                  -2 * intersection_point.y,
                                                                  2 * intersection_point.z));

        //Filling the hit object
        interaction.hit = true;
        interaction.normal = intersection_normal;
        interaction.intersection = intersection_point;
        interaction.primitive = this;
        interaction.material = material;

        // Globalizing the hit object
        delocalizeInteraction(interaction, global_ray.origin);
    }


    [[nodiscard]] glm::vec3 computeTangent(glm::vec3 normal, const glm::vec3 surface_point) override {
        return {0,0,0};
    }
};

#endif //CONE_H
