//
// Created by Guglielmo Mazzesi on 12/11/2024.
//

#ifndef MESH_BUILDERS_H
#define MESH_BUILDERS_H

/**
* Given a source mesh, build a material mesh using the provided material
* @param source_mesh The source mesh
* @param transform The mesh transform
* @param material The material of the mesh
*/
inline void buildMaterialMesh(const Mesh * source_mesh, const glm::mat4 & transform, const Material & material) {
    // Iterating all objects in the mesh
    for(auto & [faces, name] : source_mesh->objects) {
        // Iterating all faces in the object
        for(const auto & face : faces) {
            // Creating the pointer to the material used by the current face
            const Material * current_material;

            // Verifying if a mtl file was used
            if(face.material_name.empty())
                current_material = & material;
            else
                current_material = & source_mesh->materials.find(face.material_name)->second;

            // Pushing the current primitive
            primitives.push_back(new Triangle(transform, face.vertices,
                face.smoothing, current_material, source_mesh->albedo, source_mesh->normal_map, source_mesh->AO_R_M));
        }
    }
}

/**
* Given a source mesh, build a mesh emitting light
* @param source_mesh The source mesh
* @param transform The mesh transform
* @param intensity The total light intensity emitted by the mesh
* @param color The color of the light, normalized in RGB range [0, 1]
* @param emission_probability The probability of a triangle emitting light
*/
inline void buildLightMesh(const Mesh * source_mesh, const glm::mat4 & transform, const glm::vec3 & intensity,
        const glm::vec3 & color, const float emission_probability) {
    // // Computing the intensity per triangle
    // const glm::vec3 triangle_intensity = intensity / (static_cast<float>(source_mesh->primitives_amount)
    //     * emission_probability);
    //
    // // Initializing the mesh material
    // const auto triangle_material = new Material;
    // triangle_material->diffuse = color;
    // triangle_material->specular = color;
    // triangle_material->roughness = 0.25;
    // triangle_material->transparency = 1.0f;
    //
    // // Iterating all objects in the mesh
    // for(auto & object : source_mesh->objects) {
    //     // Iterating all faces in the object
    //     for(auto & current_face : object.faces) {
    //         // Building the array composing the triangle's vertices
    //         const glm::vec3 * vertices [3];
    //         vertices[0] = & source_mesh->vertices[current_face.vertices[0]];
    //         vertices[1] = & source_mesh->vertices[current_face.vertices[1]];
    //         vertices[2] = & source_mesh->vertices[current_face.vertices[2]];
    //
    //         if(RandomGenerator::getInstance()->getRandomFloat() <= emission_probability) {
    //             // Case in which smooth shading is active
    //             if(current_face.smoothing) {
    //                 // Reading the normals
    //                 const unsigned int first_normal_index = current_face.normals[0];
    //                 const unsigned int second_normal_index = current_face.normals[1];
    //                 const unsigned int third_normal_index = current_face.normals[2];
    //
    //                 // Building the array composing the triangle's normals
    //                 const glm::vec3 * normals [3];
    //                 normals[0] = & source_mesh->normals[first_normal_index];
    //                 normals[1] = & source_mesh->normals[second_normal_index];
    //                 normals[2] = & source_mesh->normals[third_normal_index];
    //
    //                 // Initializing the triangle
    //                 primitives.push_back( new LightTriangle(transform, triangle_intensity, vertices,
    //                     normals, nullptr, true, triangle_material));
    //             }
    //             else
    //                 primitives.push_back(new LightTriangle(transform, triangle_intensity, vertices,
    //                     nullptr, nullptr, false, triangle_material));
    //         }
    //         else {
    //             // Case in which smooth shading is active
    //             if(current_face.smoothing) {
    //                 // Reading the normals
    //                 const unsigned int first_normal_index = current_face.normals[0];
    //                 const unsigned int second_normal_index = current_face.normals[1];
    //                 const unsigned int third_normal_index = current_face.normals[2];
    //
    //                 // Building the array composing the triangle's normals
    //                 const glm::vec3 * normals [3];
    //                 normals[0] = & source_mesh->normals[first_normal_index];
    //                 normals[1] = & source_mesh->normals[second_normal_index];
    //                 normals[2] = & source_mesh->normals[third_normal_index];
    //
    //                 // Initializing the triangle
    //                 primitives.push_back(new Triangle(transform, vertices, normals, nullptr, true, triangle_material));
    //             }
    //             else
    //                 primitives.push_back(new Triangle(transform, vertices, nullptr, nullptr, false, triangle_material));
    //         }
    //     }
    // }
}

#endif //MESH_BUILDERS_H
