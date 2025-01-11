//
// Created by Guglielmo Mazzesi on 12/20/2024.
//

#ifndef PHOTON_MAPPING_SCENE_H
#define PHOTON_MAPPING_SCENE_H

// TRANSFORMS
inline glm::mat4 photon_mapping_area_light_transform;
inline glm::mat4 photon_mapping_glass_sphere_transform;
inline glm::mat4 photon_mapping_plastic_sphere_transform;
inline glm::mat4 photon_mapping_lucy_transform;
inline glm::mat4 photon_mapping_bottom_plane_transform;

// MATERIALS
inline Material photon_mapping_glass_material;
inline Material photon_mapping_grey_material;

inline void definePhotonMappingTransforms(int frame_number) {
    // Auxiliary variables initialization
    glm::mat4 T;
    glm::mat4 R;
    glm::mat4 S;

    // Area light
    T = glm::translate(glm::mat4(1), glm::vec3(0, 10, 7));
    R = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    photon_mapping_area_light_transform = T * R * S;

    // Glass sphere
    T = glm::translate(glm::mat4(1), glm::vec3(4, -1, 8));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(2));
    photon_mapping_glass_sphere_transform = T * R * S;

    // Plastic sphere
    T = glm::translate(glm::mat4(1), glm::vec3(-2, -2, 5.5));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    photon_mapping_plastic_sphere_transform = T * R * S;

    // Lucy transform
    T = glm::translate(glm::mat4(1), glm::vec3(-2, -3, 8));
    R = glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(0, -1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    photon_mapping_lucy_transform = T * R * S;

    // Bottom plane
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    photon_mapping_bottom_plane_transform = T * R * S;
};

inline void definePhotonMappingMaterials(int frame_number) {
    // Glass
    photon_mapping_glass_material.refraction_index = 1.5f;
    photon_mapping_glass_material.refractivity = 1;

    // Grey material
    photon_mapping_grey_material.ambient = glm::vec3(1.0f);
    photon_mapping_grey_material.diffuse = glm::vec3(0.8f);
    photon_mapping_grey_material.specular = glm::vec3(0.0f);
    photon_mapping_grey_material.roughness = 1;
};

inline void definePhotonMappingScene(const int frame_number) {
    // Initializing transforms
    definePhotonMappingTransforms(frame_number);

    // Initializing materials
    definePhotonMappingMaterials(frame_number);

    // SPHERE
    primitives.push_back(new Sphere(photon_mapping_glass_sphere_transform, & photon_mapping_glass_material));
    primitives.push_back(new Sphere(photon_mapping_plastic_sphere_transform, & photon_mapping_glass_material));

    // LUCY
    buildMaterialMesh(new Mesh("./Meshes/OBJ/Course Mesh/Lucy FULL.obj"), photon_mapping_lucy_transform, grey_material);

    // PLANES
    planes.push_back(new Plane(photon_mapping_bottom_plane_transform, & photon_mapping_grey_material));

    // AREA LIGHT
    AreaLight(photon_mapping_area_light_transform, glm::vec3(70), glm::vec3(1.0f), 3.0f, 60, true);
}

#endif //PHOTON_MAPPING_SCENE_H
