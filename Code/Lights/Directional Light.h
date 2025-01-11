//
// Created by Guglielmo Mazzesi on 30.09.24.
//

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

using namespace std;

/**
* Spotlight Class
*/
class DirectionalLight : public Light {
protected:
    glm::vec3 normal = glm::vec3(0, 1, 0); ///< Direction of the light in local coordinates
    glm::vec3 tangent = glm::vec3(1, 0, 0); ///< Tangent of the light in local coordinates
    glm::vec3 bitangent = glm::vec3(0, 0, 1); ///< Bitangent of the light in local coordinates

public:

    float aperture; ///< Aperture of the Spotlight

    glm::vec3 global_normal = glm::vec3(0.0f); ///< Direction of the light in global coordinates
    glm::vec3 global_tangent = glm::vec3(0.0f); ///< Tangent of the light in global coordinates
    glm::vec3 global_bitangent = glm::vec3(0.0f); ///< Bitangent of the light in global coordinates

    /**
    * Default constructor
    * @param transform Transform of the spotlight
    * @param intensity Color of the light
    * @param aperture Aperture of the spotlight, expressed in degrees
    */
    DirectionalLight(const glm::mat4 & transform, const glm::vec3 & intensity, const float aperture)
        : Light(transform, intensity) {
        // Initializing aperture
        this->aperture = cos(glm::radians(aperture));

        // Initializing global light normal
        this->global_normal = transform * glm::vec4(normal, 0);

        // Initializing global light tangent
        this->global_tangent = transform * glm::vec4(tangent, 0);

        // Initializing global light bitangent
        this->global_bitangent = transform * glm::vec4(bitangent, 0);
    }

    /**
    * Function that return the intensity of a light on a given surface point
    * @param surface_point The surface point in global coordinates
    */
    glm::vec3 computeRadiance(const glm::vec3 & surface_point) override {
        // Initializing the light ray
        Ray light_ray {
            .origin = this->global_origin,
            .direction = normalize(surface_point - this->global_origin),
        };

        // Verifying if the ray is occluded
        if(isLightOccluded(surface_point))
            return {0, 0, 0};

        // Localizing the surface point
        const glm::vec3 localized_surface_point = this->inverse_transform * glm::vec4(surface_point, 1);

        // Computing dot product between the surface point and the light direction
        const float dot_light_surface = dot(normalize(localized_surface_point), normal);

        // If the surface is behind the spotlight
        if(dot_light_surface < 0)
            return {0, 0, 0};

        // Computing attenuation
        const float attenuation = computeAttenuation(surface_point);

        // Return zero intensity if the surface point is outside the spotlight aperture
        return  dot_light_surface >= aperture? this->intensity * dot_light_surface * attenuation
                    : glm::vec3(0, 0, 0);
    }

};

#endif
