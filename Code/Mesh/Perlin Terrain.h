//
// Created by Guglielmo Mazzesi on 11/30/2024.
//

#ifndef PERLIN_TERRAIN_H
#define PERLIN_TERRAIN_H

class PerlinTerrainMesh : public Mesh {

public:
    PerlinTerrainMesh(const float width, const float depth,
        const float noise_frequency, const float noise_amplitude, const float time) {
        // Clock time at the beginning of terrain mesh creation
        clock_t current_time = clock();

        // Printing information regarding the terrain creation process
        if(PRINT_OBJ_PARSING_TIME)
            PrintStartingProcess("creation of Perlin Terrain");

        // Initializing the object
        Object current_object {};

        // Creating triangles using the Perlin Noise
        for(float i = - (width / 2); i < width / 2 ; i += noise_frequency) {
            for(float j = - (depth / 2); j < depth / 2; j += noise_frequency) {
                // Generating the array of x coordinates
                const float x_coordinates [4] {
                    i,
                    i,
                    i + noise_frequency,
                    i + noise_frequency
                };

                // Generating the array of y coordinates
                const float z_coordinates [4] {
                    j,
                    j + noise_frequency,
                    j,
                    j + noise_frequency,
                };

                // Generating the array of height values
                float heights [4] {0,0,0,0};

                // Initializing values
                float amplitude = noise_amplitude;
                float frequency = 1.0f;
                const int octaves = 6;

                // Combining multiple octaves of Perlin noise
                for(int i = 0; i < 4; i++) {
                    for (int k = 0; k < octaves; k++) {
                        heights[i] += amplitude * PerlinNoise::getInstance()->Noise(
                            frequency * x_coordinates[i],
                            frequency * time,
                            frequency * z_coordinates[i]);

                        amplitude *= 0.5f;
                        frequency *= 2.0f;
                    }

                    // Resetting amplitude and frequency
                    amplitude = noise_amplitude;
                    frequency = 1.0f;
                }

                // Storing the size of the vertices vector
                int starting_index = vertices_coordinates.size();

                // Pushing the vertices in the array
                for(int i = 0; i < 4; i++) {
                    this->vertices_coordinates.emplace_back(x_coordinates[i], heights[i], z_coordinates[i]);
                }

                // Creating the indexes offsets of the quad's triangles
                constexpr int first_indexes [3] {0, 1, 2};
                constexpr int second_indexes[3] {2, 1, 3};

                // Creating the first face
                Face first_face {};

                // Creating the second face
                Face second_face {};

                // Pushing the data in the faces
                for(int i = 0; i < 3; i++) {
                    // Creating vertices for the first face
                    first_face.vertices[i].coordinates = & this->vertices_coordinates[starting_index + first_indexes[i]];

                    // Creating vertices for the second triangle
                    second_face.vertices[i].coordinates = & this->vertices_coordinates[starting_index + second_indexes[i]];
                }

                // Pushing the faces
                current_object.faces.push_back(first_face);
                current_object.faces.push_back(second_face);

                // Increasing the amount of primitives generated
                primitives_amount +=2;
            }
        }

        // Pushing the current object
        this->objects.push_back(current_object);

        // Printing useful data regarding the terrain statistics
        if(PRINT_PRIMITIVES_AMOUNT)
            PrintGenericMessage("There are " + to_string(primitives_amount) + " primitives within the Perlin Terrain");

        // Printing useful data regarding the terrain creation time
        current_time = clock() - current_time;
        if(PRINT_PERLIN_TERRAIN_CREATION_TIME)
            PrintExecutionTime(current_time, "create the Perlin Terrain");

    }
};

#endif //PERLIN_TERRAIN_H