//
// Created by Guglielmo Mazzesi on 12/17/2024.
//

#ifndef MARBLE_H
#define MARBLE_H

class MarbleMaterial : public Material {
public:

    glm::vec3 veins_color; ///< Color of the veins in the marble surface
    glm::vec3 filling_color; ///< Color of the filling area in the marble surface

    /**
    * Getter for the material specular at the given surface point
    * @param surface_point  The surface point
    */
    [[nodiscard]] glm::vec3 getDiffuse(const glm::vec3 surface_point) const override {
        // Initializing values
        float noise = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float octaves = 6;

        // Combining multiple octaves of Perlin noise
        // TODO: implement this in Perlin noise header
        for (int i = 0; i < octaves; i++) {
            noise += amplitude * PerlinNoise::getInstance()->Noise(
                frequency * surface_point.x,
                frequency * surface_point.y,
                frequency * surface_point.z);

            amplitude *= 0.5f;
            frequency *= 2.0f;
        }

        // Functions coefficients
        constexpr float veins_scale = 0.01;

        // Applying the sin function
        float marble_noise = sin(veins_scale * surface_point.y +  noise);

        // Mapping the marble noise value to range [0, 1]
        marble_noise = (marble_noise + 1) * 0.5f;

        // Remapping with an exponential function
        marble_noise = pow(marble_noise, 15.0f);

        // Interpolate between the colors
        return SmoothLerp(filling_color, veins_color, marble_noise);
    }

    /**
    * Getter for the material specular at the given surface point
    * @param surface_point  The surface point
    */
    [[nodiscard]] glm::vec3 getSpecular(const glm::vec3 surface_point) const override {
        return this->specular;
    }

};

#endif //MARBLE_H
