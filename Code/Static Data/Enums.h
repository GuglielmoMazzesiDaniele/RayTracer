//
// Created by Guglielmo Mazzesi on 11/20/2024.
//

#ifndef ENUMS_H
#define ENUMS_H

// Enums

// BRDFs
enum BRDFs {
    PHONG,
    COOK_TORRANCE
};

// Cook Torrance Distribution
enum specular_distribution {
    BLINN_PHONG,
    BECKMANN,
    GGX,
    ANISOTROPIC_GGX,
};

// BVH Split methods
enum split_method {
    // Surface Area Heuristic
    SAH,
    // Middle of the primitives centroids mean
    MIDDLE,
    // Equal counts of primitives
    EQUAL_COUNTS
  };

// Material type
enum material_type {
    VOLUMETRIC,
    SOLID
};

// Photons type
enum photon_type {
    INDIRECT,
    CAUSTIC
};

// Spatial Data Structure (SDS)
enum SDS {
    // Do not use SDS (computationally expensive)
    NO_SDS,
    // Wrap meshes in a single bounding box (computationally expensive)
    BASIC_BOXING,
    // Use a BVH SDS with SAH
    BVH_SDS,
    // Use a K-D Tree SAS with mean centre of mass heuristic
    KD_TREE_SDS
};

// TMOs
enum TMO {
    EXTENDED_REINHARD,
    ACES,
    LOGARITHMIC,
    POWER,
    LINEAR
};

#endif //ENUMS_H
