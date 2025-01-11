//
/**
@file Image.h
*/

//  Image.h
//  Raytracer
//
//  Created by Piotr Didyk on 14.07.21.
//

#ifndef IMAGE_H
#define IMAGE_H

using namespace std;

constexpr int MAX_PPM_VALUE = 255;

/**
 Class allowing for creating an image and writing it to a file
 */
class Image {

    string name; ///< The name of the image, used to create the ppm

    int width; ///< Resolution width of the image
    int height; ///< Resolution height of the image

    float * hdr_data; ///< a pointer to the hdr_data representing the images
    unsigned int * ppm_data{}; ///< data that is placed into the ppm file

    /**
     * Function that computes output luminance using the Extended Reinhard function
     * @param input_luminance The array containing the input luminance
     * @param output_luminance The array containing the output luminance
     * @param max_luminance The max luminance present in the input array
     */
    void extendedReinhardTM(const std::vector<float> & input_luminance, std::vector<float> & output_luminance,
                            const float max_luminance) const {
        // Applying the TMO
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_index = h * width + w;

                // Applying the function
                float current_input_luminance = input_luminance[current_index];
                float numerator = current_input_luminance * (1.0f + current_input_luminance
                        / pow(max_luminance, 2));

                // Filling the output array
                output_luminance[current_index] = numerator / (1.0f + current_input_luminance);
            }
        }
    }

    /**
     * Function that computes output luminance using the simplified ACES by Krzysztof Narkowicz
     * @param input_luminance The array containing the input luminance
     * @param output_luminance The array containing the output luminance
     * @param max_luminance The max luminance present in the input array
     */
    void simplifiedACES (const std::vector<float> & input_luminance, std::vector<float> & output_luminance,
                         float max_luminance) const {
        // Constants
        constexpr float A = 2.51f;
        constexpr float B = 0.03f;
        constexpr float C = 2.43f;
        constexpr float D = 0.59f;
        constexpr float E = 0.14f;

        // Applying the TMO
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_index = h * width + w;

                // Applying the function
                float current_input_luminance = input_luminance[current_index];
                output_luminance[current_index] = (current_input_luminance * (A * current_input_luminance + B ))
                        / ( current_input_luminance * (C * current_input_luminance + D) + E);
            }
        }
    }

    /**
     * Function that computes output luminance using a logarithmic function
     * @param input_luminance The array containing the input luminance
     * @param output_luminance The array containing the output luminance
     * @param max_luminance The max luminance present in the input array
     */
    void logarithmicTM(const std::vector<float> & input_luminance, std::vector<float> & output_luminance,
                       const float max_luminance) const {
        // Constants
        constexpr float C = 1.0f;

        // Applying TMO
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_index = h * width + w;

                // Applying the function
                output_luminance [current_index] = log(C * input_luminance[current_index] + 1)
                                                        / log(max_luminance + 1);
            }
        }
    }

    /**
     * Function that computes output luminance using a power function
     * @param input_luminance The array containing the input luminance
     * @param output_luminance The array containing the output luminance
     * @param max_luminance The max luminance present in the input array
     */
    void powerTM(const std::vector<float> & input_luminance, std::vector<float> & output_luminance,
                 float max_luminance) const {
        // Constants
        constexpr float ALPHA  = 0.8f;
        constexpr float BETA = 0.85f;

        // Applying TMO
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_index = h * width + w;

                // Applying the function
                output_luminance [current_index] = ALPHA * pow(input_luminance[current_index], BETA) ;
            }
        }

    }

    /**
     * Function that computes output luminance using a linear function
     * @param input_luminance The array containing the input luminance
     * @param output_luminance The array containing the output luminance
     * @param max_luminance The max luminance present in the input array
     */
    void simpleTM(const std::vector<float> & input_luminance, std::vector<float> & output_luminance,
        const float max_luminance) const {
        // Applying the TMO
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_index = h * width + w;

                // Applying the function
                output_luminance[current_index] = input_luminance[current_index] / max_luminance;
            }
        }
    }

    /**
    * Function clamping the values of each RGB channel to 1
    */
    void applyRGBClamping() {
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_rgb_index = 3 * (h * width + w);

                // Clamping the RGBs values to 1 (they may exceed it a bit after post-processing)
                float current_red_value = hdr_data[current_rgb_index + 0];
                hdr_data[current_rgb_index + 0] = min(1.0f, current_red_value);
                float current_green_value = hdr_data[current_rgb_index + 1];
                hdr_data[current_rgb_index + 1] = min(1.0f, current_green_value);
                float current_blue_value = hdr_data[current_rgb_index + 2];
                hdr_data[current_rgb_index + 2] = min(1.0f, current_blue_value);
            }
        }
    }

    /**
     * Function that applies gamma correction to the image
     */
    void applyGammaCorrection(){
        // Applying the gamma correction to the image
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current index
                int current_rgb_index = 3 * (h * width + w);

                // Apply the gamma correction to the pixel
                hdr_data[current_rgb_index + 0] = pow(hdr_data[current_rgb_index + 0], GAMMA_CORRECTION_FACTOR);
                hdr_data[current_rgb_index + 1] = pow(hdr_data[current_rgb_index + 1], GAMMA_CORRECTION_FACTOR);
                hdr_data[current_rgb_index + 2] = pow(hdr_data[current_rgb_index + 2], GAMMA_CORRECTION_FACTOR);
            }
        }
    }

    /**
     * Function that applies tone mapping to the HDR data using a TMO curve
     *
     */
    void applyToneMapping(){
        // Initializing the max luminance in the scene
        float max_luminance = 0;
        // Initializing the array of input luminance
        std::vector<float> input_luminance(height * width);
        // Initializing the array of output luminance
        std::vector<float> output_luminance(height * width);

        // Iterating our hdr_data to compute the input luminance
        for(int h = 0; h < height; h++){
            for(int w = 0; w < width; w++){
                // Constants for luminance calculation
                constexpr float RED_LUMINANCE_COEFF = 0.2125f;
                constexpr float GREEN_LUMINANCE_COEFF = 0.7154f;
                constexpr float BLUE_LUMINANCE_COEFF = 0.0721f;

                // Current indexes
                const int current_index = h * width + w;
                const int current_rgb_index = 3 * current_index;

                // Extracting the RGB values of the current pixel
                const float current_red = hdr_data[current_rgb_index + 0];
                const float current_green = hdr_data[current_rgb_index + 1];
                const float current_blue = hdr_data[current_rgb_index + 2];
                // Computing the current luminance
                const float current_luminance = RED_LUMINANCE_COEFF * current_red
                                                + GREEN_LUMINANCE_COEFF * current_green
                                                + BLUE_LUMINANCE_COEFF * current_blue;
                // Updating the maximum, if necessary
                if(current_luminance > max_luminance)
                    max_luminance = current_luminance;
                // Saving the current luminance
                input_luminance[current_index] = current_luminance;
            }
        }

        // Verifying that the max luminance is not zero (full black rendering)
        if (max_luminance == 0.0f) {
            std::cerr << "Max luminance in scene is zero, cannot apply tone mapping." << std::endl;
            return;
        }

        switch (TONE_MAPPING_OPERATOR) {
            case LINEAR : {
                simpleTM(input_luminance, output_luminance,max_luminance);
                break;
            }
            case POWER: {
                powerTM(input_luminance, output_luminance, max_luminance);
                break;
            }
            case LOGARITHMIC : {
                logarithmicTM(input_luminance, output_luminance, max_luminance);
                break;
            }
            case ACES: {
                simplifiedACES(input_luminance, output_luminance, max_luminance);
                break;
            }
            case EXTENDED_REINHARD :
            default: {
                extendedReinhardTM(input_luminance, output_luminance, max_luminance);
                break;
            }
        }
        // Invoking a selected curve to compute the tone mapping
        simpleTM(input_luminance, output_luminance, max_luminance);

        // Applying to each HDR pixel the ratio between their output luminance and input luminance
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                // Current indexes
                const int current_index = h * width + w;
                const int current_rgb_index = 3 * current_index;

                // Computing the TMO coefficient obtained by the selected curve
                const float current_input_luminance = input_luminance[current_index];
                const float current_output_luminance = output_luminance[current_index];
                const float TMO_coefficient = current_output_luminance / (current_input_luminance + 1e-6f);

                // Apply the TMO to the pixel
                const float current_red_value = hdr_data[current_rgb_index + 0];
                hdr_data[current_rgb_index + 0] *= min(1.0f, TMO_coefficient * current_red_value);
                const float current_green_value = hdr_data[current_rgb_index + 1];
                hdr_data[current_rgb_index + 1] *= min(1.0f, TMO_coefficient * current_green_value);
                const float current_blue_value = hdr_data[current_rgb_index + 2];
                hdr_data[current_rgb_index + 2] *= min(1.0f, TMO_coefficient * current_blue_value);
            }
        }
    }
    
