//
// Created by Guglielmo Mazzesi on 02.10.24.
//

#ifndef CORE_H
#define CORE_H
/**
 Functions that computes a color along the ray
 @param current_ray Ray that should be traced through the scene
 @param recursion_level The current recursion level, used to prevent infinite ray reflection/refractivity
 @return Color intensity at the intersection point
 */
inline glm::vec3 traceRay(const Ray & current_ray, unsigned int recursion_level) {
    // If I reached too deep of a recursion, return
    if(recursion_level >= MAX_RAY_TRACING_RECURSION_LEVEL) {
        if(PRINT_MAXIMUM_RECURSION_LEVEL_REACHED)
            cout << "Reached maximum recursion level" << endl;
        return glm::vec3(0.0f);
    }

    // Initializing the possible components to the pixel's color
    glm::vec3 surface_intensity(0.0);
    glm::vec3 transparent_intensity(0.0);
    glm::vec3 reflective_intensity(0.0);
    glm::vec3 refractive_intensity(0.0);

    // Computing the first intersection
    const Interaction closest_interaction = bvh->intersect(current_ray);

    // Case in which the ray does not intersect anything
    if(!closest_interaction.hit)
        return glm::vec3(0);

    // Extracting the material from the intersected object
    const Material & surface_material = * closest_interaction.material;

    // Initializing the epsilon value used to avoid float inaccuracies
    constexpr float epsilon = 1e-4f;

    // Computing the view direction on intersection point
    const glm::vec3 incident_direction = current_ray.direction;

    // VOLUMETRIC RENDERING
    if(surface_material.type == VOLUMETRIC) {
        // Generating a new ray from the volumetric surface
        const Ray volumetric_ray {
            .origin = closest_interaction.intersection + epsilon * current_ray.direction,
            .direction = current_ray.direction,
            .current_medium_refraction_index = current_ray.current_medium_refraction_index
        };

        // Case in which the ray is leaving the volumetric medium
        if(dot(current_ray.direction, closest_interaction.normal) > 0) {
            return traceRay(current_ray, recursion_level + 1);
        }

        // Computing the distance to the next primitive
        const auto [hit, normal, intersection, uv_coordinates, distance, primitive, material] = bvh->intersect(volumetric_ray);

        // Case in which the ray is entering the volumetric medium
        const float intersection_probability = 1 - exp(-distance * surface_material.density);

        // Computing the surface intensity of the volumetric render
        const auto volume_intensity = surface_material.computeSurfaceIntensity(closest_interaction, current_ray) * intersection_probability;

        // Computing the intensity of whatever lies withing or behind the medium
        const auto wrapped_intensity = traceRay(volumetric_ray, recursion_level + 1) * (1 - intersection_probability);

        // Computing the final value
        return volume_intensity + wrapped_intensity;
    }

    // REFLECTION
    // Verifying if the object is reflective
    if(surface_material.reflectivity > 5e-2) {
        // Generating the specular direction
        glm::vec3 reflected_direction = reflect(incident_direction, closest_interaction.normal);

        // Case in which the material is perfectly glossy
        if(surface_material.glossiness == 1.0f) {
            // Creating the new ray generating from the object surface
            const Ray reflected_ray {
                .origin = closest_interaction.intersection + epsilon * reflected_direction,
                .direction = reflected_direction
            };

            // Computing the reflective intensity of the new ray
            reflective_intensity = traceRay(reflected_ray, recursion_level + 1);
        }
        // Case in which the material is not perfectly glossy
        else {
            // Scattering the ray
            for(int i = 0; i < ROUGH_SURFACES_SAMPLE_SIZE; i++) {
                // Generating the radius of the randomization sphere
                float disk_radius = 2e-1 - surface_material.glossiness * 2e-1;

                // Generating a random perturbation
                glm::vec2 random_perturbation = glm::diskRand(disk_radius);

                // Applying the perturbation
                glm::vec3 randomized_reflected_direction = normalize(
                    reflected_direction + glm::vec3(random_perturbation, 0));

                // Creating the new ray generating from the object surface
                const Ray randomized_reflected_ray {
                    .origin = closest_interaction.intersection + epsilon * randomized_reflected_direction,
                    .direction = randomized_reflected_direction
                };

                // Computing the value of this ray
                reflective_intensity += traceRay(randomized_reflected_ray, recursion_level + 1);
            }

            // Computing the mean value of the scattering
            reflective_intensity /= ROUGH_SURFACES_SAMPLE_SIZE;
        }

        // Applying the material coefficients
        reflective_intensity *= surface_material.reflectivity;
        reflective_intensity *= surface_material.reflection_filter;
    }

    // REFRACTION
    // Verifying if the object is refractive
    if(surface_material.refractivity > 5e-2) {
        // Computing the dot product between the normal and the current ray direction
        const float dot_incident_normal = dot(closest_interaction.normal, incident_direction);

        // Computing the refractivity index of the new medium
        float delta_1 = current_ray.current_medium_refraction_index;
        float delta_2;
        glm::vec3 oriented_normal;

        // Establishing if the ray is entering or leaving the medium
        if(dot_incident_normal > 0) {
            // Case in which I am leaving the current medium, by default I go back to traveling in the air
            delta_2 = 1.0f;
            oriented_normal = - closest_interaction.normal;
        }
        else {
            // Case in which I collided with a new medium, and I was travelling in the air
            delta_2 = surface_material.refraction_index;
            oriented_normal = closest_interaction.normal;
        }

        // Case in which I move between 2 medium with the same refraction index
        if(delta_1 == delta_2) {
            // Initializing the refracted ray
            Ray refracted_ray = Ray{
                .origin = closest_interaction.intersection + epsilon * incident_direction,
                .direction = incident_direction,
                .current_medium_refraction_index = delta_2
            };

            // Computing the refractive intensity
            refractive_intensity = traceRay(refracted_ray, recursion_level + 1);
        }
        // Case in which I move between 2 medium with different refraction index
        else {
            // Computing the refracted direction (library returns (0,0,0) if refraction is not possible)
            glm::vec3 sub_refracted_direction = refract(incident_direction, oriented_normal, delta_1 / delta_2);
            // Computing the reflected direction
            glm::vec3 sub_reflected_direction = reflect(incident_direction, oriented_normal);

            // Case in which Fresnel effect is active
            if(USE_FRESNEL) {
                // SCHLICK APPROXIMATION (https://link.springer.com/chapter/10.1007/978-1-4842-7185-8_9)
                // Computing the dot product between the oriented normal and the incident vector (cos incident)
                float cos_theta_incident = abs(dot_incident_normal);

                // Computing F(0)
                float F0 = pow((delta_1 - delta_2) / (delta_1 + delta_2), 2);

                // Computing the full Schlick’s approximation of the Fresnel effect
                float sub_reflection_coefficient = F0 + (1 - F0) * pow(1 - cos_theta_incident,5);
                sub_reflection_coefficient = min(1.0f, max(0.0f, sub_reflection_coefficient));
                float sub_refraction_coefficient = 1 - sub_reflection_coefficient;

                // Initializing the refracted ray
                Ray refracted_ray {
                    .origin = closest_interaction.intersection + epsilon * sub_refracted_direction,
                    .direction = sub_refracted_direction,
                    .current_medium_refraction_index = delta_2
                };

                // Initializing the reflected ray
                Ray reflected_ray {
                    .origin = closest_interaction.intersection + epsilon * sub_reflected_direction,
                    .direction = sub_reflected_direction,
                    .current_medium_refraction_index = delta_1
                };

                // Initializing the sub intensities
                auto sub_reflected_intensity = glm::vec3(0.0f);
                auto sub_refracted_intensity = glm::vec3(0.0f);

                // Adding the intensity of the sub reflected ray
                if(sub_reflection_coefficient > 1e-2) {
                    sub_reflected_intensity = traceRay(reflected_ray, recursion_level + 1)
                                            * sub_reflection_coefficient;
                }

                // Adding the intensity of the sub refracted ray
                if(sub_refraction_coefficient > 1e-2) {
                    sub_refracted_intensity = traceRay(refracted_ray, recursion_level + 1)
                                            * sub_refraction_coefficient;
                }

                // Computing the refracted intensity
                refractive_intensity = (sub_reflected_intensity + sub_refracted_intensity);
            }
            // Case in which Fresnel effect is disabled
            else {
                // Angle is within the boundaries of the critical angle: refraction is possible
                if (sub_refracted_direction.x != 0 && sub_refracted_direction.y != 0 && sub_refracted_direction.z != 0) {
                    // Initializing the refracted ray
                    Ray refracted_ray {
                        .origin = closest_interaction.intersection + epsilon * sub_refracted_direction,
                        .direction = sub_refracted_direction,
                        .current_medium_refraction_index = delta_2
                    };

                    // Computing the reflected sub intensity
                    refractive_intensity = traceRay(refracted_ray, recursion_level + 1)
                                           * surface_material.refractivity;
                }
                // Angle is greater than the critical angle: only reflection is possible
                else {
                    // Initializing the reflected ray
                    Ray reflected_ray {
                        .origin = closest_interaction.intersection + epsilon * sub_reflected_direction,
                        .direction = sub_reflected_direction,
                        .current_medium_refraction_index = delta_1
                    };

                    // Assigning the reflection value
                    refractive_intensity = traceRay(reflected_ray, recursion_level + 1);
                }
            }
        }

        // Applying the material coefficients
        refractive_intensity *= surface_material.refractivity;
        refractive_intensity *= surface_material.transmission_filter;
    }

    // TRANSPARENCY
    // if(surface_material.transparency > 5e-2) {
    //     // Generating a new ray with origin slightly shifted
    //     const Ray transparency_ray {
    //         .origin = closest_interaction.intersection + 1e-4f * current_ray.direction,
    //         .direction = current_ray.direction,
    //         .current_medium_refraction_index = current_ray.current_medium_refraction_index
    //     };
    //
    //     // Computing the transparency intensity
    //     transparent_intensity = traceRay(transparency_ray, recursion_level + 1) * surface_material.transparency;
    // }

    // SURFACE
    surface_intensity = surface_material.computeSurfaceIntensity(closest_interaction, current_ray);

    // Applying the surface material diffuse coefficient
    surface_intensity *= max(0.0f, 1 - surface_material.refractivity - surface_material.reflectivity);

    // Compute the intensity of the pixel by adding the sources
    glm::vec3 final_intensity = surface_intensity + reflective_intensity + refractive_intensity + transparent_intensity;

    if(isnan(final_intensity.x))
        return glm::vec3(0.0f);

    // Returning the sum of all the components
    return final_intensity;
}

