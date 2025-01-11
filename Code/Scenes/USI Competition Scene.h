//
// Created by Guglielmo Mazzesi on 12/21/2024.
//

#ifndef USI_COMPETITION_SCENE_H
#define USI_COMPETITION_SCENE_H

// Materials
inline Material vaporwave_ceramic_material;
inline Material stone_material;
inline Material atlas_globe_material;
inline Material vaporwave_fog_material;
inline Material vaporwave_sun_material;
inline Material gold_material;
inline MarbleMaterial marble_material;

inline void defineUSICompetitionMaterials(int frame_number) {
    // VAPORWAVE CERAMIC MATERIAL
    vaporwave_ceramic_material.ambient = glm::vec3(0.0f);
    vaporwave_ceramic_material.diffuse = vaporwave_palette_3 / 3.0f;
    vaporwave_ceramic_material.specular = vaporwave_palette_3;
    vaporwave_ceramic_material.shininess = 10.0f;
    vaporwave_ceramic_material.roughness = 0.1f;
    vaporwave_ceramic_material.reflectivity = 0.1f;

    // STONE MATERIAL
    stone_material.diffuse = glm::vec3(0.75);
    stone_material.specular = glm::vec3(0.1);
    stone_material.roughness = 0.9f;

    // ATLAS GLOBE MATERIAL
    atlas_globe_material.self_illuminance = vaporwave_palette_2;

    // VAPORWAVE FOG MATERIAL
    vaporwave_fog_material.self_illuminance = vaporwave_palette_1;
    vaporwave_fog_material.type = VOLUMETRIC;

    // VAPORWAVE SUN MATERIAL
    vaporwave_sun_material.self_illuminance = vaporwave_palette_1 * 10.0f;
    vaporwave_sun_material.type = VOLUMETRIC;

    // GOLD MATERIAL
    gold_material.diffuse = chrome_gold / 4.5f;
    gold_material.specular = chrome_gold;
    gold_material.roughness = 0.1f;
    gold_material.reflection_filter = chrome_gold;
    gold_material.reflectivity = 0.15f;

    // MARBLE MATERIAL
    marble_material.specular = glm::vec3(0.1);
    marble_material.roughness = 1.0f;
    marble_material.veins_color = glm::vec3(0);
    marble_material.filling_color = glm::vec3(1);
}

// LIGHTS
inline glm::mat4 above_light_transform;
inline glm::mat4 behind_camera_light_transform;
inline glm::mat4 in_scene_light_transform;

// AREA LIGHTS
inline glm::mat3 left_area_light;

// SPHERES
inline glm::mat4 vaporwave_sphere;

// SUN
inline glm::mat4 vaporwave_sun_transform;

// FOG
inline glm::mat4 vaporwave_fog_transform;

// CHESSBOARD PLANE
inline glm::mat4 chessboard_plane_transform;

// CEILING
inline glm::mat4 perlin_ceiling;

// DE LOREAN
inline glm::mat4 de_lorean_transform;

// COLUMNS
inline glm::mat4 left_first_column_transform;
inline glm::mat4 left_second_column_transform;
inline glm::mat4 left_third_column_transform;
inline glm::mat4 left_fourth_column_transform;
inline glm::mat4 left_fifth_column_transform;

inline glm::mat4 right_first_column_transform;
inline glm::mat4 right_second_column_transform;
inline glm::mat4 right_third_column_transform;
inline glm::mat4 right_fourth_column_transform;
inline glm::mat4 right_fifth_column_transform;

// LIGHTS
inline glm::mat4 left_first_light_transform;
inline glm::mat4 left_second_light_transform;
inline glm::mat4 left_third_light_transform;
inline glm::mat4 left_fourth_light_transform;
inline glm::mat4 left_fifth_light_transform;

inline glm::mat4 right_first_light_transform;
inline glm::mat4 right_second_light_transform;
inline glm::mat4 right_third_light_transform;
inline glm::mat4 right_fourth_light_transform;
inline glm::mat4 right_fifth_light_transform;

// DOLPHINS
inline glm::mat4 dolphins_transform;

// STATUES
inline glm::mat4 helios_transform;
inline glm::mat4 helios_flipped_transform;
inline glm::mat4 atlas_transform;
inline glm::mat4 atlas_globe_transform;

// DnD STATUES
inline glm::mat4 roblin_voblin_transform;