public:
    /**
     @param name Name of the image
     @param width Width of the image
     @param height Height of the image
     */
    Image(string name, const int width, const int height): name(std::move(name)), width(width), height(height){
        // Initializing the HDR data
        hdr_data = new float[3 * width * height];
    }
    
    /**
     Writes and image to a file in ppm format
     @param path the path where to the target image
     */
    void writeImage(const string & path) const {
        // Opening the file
        ofstream file;
        file.open(path, ofstream::out);

        // Printing an error if the file was not opened correctly
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << path << std::endl;
            return;
        }

        file << "P3" << endl;
        file << width << " " << height <<endl;
        file << MAX_PPM_VALUE << endl;
        if(file.is_open()) {
            cout << "Writing image to " << path << endl;
        }
        for(int h = 0; h < height; h++){
            for (int w = 0; w < width; w++){
                // Current index
                int current_rgb_index = 3 * (h * width + w);

                // Computing the values of each channel with range from 0 to MAX_PPM_VALUE
                unsigned int red_value   = MAX_PPM_VALUE * hdr_data[current_rgb_index + 0];
                unsigned int green_value = MAX_PPM_VALUE * hdr_data[current_rgb_index + 1];
                unsigned int blue_value  = MAX_PPM_VALUE * hdr_data[current_rgb_index + 2];

                // Inserting the values in the files
                file << red_value << " " << green_value << " " << blue_value << " ";
            }
            file<<endl;
        }
        file.close();
    }

    /**
    * Function that applies the post-processing pipeline to the current HDR values
    */
    void applyPostProcessing() {
        // Post-processing pipeline
        if(USE_TONE_MAPPING)
            applyToneMapping();

        applyRGBClamping();

        if(USE_GAMMA_CORRECTION)
            applyGammaCorrection();
    }

    /**
     Set a value for one pixel
     @param x x coordinate of the pixel - index of the column counting from left to right
     @param y y coordinate of the pixel - index of the row counting from top to bottom
     @param color color of the pixel expressed as vec3 of RGB values in range from 0 to 1
     */
    void setHDRPixel(const int x, const int y, const glm::vec3 color) const {
        if(isnan(x))
            cerr << "NaN in sedHDRPixel" << endl;

        // Current index rgb
        const unsigned int current_rgb_index = 3 * (y * width + x);
        hdr_data[current_rgb_index + 0] = color.r;
        hdr_data[current_rgb_index + 1] = color.g;
        hdr_data[current_rgb_index + 2] = color.b;
    }

    string getName() {
        return this->name;
    }
};

#endif
