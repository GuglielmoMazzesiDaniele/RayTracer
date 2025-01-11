//
// Created by Guglielmo Mazzesi on 12/7/2024.
//

#ifndef PERLIN_SPHERE_H
#define PERLIN_SPHERE_H

class PerlinSphereMesh : public Mesh {

    static constexpr float radius = 1.0f;

public:
    PerlinSphereMesh(const float angular_frequency, const float noise_frequency, const float noise_amplitude,
        const float noise_perturbance, const bool smooth_shading) {
        // Clock time at the beginning of sphere mesh creation
        clock_t current_time = clock();

        // Printing information regarding the terrain creation process
        if(PRINT_OBJ_PARSING_TIME)
            PrintStartingProcess("creation of Perlin Sphere");

        // Initializing the object
        Object current_object {};

        // Initializing top row of coordinates
        vector<glm::vec3> top_row;
        vector<glm::vec3> bottom_row;

        // Filling the first top row with the polar of the sphere
        for (float azimuthal = 0; azimuthal < 360; azimuthal += angular_frequency) {
            float x = radius * glm::sin(glm::radians(0.0f)) * glm::cos(glm::radians(azimuthal));
            float y = radius * glm::sin(glm::radians(0.0f)) * glm::sin(glm::radians(azimuthal));
            float z = radius * glm::cos(glm::radians(0.0f));
            top_row.emplace_back(x, y, z);
        }

        // Iterate over polar angles to build rows and stitch triangles
        for (float polar = angular_frequency; polar <= 180; polar += angular_frequency) {
            // Filling the bottom row for the current latitude
            for (float azimuthal = 0; azimuthal < 360; azimuthal += angular_frequency) {
                // Computing the coordinates
                float x = radius * glm::sin(glm::radians(polar)) * glm::cos(glm::radians(azimuthal));
                float y = radius * glm::sin(glm::radians(polar)) * glm::sin(glm::radians(azimuthal));
                float z = radius * glm::cos(glm::radians(polar));

                // Extracting the perlin noise at the coordinates
                const float noise = PerlinNoise::getInstance()->StringNoise((x + noise_perturbance) * noise_frequency,
                                                                            (y + noise_perturbance) * noise_frequency,
                                                                            (z + noise_perturbance) * noise_frequency)
                                                                                * noise_amplitude;

                // Creating the vector
                glm::vec3 coordinates {x, y, z};

                // Applying the perlin noise
                coordinates += coordinates * noise;

                // Pushing the new coordinates
                bottom_row.push_back(coordinates);
            }

            // Extracting the size of the rows
            const unsigned int vertices_amount = top_row.size();

            // Stitching between top_row and bottom_row
            for (int i = 0; i < vertices_amount; i++) {
                // Extracting the quad vertices
                const auto first_vertex = bottom_row[i % vertices_amount];
                const auto second_vertex =  top_row[(i + 1) % vertices_amount];
                const auto third_vertex = top_row[i % vertices_amount];
                const auto fourth_vertex = bottom_row[(i + 1) % vertices_amount];

                // Extracting the quad normals
                const auto center = glm::vec3(0.0f);
                const auto first_normal  = normalize(first_vertex - center);
                const auto second_normal = normalize(second_vertex - center);
                const auto third_normal  = normalize(third_vertex - center);
                const auto fourth_normal = normalize(fourth_vertex - center);

                // Storing the size of the vertices vector
                int starting_index = vertices_coordinates.size();

                // Pushing the vertices on the vertices vector
                this->vertices_coordinates.push_back(first_vertex);
                this->vertices_coordinates.push_back(second_vertex);
                this->vertices_coordinates.push_back(third_vertex);
                this->vertices_coordinates.push_back(fourth_vertex);

                // Pushing the normals on the normals vector
                this->vertices_normals.push_back(first_normal);
                this->vertices_normals.push_back(second_normal);
                this->vertices_normals.push_back(third_normal);
                this->vertices_normals.push_back(fourth_normal);

                // Creating the first face
                Face first_face {};
                first_face.smoothing = smooth_shading;

                // Creating the second face
                Face second_face {};
                second_face.smoothing = smooth_shading;

                // Creating the indexes offsets of the quad's triangles
                constexpr int first_indexes [3] {0, 1, 2};
                constexpr int second_indexes[3] {3, 1, 0};

                // Pushing the data in the faces
                for(int i = 0; i < 3; i++) {
                    // Creating vertices for the first face
                    first_face.vertices[i].coordinates = & this->vertices_coordinates[starting_index + first_indexes[i]];
                    first_face.vertices[i].normal = this->vertices_normals[starting_index + first_indexes[i]];
                    first_face.vertices[i].uv_coordinates = & this->vertices_uv_coordinates[starting_index + first_indexes[i]];

                    // Creating vertices for the second triangle
                    second_face.vertices[i].coordinates = & this->vertices_coordinates[starting_index + second_indexes[i]];
                    second_face.vertices[i].normal = this->vertices_normals[starting_index + second_indexes[i]];
                    second_face.vertices[i].uv_coordinates = & this->vertices_uv_coordinates[starting_index + second_indexes[i]];
                }

                // Pushing the faces
                current_object.faces.push_back(first_face);
                current_object.faces.push_back(second_face);

                // // Creating the triangles
                // primitives.push_back(new Triangle(transform, material,
                //     first_vertex, second_vertex, third_vertex));
                // primitives.push_back(new Triangle(transform, material,
                //     fourth_vertex, second_vertex, first_vertex));

                // Increasing the primitives amount
                this->primitives_amount += 2;
            }

            // In the next iteration the bottom row becomes the top row
            top_row = std::move(bottom_row);
            // Clearing the bottom buffer for next row
            bottom_row.clear();
        }

        // Pushing the current object
        this->objects.push_back(current_object);

        // Printing useful data regarding the terrain statistics
        if(PRINT_PRIMITIVES_AMOUNT)
            PrintGenericMessage("There are " + to_string(primitives_amount) + " primitives within the Perlin Sphere");

        // Printing useful data regarding the terrain creation time
        current_time = clock() - current_time;
        if(PRINT_PERLIN_TERRAIN_CREATION_TIME)
            PrintExecutionTime(current_time, "create the Perlin Sphere");
        }
};

#endif //PERLIN_SPHERE_H
