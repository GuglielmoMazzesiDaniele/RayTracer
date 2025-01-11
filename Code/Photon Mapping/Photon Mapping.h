//
// Created by Guglielmo Mazzesi on 12/14/2024.
//

#ifndef PHOTON_H
#define PHOTON_H

inline void traceIndirectPhotons() {
    // Iterate over each directional light representing a sample of the area light
    for (auto & light : directional_lights) {

    }
};

inline void traceCausticsPhotons() {
    PrintStartingProcess("tracing of caustics photons");

    // Storing the starting time for KD Tree building
    clock_t current_time = clock();

    // Iterating all primitives looking for refractive and reflective materials
    for(const auto & primitive : primitives) {
        // Extracting e the surface material
        const Material & primitive_material = primitive->getMaterial();

        // If the material is either refractive or reflective, shoot photons towards primitive
        if(primitive_material.refractivity > 0 || primitive_material.reflectivity > 0) {
            for (auto & light : directional_lights) {
                // Constants used to generate photons
                constexpr int polar_increment = 10;
                constexpr int radius_samples_amount = 10;

                // Computing the total amount of photons emitted
                constexpr int photons_amount = (360 / polar_increment) * radius_samples_amount;

                // Computing starting direction of all rays
                const glm::vec3 starting_vector = primitive->global_origin - light->global_origin;

                // Computing normal of the plane having origin in the primitive and normal the normalized
                // vector going from the origin to the light source
                const glm::vec3 normal = normalize(light->global_origin - primitive->global_origin);

                // Choosing a reference vector (x-axis or z-axis)
                glm::vec3 reference (1.0f, 0.0f, 0.0f);

                // Handling the case where the normal is close to the reference
                if (glm::abs(dot(normal, reference)) > 0.99f) {
                    // Switch to z-axis if the normal is nearly parallel to x-axis
                    reference = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                // Computing the tangent using Gram-Schmidt
                const auto tangent = normalize(reference - dot(reference, normal) * normal);

                // Computing the bitangent as the cross product of the normal and tangent
                const auto bitangent = glm::normalize(glm::cross(normal, tangent));

                // Initializing the tangent space transformation matrix
                const glm::mat3 tangent_to_world (tangent, bitangent, normal);

                // Initializing the world to tangent space transformation matrix (since it is orthonormal it's just
                // the transpose)
                const glm::mat3 world_to_tangent = glm::transpose(tangent_to_world);

                // Computing an approximation of the primitive size
                const auto primitive_diagonal = primitive->getWorldSpaceBoundingBox().getDiagonal();
                const float primitive_size = max(primitive_diagonal.x,
                    max(primitive_diagonal.y, primitive_diagonal.z)) / 2.0f;

                // Generating the photons
                for(int current_radius = 1; current_radius <= radius_samples_amount; current_radius++) {
                    for(int current_polar = 0; current_polar < 360; current_polar += polar_increment) {
                        // Computing the radians value of current polar angle
                        const float current_polar_radians = glm::radians(static_cast<float>(current_polar));

                        // Computing the normalized value of the current radius
                        const float current_radius_normalized = static_cast<float>(current_radius)
                                                                    / static_cast<float>(radius_samples_amount);

                        // Computing the coordinates of the current perturbance
                        glm::vec3 current_perturbance (
                            current_radius_normalized * glm::cos(current_polar_radians) * primitive_size * 1.25,
                            0,
                            current_radius_normalized * glm::sin(current_polar_radians) * primitive_size * 1.25
                            );

                        // Applying the world to tangent space transformation matrix
                        current_perturbance = world_to_tangent * current_perturbance;

                        // Computing the new direction
                        const glm::vec3 photon_direction = normalize(starting_vector + current_perturbance);

                        // Computing the photon intensity
                        const glm::vec3 photon_intensity = light->getLightIntensity()
                                                            / static_cast<float>(photons_amount);

                        // Initializing the photon ray
                        const Ray photon_ray{
                            .origin = light->getGlobalOrigin() + 1e-4f * light->global_normal,
                            .direction = photon_direction,
                            .current_medium_refraction_index = 1.0f
                        };

                        // Initializing the photon with adjusted intensity
                        const Photon current_photon{
                            .ray = photon_ray,
                            .intensity = photon_intensity
                        };

                        // Tracing the photon through the scene
                        tracePhoton(current_photon, 0);
                    }
                }
            }
        }
    }

    // Printing execution time
    PrintExecutionTime(clock() - current_time, "trace the caustics photons");

    // Printing the amount of caustics photons
    cout << "Amount of caustics photons : " << caustic_photons.size() << endl;
};

inline void photonMapping() {

};

#endif //PHOTON_H