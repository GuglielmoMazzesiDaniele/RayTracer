//
// Created by Guglielmo Mazzesi on 12/24/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {

    string path; ///< Path in file system of the texture

    int width = 0; ///< Width of the texture
    int height = 0; ///< Height of the texture
    int channels = 0; ///< Amount of channels of the texture

    unsigned char * data; ///< Texture data

public:
    /**
    * Read and initializes the texture using the provided file system path
    */
    explicit Texture(const char * path) : path(path) {
        // Flipping the image
        stbi_set_flip_vertically_on_load(true);

        // Read the data using the library stb_image
        data = stbi_load(path, & width, & height, & channels, 0);

        // Verifying the image was correctly parsed
        if(!data) {
            cerr << "Error reading texture " << path << endl;
            return;
        }

        // Printing some stats
        cout << "Texture: " << path << " loaded" << endl;
        cout << "Size : " << width << " x " << height << ", Channels: " << channels << endl;
    }

    /**
    * Returns the RGB values of the pixel at given UV coordinates
    * @param UV 2 sized vector containing UV coordinates
    * @returns A vector containing the RGB value of the pixel
    */
    [[nodiscard]] glm::vec3 getPixel(const glm::vec2 & UV) const {
        // Parsing the value of the UV coordinates [0, 1] to pixel coordinates
        int w = floor(UV.x * width);
        int h = floor(UV.y * height);

        // Clamping coordinates to avoid out-of-bounds access
        w = min(max(w, 0), width - 1);
        h = min(max(h, 0), height - 1);

        // Computing the current index
        const int current_index = (h * width + w) * channels;

        // Extracting data from the texture after mapping from range [0, 255] to [0, 1]
        return {
            static_cast<float>(data[current_index + 0]) / 255.0f,
            static_cast<float>(data[current_index + 1]) / 255.0f,
            static_cast<float>(data[current_index + 2]) / 255.0f
        };
    }
};

#endif //TEXTURE_H
