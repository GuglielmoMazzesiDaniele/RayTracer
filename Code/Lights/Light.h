//
// Created by Guglielmo Mazzesi on 11/15/2024.
//

#ifndef LIGHT_H
#define LIGHT_H

/**
 PointLight class
 */
class Light : public Entity {
protected:
    glm::vec3 intensity; ///< Color/intensity of the light source

    /**
    * Function that verifies if a given light ray is occluded while trying to reach a specific surface point
    */
    [[nodiscard]] bool isLightOccluded(const glm::vec3 & surface_point) const {
        if constexpr (!USE_OCCLUSION)
            return false;

        // Initializing the light ray
        const Ray light_ray {
            .origin = this->global_origin,
            .direction = normalize(surface_point - this->global_origin),
        };

        // Epsilon
        constexpr float epsilon = 1e-3f;

        // Computing the distance between the ray origin and the target point
        const float distance = glm::distance(light_ray.origin, surface_point + epsilon * (-light_ray.direction));

        // Computing the closest intersection
        Interaction tentative_hit = bvh->intersectNoTransparentWithinDistance(light_ray, distance);

        // Verifying if there was a hit
        if(tentative_hit.hit && tentative_hit.distance <= distance)
            return true;

        // Case with no occlusion
        return false;
    }

public:
    /**
    * Constructor of Point Light
    */
    Light(const glm::mat4 & transform, const glm::vec3 color_intensity)
        : Entity(transform), intensity(color_intensity){

    }

    /**
     * Getter of the light intensity
     */
    glm::vec3 getLightIntensity() const {
        return this->intensity;
    }

    /**
    * Computed the attenuation between the light and the given surface point
    * @param surface_point The surface point expressed as a vec3
    * @return A float in range [0,1] indicating how attenuated is the light
    */
    virtual float computeAttenuation(const glm::vec3 & surface_point) {
        // Case in which the attenuation is disabled
        if constexpr (!USE_LIGHT_ATTENUATION)
            return 1;

        // Computing distance between surface point and light origin
        const float distance_light_surface = length(surface_point - this->global_origin);

        // Computing the attenuation
        const float attenuation = 1.0f / pow(max(distance_light_surface, 1.0f), 2);

        // Returning the attenuation
        return attenuation;
    }

    /**
    * Function that return the intensity of a light on a given surface point
    * @param surface_point The surface point in global coordinates
    */
    virtual glm::vec3 computeRadiance(const glm::vec3 & surface_point) {
        // Verifying if the light is occluded
        if(isLightOccluded(surface_point))
            return {0, 0, 0};

        return this->intensity * computeAttenuation(surface_point);
    }
};

#endif //LIGHT_H
