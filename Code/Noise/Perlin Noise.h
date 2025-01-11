//
// Created by Guglielmo Mazzesi on 11/18/2024.
//

#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

// The permutations hash tables
constexpr int permutation[] = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

class PerlinNoise {
    static PerlinNoise * instance; ///< Pointer to the instance of the Singleton

    int hash [512]; ///< Double sized permutations hash table to avoid overflowing

    /**
    * Default constructor (must be private)
    */
    PerlinNoise() {
        // Initializing the double sized permutation hash table
        for (int i = 0; i < 512; i++) {
            hash[i] = permutation[i % 256];
        }
    }

    /**
    * Increments the given value while handling the overflows
    */
    static int Increment(const int value) {
        // TODO: repeat
        return (value + 1) & 255;
    }

    /**
    * Computed the gradient value of the given hash and coordinates
    */
    static double Gradient(const int hash, const double x, const double y, const double z) {
        switch (hash & 0xF) {
            case 0x0: return  x + y;
            case 0x1: return -x + y;
            case 0x2: return  x - y;
            case 0x3: return -x - y;
            case 0x4: return  x + z;
            case 0x5: return -x + z;
            case 0x6: return  x - z;
            case 0x7: return -x - z;
            case 0x8: return  y + z;
            case 0x9: return -y + z;
            case 0xA: return  y - z;
            case 0xB: return -y - z;
            case 0xC: return  y + x;
            case 0xD: return -y + z;
            case 0xE: return  y - x;
            case 0xF: return -y - z;
            default: return 0;
        }
    }

    static glm::vec3 Gradient(const int hash){
        const static glm::vec3 gradients[16] = {
            { 1, 1, 0}, {-1, 1, 0}, { 1,-1, 0}, {-1,-1, 0},
            { 1, 0, 1}, {-1, 0, 1}, { 1, 0,-1}, {-1, 0,-1},
            { 0, 1, 1}, { 0,-1, 1}, { 0, 1,-1}, { 0,-1,-1},
            { 1, 1, 0}, { 0,-1, 1}, {-1, 1, 0}, { 0,-1,-1}
        };
        return gradients[hash & 0xF];
    }

public:
    /**
    * Getter for the singleton instance
    * @return The Perlin Noise instance
    */
    static PerlinNoise * getInstance();

    /**
    * Generates perlin noise value within range [0, 1] for the given coordinates
    * @param x The X coordinate of the requested value
    * @param y The Y coordinates of the requested value
    * @param z The Z coordinates of the requested value
    * @return A double representing the value of the function in point (x, y, z)
    */
    [[nodiscard]] double Noise(const double x, const double y, const double z) const {
        // TODO: Include repeat

        // Computing the unitary coordinates of the cube
        const int unit_x = static_cast<int>(floor(x)) & 255;
        const int unit_y = static_cast<int>(floor(y)) & 255;
        const int unit_z = static_cast<int>(floor(z)) & 255;

        // Computing the floating components of coordinates, used to extrapolate the weights
        const double decimal_x = x - floor(x);
        const double decimal_y = y - floor(y);
        const double decimal_z = z - floor(z);

        // Computing the weights of each axis
        const double x_weight = Fade(decimal_x);
        const double y_weight = Fade(decimal_y);
        const double z_weight = Fade(decimal_z);

        // Computing the hashed valued for each vertex of the cube
        const int dot_000 = hash[hash[hash[unit_x           ] + unit_y           ] + unit_z           ];
        const int dot_001 = hash[hash[hash[unit_x           ] + unit_y           ] + Increment(unit_z)];
        const int dot_010 = hash[hash[hash[unit_x           ] + Increment(unit_y)] + unit_z           ];
        const int dot_011 = hash[hash[hash[unit_x           ] + Increment(unit_y)] + Increment(unit_z)];
        const int dot_100 = hash[hash[hash[Increment(unit_x)] + unit_y           ] + unit_z           ];
        const int dot_101 = hash[hash[hash[Increment(unit_x)] + unit_y           ] + Increment(unit_z)];
        const int dot_110 = hash[hash[hash[Increment(unit_x)] + Increment(unit_y)] + unit_z           ];
        const int dot_111 = hash[hash[hash[Increment(unit_x)] + Increment(unit_y)] + Increment(unit_z)];

        // Computing the final value using linear interpolation smoothed with precomputed weights
        double x1 = Lerp(Gradient(dot_000, decimal_x, decimal_y, decimal_z),
                         Gradient(dot_100, decimal_x - 1, decimal_y, decimal_z),
                         x_weight);

        double x2 = Lerp(Gradient(dot_010, decimal_x, decimal_y - 1, decimal_z),
                         Gradient(dot_110, decimal_x - 1, decimal_y - 1, decimal_z),
                         x_weight);

        const double y1 = Lerp(x1, x2, y_weight);

        x1 = Lerp(Gradient(dot_001, decimal_x, decimal_y, decimal_z - 1),
            Gradient(dot_101, decimal_x - 1, decimal_y, decimal_z - 1),
            x_weight);
        x2 = Lerp(Gradient(dot_011, decimal_x, decimal_y - 1, decimal_z - 1),
            Gradient(dot_111, decimal_x - 1, decimal_y - 1, decimal_z - 1),
            x_weight);

        const double y2 = Lerp(x1, x2, y_weight);

        // Computing final noise value (mapping to range [0, 1])
        return (Lerp(y1, y2, z_weight) + 1) / 2;
    }