inline void defineUSICompetitionTransforms(int frame_number) {
    // Auxiliary variables initialization
    glm::mat4 T;
    glm::mat4 R;
    glm::mat4 S;

    // LIGHTS

    // Vaporwave sun
    T = glm::translate(glm::mat4(1), glm::vec3(0, 30, 15));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1.5));
    above_light_transform = T * R * S;

    // Behind camera light
    T = glm::translate(glm::mat4(1), glm::vec3(0, 0, -8));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1.5));
    behind_camera_light_transform = T * R * S;

    // In scene camera light
    T = glm::translate(glm::mat4(1), glm::vec3(0, 7, 5));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    in_scene_light_transform = T * R * S;

    // AREA LIGHTS

    // Left area light
    T = glm::translate(glm::mat4(1), glm::vec3(0, 10, 7.5));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1.5f));
    left_area_light = T * R * S;

    // SPHERES

    // First vaporwave sphere
    T = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(30.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    vaporwave_sphere = T * R * S;

    // PLANES

    // Chessboard plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    chessboard_plane_transform = T * R * S;

    // Perlin ceiling transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, 10, 18));
    R = glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    perlin_ceiling = T * R * S;

    // SUN

    // Vaporwave water sphere
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 150));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(50));
    vaporwave_sun_transform = T * R * S;

    // FOG

    // Vaporwave water sphere
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 10));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(30, 2, 40));
    vaporwave_fog_transform = T * R * S;

    // LEFT COLUMNS

    // First column transform
    T = glm::translate(glm::mat4(1), glm::vec3(-6, -3, 6));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    left_first_column_transform = T * R * S;

    // Second column transform
    T = glm::translate(glm::mat4(1), glm::vec3(-6, -3, 12));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    left_second_column_transform = T * R * S;

    // Third column transform
    T = glm::translate(glm::mat4(1), glm::vec3(-6, -3, 18));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    left_third_column_transform = T * R * S;

    // Fourth column transform
    T = glm::translate(glm::mat4(1), glm::vec3(-6, -3, 24));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    left_fourth_column_transform = T * R * S;

    // Fifth column transform
    T = glm::translate(glm::mat4(1), glm::vec3(-6, -3, 30));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    left_fifth_column_transform = T * R * S;

    // RIGHT COLUMNS

    // First column transform
    T = glm::translate(glm::mat4(1), glm::vec3(6, -3, 6));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    right_first_column_transform = T * R * S;

    // Second column transform
    T = glm::translate(glm::mat4(1), glm::vec3(6, -3, 12));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    right_second_column_transform = T * R * S;

    // Third column transform
    T = glm::translate(glm::mat4(1), glm::vec3(6, -3, 18));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    right_third_column_transform = T * R * S;

    // Fourth column transform
    T = glm::translate(glm::mat4(1), glm::vec3(6, -3, 24));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    right_fourth_column_transform = T * R * S;

    // Fifth column transform
    T = glm::translate(glm::mat4(1), glm::vec3(6, -3, 30));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    right_fifth_column_transform = T * R * S;

    // CONSTANTS FOR LIGHT ANIMATION
    constexpr float spawn_rate = 24;

    constexpr float spawn_frame [5] = {
        48.0f,
        72.0f,
        86.0f,
        110.0f,
        134.0f
    };

    const float spheres_scales [5] {
        max(0.0f, min(0.5f, (static_cast<float>(frame_number) - spawn_frame[0]) / spawn_rate * 0.5f)),
        max(0.0f, min(0.5f, (static_cast<float>(frame_number) - spawn_frame[1]) / spawn_rate * 0.5f)),
        max(0.0f, min(0.5f, (static_cast<float>(frame_number) - spawn_frame[2]) / spawn_rate * 0.5f)),
        max(0.0f, min(0.5f, (static_cast<float>(frame_number) - spawn_frame[3]) / spawn_rate * 0.5f)),
        max(0.0f, min(0.5f, (static_cast<float>(frame_number) - spawn_frame[4]) / spawn_rate * 0.5f)),
    };

    cout << "Scale: " << spheres_scales[0] << endl;

    // LEFT LIGHTS

    // First light transform
    T = glm::translate(left_first_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[0]));
    left_first_light_transform = T * R * S;

    // Second light transform
    T = glm::translate(left_second_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[1]));
    left_second_light_transform = T * R * S;

    // Third light transform
    T = glm::translate(left_third_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[2]));
    left_third_light_transform = T * R * S;

    // Fourth light transform
    T = glm::translate(left_fourth_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[3]));
    left_fourth_light_transform = T * R * S;

    // Fifth light transform
    T = glm::translate(left_fifth_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[4]));
    left_fifth_light_transform = T * R * S;

    // RIGHT LIGHTS

    // First light transform
    T = glm::translate(right_first_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[0]));
    right_first_light_transform = T * R * S;

    // Second light transform
    T = glm::translate(right_second_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[1]));
    right_second_light_transform = T * R * S;

    // Third light transform
    T = glm::translate(right_third_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[2]));
    right_third_light_transform = T * R * S;

    // Fourth light transform
    T = glm::translate(right_fourth_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[3]));
    right_fourth_light_transform = T * R * S;

    // Fifth light transform
    T = glm::translate(right_fifth_column_transform, glm::vec3(0, 6, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(spheres_scales[4]));
    right_fifth_light_transform = T * R * S;

    // DOLPHINS

    // Dolphins transform
    T = glm::translate(glm::mat4(1), glm::vec3(7.5, 3.5, 15));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    dolphins_transform = T * R * S;

    // DE LOREAN

    // DeLorean transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 30));
    R = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, -1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    de_lorean_transform = T * R * S;

    // STATUES

    // Helios transform
    T = glm::translate(glm::mat4(1), glm::vec3(4, -3, 10));
    R = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(0.65));
    helios_transform = T * R * S;

    // Helios flipped transform
    T = glm::translate(glm::mat4(1), glm::vec3(-4, -3, 10));
    R = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(0.65));
    helios_flipped_transform = T * R * S;

    // Atlas transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 7.5));
    R = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1.5f));
    atlas_transform = T * R * S;

    // Atlas globe transform
    S = glm::scale(atlas_transform, glm::vec3(0.85f));
    atlas_globe_transform = T * R * S;

    // DnD STATUES
    T = glm::translate(glm::mat4(1), glm::vec3(4, -3, 10));
    R = glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0));
    R = glm::rotate(R, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(0.15f));
    roblin_voblin_transform = T * R * S;
}

