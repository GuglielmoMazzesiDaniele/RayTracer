//
// Created by Guglielmo Mazzesi on 10/17/2024.
//

#ifndef BRDFS_H
#define BRDFS_H

using namespace std;

inline float Isotropic_GGX(const float roughness, const float dot_half_normal) {
    // Handle edge case for perfectly smooth surface
    if (roughness == 0.0f) {
        return (abs(dot_half_normal) <= 9e-4f) ? 1.0f : 0.0f;
    }

    // Auxiliary variables
    const float pow_roughness = pow(roughness, 2);
    const float den_auxiliary = pow(dot_half_normal, 2);

    // Computing denominator
    return pow_roughness / (M_PI * pow(den_auxiliary * (pow_roughness - 1) + 1, 2));
}

inline float Anisotropic_GGX(const float tangent_intensity, const float bitangent_intensity,
                                 const glm::vec3 & tangent, const glm::vec3 & bitangent, const glm::vec3 & half_vector,
                                 const float dot_half_normal) {
    // Auxiliary variables
    const float tangent_intensity_squared = pow(tangent_intensity, 2);
    const float bitangent_intensity_squared = pow(bitangent_intensity, 2);
    const float dot_tangent_half = dot(tangent, half_vector);
    const float dot_bi_tangent_half = dot(bitangent, half_vector);
    const float dot_half_normal_squared = pow(dot_half_normal, 2);

    // Computing the distribution value
    const float distribution = 1.0f /
                                pow(pow(dot_tangent_half, 2) / tangent_intensity_squared
                                    + pow(dot_bi_tangent_half, 2) / bitangent_intensity_squared
                                    + dot_half_normal_squared, 2);

    // Computing the normalization
    const float normalization = 1.0f / (M_PI * tangent_intensity * bitangent_intensity);

    return normalization * distribution;
}

/**
* Computed the absorbed energy using the Cook Torrance BRDF
* @param light_intensity The intensity of the light at the intersection point
* @param incoming_direction The direction of the light at the given point
* @param interaction The interaction at the given point, containing information like materials and such
* @param outgoing_direction The outgoing direction
*/
inline glm::vec3 computeReflectedIntensity(const glm::vec3 & light_intensity, const glm::vec3 & incoming_direction,
    const Interaction & interaction, const glm::vec3 & outgoing_direction) {
    // Epsilon
    constexpr float epsilon = 1e-6f;

    // Extracting vital information
    const Material * surface_material = interaction.material;
    const glm::vec3 & normal = interaction.normal;

    // The final color
    glm::vec3 final_color (0.0f);

    // Case in which the current light does not impact the visibility of the closest surface hit
    if(light_intensity.x <= epsilon && light_intensity.y <= epsilon && light_intensity.z <= epsilon)
        return final_color;

    // ESSENTIAL COMPUTATIONS
    // Computing the half vector
    const glm::vec3 half_vector = normalize(incoming_direction + outgoing_direction);

    // DOT PRODUCTS
    // Computing the dot product between surface normal and light direction
    const float dot_normal_light = dot(incoming_direction, normal);
    // Preventing lights to affect internal side of the object
    if(dot_normal_light < 0)
        return final_color;
    // Computing the dot product between view and normal
    const float dot_view_normal = dot(outgoing_direction, normal);
    // Computing dot product between half vector and normal
    const float dot_half_normal = dot(half_vector, normal);
    // Computing the dot product between half vector and view
    const float dot_half_view = dot(half_vector, outgoing_direction);

    // NORMAL DISTRIBUTION

    // Initializing the micro facet distribution value
    float D;

    // Extracting the primitive roughness
    const float roughness = interaction.primitive->getRoughness(interaction.uv_coordinates);

    // Case in which the material is not isotropic
    if(surface_material->anisotropy != 0) {
        // Computing the second vector of the tangent space's basis (tangent)
        const glm::vec3 tangent = interaction.primitive->computeTangent(interaction.normal, interaction.intersection);

        // Computing the third vector of the tangent space's basis (bi tangent)
        const glm::vec3 bi_tangent = normalize(cross(normal, tangent));

        // Computing the anisotropy coefficients
        const float anisotropy_tangent = max(0.1f, min(1.0f,
            roughness * (1 + surface_material->anisotropy)));
        const float anisotropy_bitangent = max(0.1f, min(1.0f,
            roughness * (1 - surface_material->anisotropy)));

        D = Anisotropic_GGX(anisotropy_tangent, anisotropy_bitangent,
            tangent, bi_tangent, half_vector, dot_half_normal);
    }
    // Case in which the material is isotropic
    else {
        D = Isotropic_GGX(roughness, dot_half_normal);
    }

    //GEOMETRIC ATTENUATION
    // Computing the geometric attenuation value
    float G = min(1.0f,
                  min(2 * dot_half_normal * dot_view_normal / dot_half_view,
                  2 * dot_half_normal * dot_normal_light / dot_half_view));

    // FRESNEL EFFECT
    // Computing F0
    float F0 = pow(surface_material->refractivity - 1, 2) / pow(surface_material->refractivity + 1, 2);

    // Computing the full Schlick’s approximation of the Fresnel effect
    float F = F0 + (1 - F0) * pow((1 - dot_half_view),5);

    // Computing the coefficient for the specular component
    float specular_intensity = max(0.0f, D * G * F / (4 * dot_normal_light * dot_view_normal));
    float diffuse_intensity = 1 - specular_intensity;

    // Extracting the material diffuse and specular
    const auto diffuse = interaction.primitive->getDiffuse(interaction.uv_coordinates);
    const auto material_specular = surface_material->getSpecular(interaction.intersection);

    // Adding current light contribution to the final color
    final_color.x += light_intensity.x
                        * dot_normal_light
                        * (diffuse_intensity * diffuse.x
                            + specular_intensity * material_specular.x);
    final_color.y += light_intensity.y
                        * dot_normal_light
                        * (diffuse_intensity * diffuse.y
                            + specular_intensity * material_specular.y);
    final_color.z += light_intensity.z
                        * dot_normal_light
                        * (diffuse_intensity * diffuse.z
                            + specular_intensity * material_specular.z);

    return final_color;
}

