//
// Created by Guglielmo Mazzesi on 11/15/2024.
//

#ifndef GAUSSIAN_SPOTLIGHT_H
#define GAUSSIAN_SPOTLIGHT_H

class GaussianDirectionalLight : public DirectionalLight {

public:
    /**
    * Default constructor
    * @param transform Transform of the spotlight
    * @param color Color of the light
    * @param aperture Aperture of the spotlight, expressed in degrees
    */
    GaussianDirectionalLight(const glm::mat4 & transform, const glm::vec3 & color, float aperture)
        : DirectionalLight(transform, color, aperture) {
    }

    /**
    * Function that return the intensity of a light on a given surface point
    * @param surface_point The surface point in global coordinates
    */
    glm::vec3 computeRadiance(const glm::vec3 & surface_point) override {
        // Initializing the returned intensity
        constexpr auto zero_intensity = glm::vec3(0);

        // Localizing the surface point
        const glm::vec3 localized_surface_point = this->inverse_transform * glm::vec4(surface_point, 1);

        // Computing dot product between the surface point and the light direction
        const float dot_light_surface = dot(normalize(localized_surface_point), normal);

        // If the surface is behind the spotlight
        if(dot_light_surface < 0 || dot_light_surface < aperture)
            return zero_intensity;

        // Variables for the normal distribution
        const float mean = 1.0f;
        const float variance = 0.2f;

        // Computing the coefficient using standard distribution
        float distribution_coefficient = exp(-(pow(dot_light_surface - mean, 2) / (2.0f * variance)));

        // Additional safety net
        distribution_coefficient = max(1.0f, distribution_coefficient);

        // Computing the intensity based on the normal distribution
        glm::vec3 intensity = this->intensity * distribution_coefficient;

        // Returning the intensity
        return intensity;
    }
};

#endif //GAUSSIAN_SPOTLIGHT_H
