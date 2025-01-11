//
// Created by Guglielmo Mazzesi on 11/25/2024.
//

#ifndef SCENES_H
#define SCENES_H

inline vector<Light *> lights; ///< A list of point lights in the scene
inline vector<DirectionalLight *> directional_lights; ///< A list of directional lights in the scene
inline vector<Primitive *> primitives; ///< A list of all objects in the scene

inline vector<Photon> indirect_photons; ///< A list of photons used for indirect lighting
inline vector<Photon> caustic_photons; ///< A list of photons used for caustics

inline vector<Plane *> planes; ///< A lost of all the planes in the scene
inline vector<Camera *> cameras; ///< A list of all the cameras capturing the scene

inline BVH * bvh;
inline KDTreeNode * indirect_photons_root;
inline KDTreeNode * caustic_photons_root;

#endif //SCENES_H