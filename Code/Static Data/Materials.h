//
// Created by Guglielmo Mazzesi on 26.10.24.
//

#ifndef MATERIALS_H
#define MATERIALS_H
#include "Colors.h"

// BASIC MATERIALS
inline Material red_material;
inline Material blue_material;
inline Material green_material;
inline Material yellow_material;
inline Material grey_material;

// METALS
inline Material copper_material;
inline Material brass_material;

// REFLECTION
inline Material smooth_reflective_material;
inline Material rough_reflective_material;
inline Material black_reflective_material;

// OTHER MATERIALS
inline Material air_material;
inline Material water_material;
inline Material glass_material;

inline void defineStandardMaterials(int frame) {

    // BASIC MATERIALS

    // RED MATERIAL
    red_material.ambient = glm::vec3(1);
    red_material.diffuse = glm::vec3(0.7, 0.3, 0.3);
    red_material.specular = glm::vec3(0.7, 0.5, 0.5);
    red_material.shininess = 100.0f;
    red_material.roughness = 1.0f;

    // BLUE MATERIAL
    blue_material.ambient = glm::vec3(0.001);
    blue_material.diffuse = glm::vec3(0.3, 0.3, 0.5);
    blue_material.specular = glm::vec3(0.5, 0.5, 0.7);
    blue_material.shininess = 100.0f;
    blue_material.roughness = 0.1f;

    // GREEN MATERIAL
    green_material.ambient = glm::vec3(0.001);
    green_material.diffuse = glm::vec3(0.3, 0.6, 0.2);
    green_material.specular = glm::vec3(0.01, 0.01, 0.01);
    green_material.shininess = 1.0f;
    green_material.roughness = 0.9f;

    // GREY MATERIAL
    grey_material.ambient = glm::vec3(1);
    grey_material.diffuse = glm::vec3(0.8);
    grey_material.specular = glm::vec3(0.01);
    grey_material.shininess = 10.0f;
    grey_material.roughness = 0.9f;

    // YELLOW MATERIAL
    yellow_material.ambient = glm::vec3(0.001);
    yellow_material.diffuse = glm::vec3(0.5, 0.5, 0.1);
    yellow_material.specular = glm::vec3(0.8, 0.8, 0.2);
    yellow_material.shininess = 100.0f;
    yellow_material.roughness = 0.9f;

    // METALS

    // COPPER
    copper_material.ambient = glm::vec3(1);
    copper_material.diffuse = glm::vec3(0.7, 0.5, 0.5) / 5.0f;
    copper_material.specular = glm::vec3(0.7, 0.5, 0.5);
    copper_material.reflection_filter = glm::vec3(0.7, 0.5, 0.5);
    copper_material.shininess = 10.0f;
    copper_material.roughness = 0.25f;

    // BRASS
    brass_material.ambient = metallic_violet;
    brass_material.diffuse = metallic_violet / 1.5f;
    brass_material.specular = metallic_violet;
    brass_material.reflection_filter = metallic_violet;
    brass_material.shininess = 10.0f;
    brass_material.roughness = 0.25f;
    brass_material.glossiness = 1.0f;

    // REFLECTIVE MATERIALS

    // SMOOTH REFLECTIVE
    smooth_reflective_material.reflectivity = 1.0f;
    smooth_reflective_material.glossiness = 1.0f;

    // ROUGH REFLECTIVE
    rough_reflective_material.reflectivity = 1.0f;
    rough_reflective_material.glossiness = 0.1f;

    // BLACK REFLECTIVE MATERIAL
    black_reflective_material.diffuse = glm::vec3(0.01);
    black_reflective_material.specular = glm::vec3(0.01);
    black_reflective_material.roughness = 1.0f;
    black_reflective_material.reflectivity = 0.7f;
    black_reflective_material.glossiness = 1.0f;

    // OTHER MATERIALS

    // AIR MATERIAL
    air_material.refraction_index = 1.0f;
    air_material.refractivity = 1.0f;

    // WATER MATERIAL
    water_material.refraction_index = 1.33f;
    water_material.transparency = 1.0f;
    water_material.refractivity = 1.0f;

    // GLASS MATERIAL
    glass_material.refraction_index = 1.5f;
    glass_material.refractivity = 1.0f;
};

#endif //MATERIALS_H