/** Function for computing color of an object according to the Cook-Torrance model. Currently using Blinn-Pong
 * distribution and Schlick’s approximation for Fresnel effect.
 @param interaction A point belonging to the object for which the color is computer
 @param ray The ray
*/
inline glm::vec3 CookTorrance(const Interaction & interaction, const Ray & ray) {
    // Initializing the color of the pixel
    glm::vec3 surface_intensity(0.0);

    // Extracting the references to some variables
    const Material & surface_material = * interaction.material;
    const glm::vec3 & surface_point = interaction.intersection;

    // SELF - ILLUMINATION
    // Computing RED channel
    surface_intensity.x += surface_material.self_illuminance.x;
    // Computing GREEN channel
    surface_intensity.y += surface_material.self_illuminance.y;
    // Computing BLU channel
    surface_intensity.z += surface_material.self_illuminance.z;

    // Extracting potential occlusion from the texture
    const float ambient_occlusion = interaction.primitive->getAmbientOcclusion(interaction.uv_coordinates);

    // AMBIENT
    // Computing RED channel
    surface_intensity.x += surface_material.ambient.x * AMBIENT_LIGHT.x * ambient_occlusion;
    // Computing GREEN channel
    surface_intensity.y += surface_material.ambient.y * AMBIENT_LIGHT.y * ambient_occlusion;
    // Computing BLU channel
    surface_intensity.z += surface_material.ambient.z * AMBIENT_LIGHT.z * ambient_occlusion;

    // Initializing direct light intensity
    glm::vec3 direct_intensity (0.0f);

    // DIFFUSE LIGHTS

    for(auto & current_light : lights){
        // Computing the light contribution
        const auto light_intensity = computeReflectedIntensity(
                current_light->computeRadiance(surface_point),
                glm::normalize(current_light->getGlobalOrigin() - surface_point),
                interaction,
                - ray.direction);

        // Adding it to the direct intensity value
        direct_intensity += light_intensity;
    }

    // DIRECTIONAL LIGHTS
    for(auto & current_light : directional_lights){
        // Computing the light contribution
        const auto light_intensity = computeReflectedIntensity(
                current_light->computeRadiance(surface_point),
                glm::normalize(current_light->getGlobalOrigin() - surface_point),
                interaction,
                - ray.direction);

        // Adding it to the direct intensity value
        direct_intensity += light_intensity;
    }

    // Applying direct light
    surface_intensity += direct_intensity;

    // PHOTONS CONTRIBUTION
    if (USE_PHOTON_MAPPING) {
        // INDIRECT LIGHTING
        if(USE_INDIRECT_LIGHTING) {
            // Static coefficients
            constexpr int max_photons = 150;
            constexpr float smoothing_kernel_sigma = 0.2f;

            // Find the nearest photons to the interaction point using the KD-tree
            std::vector<Photon> nearest_photons = getNearestNeighbors(indirect_photons_root, interaction.intersection, max_photons);

            // Compute the sphere radius as the mean value of the distance of all photons (attenuating outliers)
            float sphere_radius = 0.0f;
            for (const auto & photon : nearest_photons) {
                sphere_radius += glm::distance(photon.position, interaction.intersection);
            }
            // Averaging the radius of the sphere
            sphere_radius /= nearest_photons.size(); // Average radius

            // Computing the normalization based on sphere volume
            const float intensity_normalization = 1.0f / nearest_photons.size();

            // Initialize indirect intensity
            glm::vec3 indirect_intensity(0.0f);

            // Process the closest N photons
            for (const auto & [photon_ray, travelled_distance, type, position, intensity] : nearest_photons) {
                // Compute the distance to the current photon
                float photon_distance = glm::distance(position, interaction.intersection);

                // Compute the smoothing coefficient
                const float weight = exp(- (photon_distance * photon_distance) /
                                         (2 * smoothing_kernel_sigma * smoothing_kernel_sigma));

                // Compute the photon intensity
                const glm::vec3 photon_intensity = computeReflectedIntensity(
                    intensity,
                    -photon_ray.direction,
                    interaction,
                    -ray.direction
                ) * weight;

                // Add the photon contribution to the indirect lighting
                indirect_intensity += photon_intensity * intensity_normalization;
            }

            // Compute the length of direct and indirect light in vector space
            float direct_light_intensity = length(direct_intensity);
            float indirect_light_intensity = length(indirect_intensity);

            // Compute the attenuation factors
            float indirect_attenuation = glm::clamp(indirect_light_intensity /
                (indirect_light_intensity + direct_light_intensity), 0.0f, 1.0f);

            // Interpolate between direct and indirect light based on their values
            surface_intensity += indirect_intensity * indirect_attenuation;
        }

        // CAUSTICS
        if(USE_CAUSTIC) {
            // Static coefficients
            constexpr int max_photons = 60;
            constexpr float alpha = 0.918;
            constexpr float beta = 1.953;

            // Find the nearest photons to the interaction point using the KD-tree
            std::vector<Photon> nearest_photons = getNearestNeighbors(caustic_photons_root, interaction.intersection, max_photons);

            // Initializing the sphere radius
            float sphere_radius = 0.0f;

            // Computing the sphere radius
            for (const auto & [ray, travelled_distance, type, position, intensity] : nearest_photons) {
                // Computing the distance between the intersection and the current photon
                const float photon_distance = glm::distance(position, interaction.intersection);

                // If the photon distance is greater than the maximum distance, update the sphere radius
                if(sphere_radius < photon_distance)
                    sphere_radius =  photon_distance;
            }

            // Computing the normalization based on size of the sphere
            const float sphere_area = M_PI * sphere_radius * sphere_radius;
            const float intensity_normalization = min(80.0f, 1.0f / sphere_area);

            // Initialize indirect intensity
            glm::vec3 caustic_intensity(0.0f);

            // Process the closest N photons
            for (const auto & [photon_ray, travelled_distance, type, position, intensity] : nearest_photons) {
                // Compute the distance to the current photon
                float photon_squared_distance = distance2(position, interaction.intersection);

                // Computing the weight of the current photon
                const float photon_weight = alpha * (1 -
                    (1 - exp(-beta * (photon_squared_distance / (2 * sphere_radius * sphere_radius))))
                    /
                    (1 - exp(-beta))
                    );

                // Compute the photon intensity
                const glm::vec3 photon_intensity = computeReflectedIntensity(
                    intensity,
                    -photon_ray.direction,
                    interaction,
                    -ray.direction
                ) * photon_weight * intensity_normalization;

                // Add the photon contribution to the indirect lighting
                caustic_intensity += photon_intensity;
            }

            // Adding the caustic intensity to the surface intensity
            surface_intensity += caustic_intensity;
        }
    }

    return surface_intensity;
}

#endif //BRDFS_H