inline void defineUSICompetitionScene(const int frame_number) {
    // Initializing the vaporwave transforms
    defineUSICompetitionTransforms(frame_number);

    // Initializing the vaporwave materials
    defineUSICompetitionMaterials(frame_number);

    // AREA LIGHT
    AreaLight(left_area_light, glm::vec3(100 * max(0.0f, min(1.0f, static_cast<float>(frame_number) / 24.0f))),
            glm::vec3(1), 1, 30, false);

    // CEILING
    if(frame_number > 168)
        buildMaterialMesh(new PerlinTerrainMesh(
            min(70.0f, (static_cast<float>(frame_number) - 168.0f) / 24.0f * 70),
            min(70.0f, (static_cast<float>(frame_number) - 168.0f) / 24.0f * 70),
            0.25, 0.5, frame_number * 0.1), perlin_ceiling, smooth_reflective_material);

    // CHESSBOARD PLANE
    planes.push_back(new ChessboardPlane(chessboard_plane_transform, & grey_material));

    // COLUMNS
    if(true) {
        // LEFT COLUMNS
        const auto column_mesh = new Mesh("./Meshes/OBJ/Column.obj");
        buildMaterialMesh(column_mesh, left_first_column_transform, grey_material);
        buildMaterialMesh(column_mesh,left_second_column_transform, grey_material);
        buildMaterialMesh(column_mesh, left_third_column_transform, grey_material);
        buildMaterialMesh(column_mesh, left_fourth_column_transform, grey_material);
        buildMaterialMesh(column_mesh, left_fifth_column_transform, grey_material);

        // RIGHT COLUMNS
        buildMaterialMesh(column_mesh, right_first_column_transform, grey_material);
        buildMaterialMesh(column_mesh, right_second_column_transform, grey_material);
        buildMaterialMesh(column_mesh, right_third_column_transform, grey_material);
        buildMaterialMesh(column_mesh, right_fourth_column_transform, grey_material);
        buildMaterialMesh(column_mesh, right_fifth_column_transform, grey_material);
    }

    // STATIC DATA TO SETUP LIGHTS ANIMATION
    constexpr float angular_frequency = 2;
    constexpr float noise_frequency = 1.5f;
    constexpr float noise_amplitude = 0.5f;
    constexpr float emission_probability = 0.001;

    constexpr float spawn_rate = 24;

    constexpr float spawn_frame [5] = {
        48.0f,
        72.0f,
        86.0f,
        110.0f,
        134.0f
    };

    const float lights_buildup [5] = {
        max(0.0f, min(1.0f, (static_cast<float>(frame_number) - spawn_frame[0]) / spawn_rate)),
        max(0.0f, min(1.0f, (static_cast<float>(frame_number) - spawn_frame[1]) / spawn_rate)),
        max(0.0f, min(1.0f, (static_cast<float>(frame_number) - spawn_frame[2]) / spawn_rate)),
        max(0.0f, min(1.0f, (static_cast<float>(frame_number) - spawn_frame[3]) / spawn_rate)),
        max(0.0f, min(1.0f, (static_cast<float>(frame_number) - spawn_frame[4]) / spawn_rate)),
    };

    const float noise_perturbance [5] = {
        0  + static_cast<float>(frame_number) * 0.1f,
        10 + static_cast<float>(frame_number) * 0.1f,
        20 + static_cast<float>(frame_number) * 0.1f,
        30 + static_cast<float>(frame_number) * 0.1f,
        50 + static_cast<float>(frame_number) * 0.1f
    };

    //LIGHTS
    // Spawns at 1.5 secs
    if(frame_number >= spawn_frame[0]) {
        // LEFT LIGHTS
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[0], false),
            left_first_light_transform, lights_buildup[0] * 200.0f * vaporwave_palette_1, vaporwave_palette_1, emission_probability);
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[0],false),
            right_first_light_transform, lights_buildup[0] * 200.0f * vaporwave_palette_1, vaporwave_palette_1, emission_probability);
    }
    // Spawns at 2 seconds
    if(frame_number >= spawn_frame[1]) {
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[1],false),
            left_second_light_transform, lights_buildup[1] * 200.0f * vaporwave_palette_2, vaporwave_palette_2, emission_probability);
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[1],false),
            right_second_light_transform, lights_buildup[1] * 200.0f * vaporwave_palette_2, vaporwave_palette_2, emission_probability);
    }
    // Spawns at 2.5 seconds
    if(frame_number >= spawn_frame[2]) {
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[2], false),
            left_third_light_transform, lights_buildup[2] * 200.0f * vaporwave_palette_3, vaporwave_palette_3, emission_probability);
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[2], false),
            right_third_light_transform, lights_buildup[2] * 200.0f * vaporwave_palette_3, vaporwave_palette_3, emission_probability);
    }
    // Spawns at 3 seconds
    if(frame_number >= spawn_frame[3]) {
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[3],false),
            left_fourth_light_transform, lights_buildup[3] * 200.0f * vaporwave_palette_4, vaporwave_palette_4, emission_probability);
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[3],false),
            right_fourth_light_transform, lights_buildup[3] * 200.0f * vaporwave_palette_4, vaporwave_palette_4, emission_probability);
    }
    // Spawns at 3.5 seconds
    if(frame_number >= spawn_frame[4]) {
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[4],false),
            left_fifth_light_transform, lights_buildup[4] * 200.0f * vaporwave_palette_5, vaporwave_palette_5, emission_probability);
        buildLightMesh(
            new PerlinSphereMesh(angular_frequency, noise_frequency, noise_amplitude, noise_perturbance[4],false),
            right_fifth_light_transform, lights_buildup[4] * 200.0f * vaporwave_palette_5, vaporwave_palette_5, emission_probability);

    }

    if(true) {
        // HELIOS
        buildMaterialMesh(new Mesh("./Meshes/OBJ/Helios.obj"), helios_flipped_transform, vaporwave_ceramic_material);
        buildMaterialMesh(new Mesh("./Meshes/OBJ/Helios Flipped.obj"), helios_transform, vaporwave_ceramic_material);

        // DnD STATUES
        // buildMaterialMesh(Mesh("./Meshes/OBJ/PC.obj"), helios_transform, vaporwave_ceramic_material);
        // buildMaterialMesh(Mesh("./Meshes/OBJ/Roblin Voblin.obj"), roblin_voblin_transform, grey_material);

        // ATLAS
        buildMaterialMesh(new Mesh("./Meshes/OBJ/Atlas/Atlas Body.obj"), atlas_transform, gold_material);
        buildMaterialMesh(new Mesh("./Meshes/OBJ/Atlas/Atlas Clothing.obj"), atlas_transform, grey_material);
        buildMaterialMesh(new Mesh("./Meshes/OBJ/Atlas/Atlas Pedestal.obj"), atlas_transform, marble_material);
        buildMaterialMesh(new Mesh("./Meshes/OBJ/Atlas/Atlas Globe.obj"), atlas_transform, glass_material);
        // buildVolumetricMesh("./Meshes/OBJ/Atlas/Atlas Globe.obj", atlas_transform, vaporwave_fog_material, 0.5);
    }
};

#endif //USI_COMPETITION_SCENE_H