/**
 Functions that computes the path of a photon and story it in the map
 @param current_photon Ray that should be traced through the scene
 @param recursion_level The current recursion level, used to prevent infinite ray reflection/refractivity
 */
inline void tracePhoton(const Photon & current_photon, unsigned int recursion_level) {
    // If I reached too deep of a recursion or the photon intensity is negligible, return
    if(recursion_level >= MAX_PHOTON_TRACING_RECURSION_LEVEL) {
        return;
    }

    // Computing the closest interaction
    const Interaction closest_interaction = bvh->intersect(current_photon.ray);

    // Case in which the photon ray does not intersect anything
    if(!closest_interaction.hit)
        return;

    // Initializing the epsilon value used to avoid float inaccuracies
    constexpr float epsilon = 1e-4f;

    // Photon information at the intersection point.
    const glm::vec3 intersection_point = closest_interaction.intersection;
    const glm::vec3 incident_direction = current_photon.ray.direction;

    // Extracting the material from the intersected object
    const Material & surface_material = * closest_interaction.material;

    // REFRACTION
    // Verifying if the object is refractive
    if(surface_material.refractivity > 5e-2) {
        // Computing the dot product between the normal and the current ray direction
        const float dot_incident_normal = dot(closest_interaction.normal, incident_direction);

        // Computing the refractivity index of the new medium
        float delta_1 = current_photon.ray.current_medium_refraction_index;
        float delta_2;
        glm::vec3 oriented_normal;

        // Establishing if the ray is entering or leaving the medium
        if(dot_incident_normal > 0) {
            // Case in which I am leaving the current medium, by default I go back to traveling in the air
            delta_2 = 1.0f;
            oriented_normal = - closest_interaction.normal;
        }
        else {
            // Case in which I collided with a new medium, and I was travelling in the air
            delta_2 = surface_material.refraction_index;
            oriented_normal = closest_interaction.normal;
        }

        // Corner case in which I move between 2 medium with the same refraction index
        if(delta_1 == delta_2) {
            // Initializing the refracted ray
            Ray refracted_ray {
                .origin = closest_interaction.intersection + epsilon * incident_direction,
                .direction = incident_direction,
                .current_medium_refraction_index = delta_2
            };

            // Initializing the refracted photon
            Photon refracted_photon {
                .ray = refracted_ray,
                .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
                .type = CAUSTIC,
                .intensity = current_photon.intensity * surface_material.refractivity,
            };

            // Computing the refractive intensity
            tracePhoton(refracted_photon, recursion_level + 1);
        }
        // Case in which I move between 2 medium with different refraction index
        else {
            // Computing the refracted direction (library returns (0,0,0) if refraction is not possible)
            glm::vec3 sub_refracted_direction = refract(incident_direction, oriented_normal, delta_1 / delta_2);
            // Computing the reflected direction
            glm::vec3 sub_reflected_direction = reflect(incident_direction, oriented_normal);

            // Case in which Fresnel effect is active
            if(USE_FRESNEL) {
                // SCHLICK APPROXIMATION (https://link.springer.com/chapter/10.1007/978-1-4842-7185-8_9)
                // Computing the dot product between the oriented normal and the incident vector (cos incident)
                float cos_theta_incident = abs(dot_incident_normal);

                // Computing F(0)
                float F0 = pow((delta_1 - delta_2) / (delta_1 + delta_2), 2);

                // Computing the full Schlick’s approximation of the Fresnel effect
                float sub_reflection_coefficient = F0 + (1 - F0) * pow(1 - cos_theta_incident,5);
                sub_reflection_coefficient = min(1.0f, max(0.0f, sub_reflection_coefficient));
                float sub_refraction_coefficient = 1 - sub_reflection_coefficient;

                // Initializing the refracted ray
                Ray refracted_ray {
                    .origin = closest_interaction.intersection + epsilon * sub_refracted_direction,
                    .direction = sub_refracted_direction,
                    .current_medium_refraction_index = delta_2
                };

                // Initializing the refracted photon
                Photon refracted_photon {
                    .ray = refracted_ray,
                    .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
                    .type = CAUSTIC,
                    .intensity = current_photon.intensity * sub_refraction_coefficient,
                };

                // Initializing the reflected ray
                Ray reflected_ray {
                    .origin = closest_interaction.intersection + epsilon * sub_reflected_direction,
                    .direction = sub_reflected_direction,
                    .current_medium_refraction_index = delta_1
                };

                // Initializing the reflected photon
                Photon reflected_photon {
                    .ray = reflected_ray,
                    .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
                    .type = CAUSTIC,
                    .intensity = current_photon.intensity * sub_reflection_coefficient,
                };

                // Recursive call for the refracted photon
                if(sub_refraction_coefficient > 1e-3) {
                    tracePhoton(refracted_photon, recursion_level + 1);
                }

                // Recursive call for the reflected photon
                if(sub_reflection_coefficient > 1e-3) {
                    tracePhoton(reflected_photon, recursion_level + 1);
                }
            }
            // Case in which Fresnel effect is disabled
            else {
                // Angle is within the boundaries of the critical angle: refraction is possible
                if (sub_refracted_direction.x != 0 && sub_refracted_direction.y != 0 && sub_refracted_direction.z != 0) {
                    // Initializing the refracted ray
                    Ray refracted_ray {
                        .origin = closest_interaction.intersection + epsilon * sub_refracted_direction,
                        .direction = sub_refracted_direction,
                        .current_medium_refraction_index = delta_2
                    };

                    // Initializing the refracted photon
                    Photon refracted_photon {
                        .ray = refracted_ray,
                        .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
                        .type = CAUSTIC,
                        .intensity = current_photon.intensity * surface_material.refractivity,
                    };

                    // Recursive call for the refracted photon
                    tracePhoton(refracted_photon, recursion_level + 1);
                }
                // Angle is greater than the critical angle: only reflection is possible
                else {
                    // Initializing the reflected ray
                    Ray reflected_ray {
                        .origin = closest_interaction.intersection + epsilon * sub_reflected_direction,
                        .direction = sub_reflected_direction,
                        .current_medium_refraction_index = delta_1
                    };

                    // Initializing the reflected photon
                    Photon reflected_photon {
                        .ray = reflected_ray,
                        .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
                        .type = CAUSTIC,
                        .intensity = current_photon.intensity * surface_material.refractivity,
                    };

                    // Recursive call for the reflected photon
                    tracePhoton(reflected_photon, recursion_level + 1);
                }
            }
        }
    }

    // Computing the intensity of the surface light
    const float surface_coefficient =  max(0.0f, 1 - surface_material.refractivity -
        surface_material.reflectivity);

    // If the surface intensity is too small, avoid adding the photon
    if(surface_coefficient < 1e-2)
        return;

    // SOLID SURFACES
    // Creating the photons to add to the surface
    const Photon surface_photon {
        .ray = current_photon.ray,
        .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
        .type = current_photon.type,
        .position = closest_interaction.intersection,
        .intensity = current_photon.intensity * surface_coefficient,
    };

    // Adding the photon
    switch (surface_photon.type) {
        case INDIRECT : {
            if(USE_INDIRECT_LIGHTING)
                indirect_photons.push_back(surface_photon);
            break;
        }
        case CAUSTIC : {
            if(USE_CAUSTIC)
                caustic_photons.push_back(surface_photon);
            break;
        }
    }

    // Initializing the outgoing direction
    glm::vec3 outgoing_direction  = reflect(incident_direction, closest_interaction.normal);

    // Compute the reflected intensity carried away by another photon
    glm::vec3 reflected_intensity = computeReflectedIntensity(
                                        current_photon.intensity,
                                        - incident_direction,
                                        closest_interaction,
                                        - outgoing_direction);

    // Generate a new ray with the sampled outgoing direction.
    const Ray outgoing_ray {
        .origin = intersection_point + epsilon * outgoing_direction,
        .direction = outgoing_direction,
        .current_medium_refraction_index = current_photon.ray.current_medium_refraction_index
    };

    // Create the new photon for the next recursion step.
    const Photon reflected_photon {
        .ray = outgoing_ray,
        .travelled_distance = current_photon.travelled_distance + closest_interaction.distance,
        .type = INDIRECT,
        .intensity = reflected_intensity,
    };

    // Recursively trace the photon.
    tracePhoton(reflected_photon, recursion_level + 1);
}

#endif