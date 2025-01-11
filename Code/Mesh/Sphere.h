//
// Created by Guglielmo Mazzesi on 12/29/2024.
//

#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H

class SphereMesh : public Mesh {

    static constexpr auto center = glm::vec3(0.0f); ///< Center of the sphere

public:
    SphereMesh(const int angular_frequency,
        const int angular_precision,
        const bool smooth_shading,
        const float displacement_scale_factor,
        const Texture * albedo = nullptr,
        const Texture * normal_map = nullptr,
        const Texture * AO_R_M = nullptr,
        const Texture * displacement = nullptr
        ) : Mesh(nullptr, nullptr, albedo, normal_map, AO_R_M, displacement) {
        // Clock time at the beginning of sphere mesh creation
        clock_t current_time = clock();

        // Initializing the object
        Object current_object {};

        // Printing information regarding the terrain creation process
        if(PRINT_OBJ_PARSING_TIME)
            PrintStartingProcess("creation of Mesh Sphere");

        // Initializing top row of coordinates
        vector<glm::vec3> top_row;
        vector<glm::vec3> bottom_row;
        vector<glm::vec2> top_row_uv;
        vector<glm::vec2> bottom_row_uv;

        // Computing the amount of vertices
        unsigned int vertices_amount = 360 * angular_precision * 180 * angular_precision * 2;

        // Filling the first top row with the polar of the sphere
        for (int azimuthal = 0; azimuthal < 360 * angular_precision; azimuthal += angular_frequency) {
            // Casting azimuthal value to float
            auto azimuthal_f = static_cast<float>(azimuthal) / static_cast<float>(angular_precision);

            // Computing the cartesian coordinates
            float x = glm::sin(glm::radians(0.0f)) * glm::cos(glm::radians(azimuthal_f));
            float y = glm::sin(glm::radians(0.0f)) * glm::sin(glm::radians(azimuthal_f));
            float z = glm::cos(glm::radians(0.0f));
            top_row.emplace_back(x, y, z);

            // Computing the UV coordinates
            top_row_uv.emplace_back(glm::radians(azimuthal_f) / (2 * M_PI) + 0.5f, 0.0f);
        }

        // Iterate over polar angles to build rows and stitch triangles
        for (int angular_polar = angular_frequency; angular_polar <= 180 * angular_precision; angular_polar += angular_frequency) {
            // Filling the bottom row for the current latitude
            for (int angular_azimuthal = 0; angular_azimuthal < 360 * angular_precision; angular_azimuthal += angular_frequency) {
                // Casting azimuthal value to float
                auto angular_azimuthal_f = static_cast<float>(angular_azimuthal) / static_cast<float>(angular_precision);

                // Casting polar value to float
                auto angular_polar_f = static_cast<float>(angular_polar) / static_cast<float>(angular_precision);

                // Computing the coordinates
                float x = glm::sin(glm::radians(angular_polar_f)) * glm::cos(glm::radians(angular_azimuthal_f));
                float y = glm::sin(glm::radians(angular_polar_f)) * glm::sin(glm::radians(angular_azimuthal_f));
                float z = glm::cos(glm::radians(angular_polar_f));

                // Creating the coordinates vector
                glm::vec3 cartesian (x, y, z);

                // Computing azimuthal and polar angle of the intersection point
                const float radians_azimuthal = atan2(cartesian.y, cartesian.x);
                const float radians_polar = acos(cartesian.z);

                // Computing the UV coordinates of current point
                const glm::vec2 UV (radians_azimuthal / (2 * M_PI) + 0.5f, radians_polar / M_PI);

                // Pushing the UV coordinates
                bottom_row_uv.push_back(UV);

                // Extracting the displacement at the current UV coordinates
                if(displacement) {
                    // Extracting the displacement value from the displacement map
                    const float displacement_value = this->displacement->getPixel(UV).x;

                    // Applying the displacement
                    cartesian += cartesian * displacement_value * displacement_scale_factor;
                }

                // Pushing the new coordinates
                bottom_row.push_back(cartesian);
            }

            // Extracting the size of the rows
            const unsigned int vertices_amount = top_row.size();

            // Stitching between top_row and bottom_row
            for(int i = 0; i < vertices_amount; i++) {
                // Extracting the quad vertices
                const glm::vec3 cartesian_coordinates [4] {
                    bottom_row[      i % vertices_amount],
                    top_row   [(i + 1) % vertices_amount],
                    top_row   [      i % vertices_amount],
                    bottom_row[(i + 1) % vertices_amount]
                };

                // Extracting the quad normals
                const glm::vec3 normals [4] {
                    normalize(cartesian_coordinates[0] - center),
                    normalize(cartesian_coordinates[1] - center),
                    normalize(cartesian_coordinates[2] - center),
                    normalize(cartesian_coordinates[3] - center)
                };

                // Extracting the uv coordinates
                const glm::vec2 uv_coordinates [4] {
                    bottom_row_uv[      i % vertices_amount],
                    top_row_uv   [(i + 1) % vertices_amount],
                    top_row_uv   [      i % vertices_amount],
                    bottom_row_uv[(i + 1) % vertices_amount]
                };

                // Storing the size of the vertices vector
                int starting_index = this->vertices_coordinates.size();

                // Pushing the data in the corresponding vectors
                for(int j = 0; j < 4;  j++) {
                    this->vertices_coordinates.push_back(cartesian_coordinates[j]);
                    this->vertices_normals.push_back(normals[j]);
                    this->vertices_uv_coordinates.push_back(uv_coordinates[j]);
                }

                // Creating the first face
                Face first_face;
                first_face.smoothing = smooth_shading;

                // Creating the second face
                Face second_face;
                second_face.smoothing = smooth_shading;

                // Creating the indexes offsets of the quad's triangles
                constexpr int first_indexes [3] {0, 1, 2};
                constexpr int second_indexes[3] {3, 1, 0};

                // Pushing the data in the faces
                for(int j = 0; j < 3; j++) {
                    // Creating vertices for the first face
                    first_face.vertices_indexes.push_back(starting_index + first_indexes[j]);
                    first_face.normals_indexes.push_back(starting_index + first_indexes[j]);
                    first_face.uv_indexes.push_back(starting_index + first_indexes[j]);
                    // first_face.vertices[j].coordinates     = & this->vertices_coordinates   [starting_index + first_indexes[j]];
                    // first_face.vertices[j].normal          = & this->vertices_normals       [starting_index + first_indexes[j]];
                    // first_face.vertices[j].uv_coordinates  = & this->vertices_uv_coordinates[starting_index + first_indexes[j]];

                    // Creating vertices for the second triangle
                    second_face.vertices_indexes.push_back(starting_index + second_indexes[j]);
                    second_face.normals_indexes.push_back(starting_index + second_indexes[j]);
                    second_face.uv_indexes.push_back(starting_index + second_indexes[j]);
                    // second_face.vertices[j].coordinates    = & this->vertices_coordinates   [starting_index + second_indexes[j]];
                    // second_face.vertices[j].normal         = & this->vertices_normals       [starting_index + second_indexes[j]];
                    // second_face.vertices[j].uv_coordinates = & this->vertices_uv_coordinates[starting_index + second_indexes[j]];
                }

                // Pushing the faces
                current_object.faces.push_back(first_face);
                current_object.faces.push_back(second_face);

                // Increasing the primitives amount
                this->primitives_amount += 2;
            }

            // In the next iteration the bottom row becomes the top row
            top_row = std::move(bottom_row);
            top_row_uv = std::move(bottom_row_uv);
            // Clearing the bottom buffer for next row
            bottom_row.clear();
            bottom_row_uv.clear();
        }

        // Pushing the current object
        this->objects.push_back(current_object);

        elaborateFacesAndVertices();

        // Printing useful data regarding the terrain statistics
        if(PRINT_PRIMITIVES_AMOUNT)
            PrintGenericMessage("There are " + to_string(primitives_amount) + " primitives within the Mesh Sphere");

        // Printing useful data regarding the terrain creation time
        current_time = clock() - current_time;
        if(PRINT_PERLIN_TERRAIN_CREATION_TIME)
            PrintExecutionTime(current_time, "create the Mesh Sphere");
    }
};

#endif //SPHERE_MESH_H