    [[nodiscard]] double StringNoise(const double x, const double y, const double z) const {
        // TODO: Include repeat

        // Computing the unitary coordinates of the cube
        const int unit_x = static_cast<int>(floor(x)) & 255;
        const int unit_y = static_cast<int>(floor(y)) & 255;
        const int unit_z = static_cast<int>(floor(z)) & 255;

        // Computing the floating components of coordinates, used to extrapolate the weights
        const double decimal_x = x - floor(x);
        const double decimal_y = y - floor(y);
        const double decimal_z = z - floor(z);

        // Computing the weights of each axis
        const double x_weight = Fade(decimal_x);
        const double y_weight = Fade(decimal_y);
        const double z_weight = Fade(decimal_z);

        // Computing the hashed valued for each vertex of the cube
        const int dot_000 = hash[hash[hash[unit_x           ] + unit_y           ] + unit_z           ];
        const int dot_001 = hash[hash[hash[unit_x           ] + unit_y           ] + Increment(unit_z)];
        const int dot_010 = hash[hash[hash[unit_x           ] + Increment(unit_y)] + unit_z           ];
        const int dot_011 = hash[hash[hash[unit_x           ] + Increment(unit_y)] + Increment(unit_z)];
        const int dot_100 = hash[hash[hash[Increment(unit_x)] + unit_y           ] + unit_z           ];
        const int dot_101 = hash[hash[hash[Increment(unit_x)] + unit_y           ] + Increment(unit_z)];
        const int dot_110 = hash[hash[hash[Increment(unit_x)] + Increment(unit_y)] + unit_z           ];
        const int dot_111 = hash[hash[hash[Increment(unit_x)] + Increment(unit_y)] + Increment(unit_z)];

        // Computing the final value using linear interpolation smoothed with precomputed weights
        double x1 = Lerp(Gradient(dot_000, decimal_x, decimal_y, decimal_z),
                         Gradient(dot_100, decimal_x - 1, decimal_y, decimal_z),
                         x_weight);

        double x2 = Lerp(Gradient(dot_010, decimal_x, decimal_y - 1, decimal_z),
                         Gradient(dot_110, decimal_x - 1, decimal_y - 1, decimal_z),
                         x_weight);

        const double y1 = Lerp(x1, x2, y_weight);

        x1 = Lerp(Gradient(dot_001, decimal_x, decimal_y, decimal_z - 1),
            Gradient(dot_101, decimal_x - 1, decimal_y, decimal_z - 1),
            x_weight);
        x2 = Lerp(Gradient(dot_011, decimal_x, decimal_y - 1, decimal_z - 1),
            Gradient(dot_111, decimal_x - 1, decimal_y - 1, decimal_z - 1),
            x_weight);

        const double y2 = Lerp(x1, x2, y_weight);

        // Computing final noise value (mapping to range [0, 1])
        return abs(Lerp(y1, y2, z_weight));
    }

    /**
    * Function that generates a texture using the fractal noise algorithm
    * @param width The width of the texture
    * @param height The height of the texture
    * @param starting_amplitude The starting amplitude
    * @param starting_frequency The starting frequency
    * @param layers The amount of layers
    * @return A double vector sized width * height containing the texture
    */
    [[nodiscard]] float * GenerateFractalNoiseTexture(const unsigned int width, const unsigned int height,
        const float starting_amplitude, const float starting_frequency, const unsigned int layers) const {
        // Initializing the texture
        const auto texture = new float [width * height];

        // Initializing auxiliary constants
        const float amplitude_multiplier = 0.35;
        const float frequency_multiplier = 1.8;

        // Initializing data
        float max_noise = -INFINITY;

        // Creating the texture
        for(unsigned int i = 0; i < width; i++) {
            for(unsigned int j = 0; j < height; j++) {
                // Generating current point in the Perlin space
                auto point = glm::vec3(i, j, 0) * starting_frequency;

                // Resetting the amplitude
                float amplitude = starting_amplitude;

                // Computing Perlin Noise at all layers
                for(unsigned int k = 0; k < layers; k++) {
                    // Extracting the noise
                    const float noise = StringNoise(point.x, point.y, point.z) * amplitude;

                    // Adding the current layer to the texture
                    texture [i + j * width] += noise;
                    // Applying frequency multipliers to the point (increasing frequency)
                    point *= frequency_multiplier;
                    // Applying multiplier to the amplitude (reducing amplitude)
                    amplitude *= amplitude_multiplier;
                }

                // Updating the max value
                if(texture[i + j * width] > max_noise)
                    max_noise = texture[i + j * width];
            }
        }

        // Mapping the noise to the range [0,1]
        for(unsigned int i = 0; i < width * height; i++)
            texture[i] /= max_noise;

        return texture;
    }
};

PerlinNoise * PerlinNoise::instance = nullptr;

/**
* Return the singleton instance of the perlin noise generator
*/
inline PerlinNoise * PerlinNoise::getInstance()  {
    if(instance == nullptr)
        instance = new PerlinNoise();
    return instance;
}

inline void PerlinTest(){
    float test_noise = PerlinNoise::getInstance()->Noise(255, 255, 255);
}

#endif //PERLIN_NOISE_H