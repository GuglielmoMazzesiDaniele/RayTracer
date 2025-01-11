//
// Created by Guglielmo Mazzesi on 11/7/2024.
//

#ifndef SETTINGS_H
#define SETTINGS_H

// RAY TRACING
constexpr int MAX_RAY_TRACING_RECURSION_LEVEL = 5;
constexpr int FRAMES_GENERATED = 1;

constexpr bool USE_ANTIALIASING = false;
constexpr float ANTIALIASING_SUBDIVISIONS_AMOUNT = 2;

constexpr bool USE_FRESNEL = true;

constexpr bool PRINT_RAYTRACING_EXECUTION_PERCENTAGE = false;
constexpr bool PRINT_MAXIMUM_RECURSION_LEVEL_REACHED = false;
constexpr bool PRINT_RAYTRACING_EXECUTION_TIME = true;

// POST PROCESSING
constexpr bool USE_GAMMA_CORRECTION = true;
constexpr float GAMMA_CORRECTION_FACTOR = 1.0F / 2.2f;
constexpr bool USE_TONE_MAPPING = true;
constexpr auto TONE_MAPPING_OPERATOR = EXTENDED_REINHARD;
constexpr bool PRINT_POST_PROCESSING_EXECUTION_TIME = true;

// LIGHT
constexpr bool USE_OCCLUSION = true;
constexpr bool USE_LIGHT_ATTENUATION = true;
constexpr auto AMBIENT_LIGHT = glm::vec3(0.0f);
constexpr auto AREA_LIGHT_SAMPLES_AMOUNT = 250;

// PHOTON MAPPING
constexpr bool USE_PHOTON_MAPPING = false;
constexpr bool USE_INDIRECT_LIGHTING = false;
constexpr bool USE_CAUSTIC = true;
constexpr int MAX_PHOTON_TRACING_RECURSION_LEVEL = 3;

// CAMERA
constexpr bool USE_DEPTH_OF_FIELD = false;
constexpr int DEPTH_OF_FIELD_SAMPLES_AMOUNT = 15;

// MATERIALS
constexpr int ROUGH_SURFACES_SAMPLE_SIZE = 25;

// SPECULAR MODELS
constexpr auto COOK_TORRANCE_ISOTROPIC_DISTRIBUTION = GGX;

// SDS
constexpr bool PRINT_SDS_BUILDING_TIME = true;

// BVH
constexpr auto SPLIT_METHOD = SAH;
constexpr int SAH_BUCKETS_AMOUNT = 12;

// MESH
constexpr bool PRINT_OBJ_PARSING_TIME = true;
constexpr bool PRINT_PERLIN_TERRAIN_CREATION_TIME = true;
constexpr bool PRINT_PRIMITIVES_AMOUNT = true;

#endif //SETTINGS_H
