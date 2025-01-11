//
// Created by Guglielmo Mazzesi on 12/10/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H

/**
 Class describing a material
 */
class Material {

public:
    // CORE
    glm::vec3 self_illuminance = glm::vec3(0, 0, 0); /// < Material self illuminance
    glm::vec3 ambient = glm::vec3(1.0); ///< Material ambient light coefficient
    glm::vec3 diffuse = glm::vec3(0.0); ///< Material diffuse coefficient
    glm::vec3 specular = glm::vec3(0.0); ///< Material specular coefficient

    // GENERAL PROPERTIES
    float refractivity = 0; ///< Material index of refractivity
    float refraction_index = 1; ///< Refraction index of the material, used in transparent materials
    float reflectivity = 0; ///< Material reflectivity property, used to computed reflections
    float glossiness = 1; ///< Material glossiness property
    float transparency = 0; ///< Material transparency
    glm::vec3 transmission_filter = glm::vec3(1.0); ///< Material transmission filter
    glm::vec3 reflection_filter = glm::vec3(1.0); ///< Material reflection filter

    // BRDF
    BRDFs BRDF = COOK_TORRANCE;

    // TYPE
    material_type type = SOLID;

    // COOK-TORRANCE
    float roughness = 1; ///< Material roughness coefficient, used in the Cook-Torrance model
    float anisotropy = 0; ///< Material anisotropy coefficient, used in the Cook-Torrance model
    float fresnel_constant = INFINITY; ///< Fresnel constant coefficient, computed once and stored

    // BLINN-PHONG
    float shininess = 0; ///< Material shininess coefficient, used in the Blinn-Phong model

    // VOLUMETRIC
    float density = 0.0f; ///< Material density, used in the computation for volumetric rendering

    /**
    * Getter for the material diffuse at the given surface point
    * @param surface_point  The surface point
    */
    [[nodiscard]] virtual glm::vec3 getDiffuse(const glm::vec3 surface_point) const {
        return this->diffuse;
    }

    /**
    * Getter for the material specular at the given surface point
    * @param surface_point  The surface point
    */
    [[nodiscard]] virtual glm::vec3 getSpecular(const glm::vec3 surface_point) const {
        return this->specular;
    }

    [[nodiscard]] glm::vec3 computeSurfaceIntensity (const Interaction & interaction, const Ray & ray) const {
        // Different behaviour based on the type
        switch(type) {
            // Default case, the material is solid
            default :
            case VOLUMETRIC:
            case SOLID : {
                return CookTorrance(interaction, ray);
            }
        }
    }

};

#endif //MATERIAL_H
