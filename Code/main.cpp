/**
@file main.cpp
*/

using namespace std;

#include "Import.h"

// Scene variables
vector<Image *> images; ///< A list of all the rendered images

/**
 * Function that defines the cameras capturing the scene
 */
void defineCameras(const int frame_number){
    // Default camera transform
    glm::mat4 T = glm::translate(glm::mat4(1),glm::vec3(0, 3, -5));
    glm::mat4 R = glm::rotate(glm::mat4(1), 0.0f, glm::vec3(0, 1, 0));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(1));
    glm::mat4 default_camera_transform = T * R * S;

    // Testing camera transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, 10, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    glm::mat4 testing_camera_transform = T * R * S;

    // Default camera
    cameras.push_back(new Camera(default_camera_transform, "Default View - Frame " + to_string(frame_number),
        two_k, 90, 11, 0.25));
    // cameras.push_back(new Camera(testing_camera_transform, "Testing View - Frame " + to_string(frame_number),
    //     testing_resolution, 90, 6, 0.1));
}

/**
 * Function that generates the pixel color based on camera and ray direction
 * @param current_camera The camera currently rendering the scene
 * @param ray_direction The ray pointing at the pixel from the camera prospective
 * @return
 */
glm::vec3 computePixel(const Camera * current_camera, const glm::vec3 ray_direction) {
    // Generating the pixel value using depth of field
    if(USE_DEPTH_OF_FIELD) {
        // Initializing the pixel color
        auto pixel_color = glm::vec3(0);

        // Initializing the focal point
        glm::vec3 focal_point = current_camera->getFocalDistance()
                                    * (ray_direction / ray_direction.z) ;

        // Creating a samples of ray shifted by the lens aperture
        for(int k = 0; k < DEPTH_OF_FIELD_SAMPLES_AMOUNT; k++) {
            // Generating a random 2D coordinate withing the lens plane
            glm::vec2 lens_offset = glm::diskRand(current_camera->getAperture());
            // Generating the new ray origin, shifted by the aperture lens
            auto shifted_ray_origin = glm::vec3(lens_offset.x, lens_offset.y, 0.0f);
            // Generating the new ray direction, going from the new origin to the focal point
            glm::vec3 shifted_ray_direction = normalize(focal_point - shifted_ray_origin);

            // Creating the new ray
            Ray shifted_ray {
            .origin = shifted_ray_origin,
            .direction = shifted_ray_direction,
            .current_medium_refraction_index = 1.0f
            };

            // Globalizing the ray
            shifted_ray = current_camera->globalizeRay(shifted_ray);

            // Creating the pixel color
            pixel_color += traceRay(shifted_ray, 0);
        }

        // Computing the mean value of all the shifted rays
        pixel_color = pixel_color / (float)DEPTH_OF_FIELD_SAMPLES_AMOUNT;

        // Returning the value
        return pixel_color;
    }
    // Generating the pixel value with an infinitely small aperture
    else {
        // Creating the ray object
        Ray current_ray {
            .origin = glm::vec3(0.0f),
            .direction = ray_direction,
            .current_medium_refraction_index = 1.0f
        };

        // Globalizing the ray
        current_ray = current_camera->globalizeRay(current_ray);

        // Returning the pixel value
        return traceRay(current_ray, 0);
    }
}

/**
 * Function that render the HDR pixels representing the current scene
 */
void renderCurrentScene() {
    for (const auto current_camera : cameras) {
        if (PRINT_RAYTRACING_EXECUTION_TIME)
            std::cout << "Starting rendering of camera " << current_camera->getName() << std::endl;

        // Ensure camera dimensions are integers
        const int camera_width = static_cast<int>(current_camera->getWidth());
        const int camera_height = static_cast<int>(current_camera->getHeight());
        const float camera_fov = current_camera->getFOV();

        // Compute pixel size and position
        const float pixel_size = 2 * tan(glm::radians(camera_fov / 2)) / camera_width;
        const float top_left_X = -(pixel_size * camera_width) / 2;
        const float top_left_Y = (pixel_size * camera_height) / 2;

        // Initializing a new image
        auto current_image = new Image(current_camera->getName(), camera_width, camera_height);

        clock_t current_time = clock();

        // Create a shared progress counter
        int progress = 0;
        const int total_rays = camera_width * camera_height;

        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < camera_width; i++) {
            for (int j = 0; j < camera_height; j++) {
                glm::vec3 pixel_color(0.0f);

                // Compute pixel value with antialiasing
                if (USE_ANTIALIASING) {
                    float increment_ray_difference = pixel_size / ANTIALIASING_SUBDIVISIONS_AMOUNT;
                    glm::vec3 thread_pixel_color(0.0f);

                    for (unsigned int delta_x = 0; delta_x < ANTIALIASING_SUBDIVISIONS_AMOUNT; delta_x++) {
                        for (unsigned int delta_y = 0; delta_y < ANTIALIASING_SUBDIVISIONS_AMOUNT; delta_y++) {
                            glm::vec3 current_ray_direction(
                                top_left_X + i * pixel_size + increment_ray_difference * delta_x,
                                top_left_Y - j * pixel_size - increment_ray_difference * delta_y,
                                1.0f
                            );
                            current_ray_direction = normalize(current_ray_direction);
                            thread_pixel_color += computePixel(current_camera, current_ray_direction);
                        }
                    }
                    pixel_color = thread_pixel_color / (ANTIALIASING_SUBDIVISIONS_AMOUNT * ANTIALIASING_SUBDIVISIONS_AMOUNT);
                }
                // Compute pixel value without antialiasing
                else {
                    glm::vec3 current_ray_direction(
                        top_left_X + i * pixel_size + pixel_size / 2,
                        top_left_Y - j * pixel_size - pixel_size / 2,
                        1.0f
                    );
                    current_ray_direction = normalize(current_ray_direction);
                    pixel_color = computePixel(current_camera, current_ray_direction);
                }

                // Updating the progress counter atomically
                if (PRINT_RAYTRACING_EXECUTION_PERCENTAGE) {
                    #pragma omp atomic
                    progress++;
                }

                // Print current progress
                if (PRINT_RAYTRACING_EXECUTION_PERCENTAGE) {
                    #pragma omp critical
                    {
                        // Computing the total progress
                        float percent_complete = ((progress * 100.0f) / total_rays);
                        // Printing the total progress
                        cout << fixed << setprecision(2) << current_camera->getName()
                            << " current progress: " << percent_complete << " %" << std::endl;
                    }
                }

                // Set the HDR pixel
                current_image->setHDRPixel(i, j, pixel_color);
            }
        }

        // Measure execution time
        current_time = clock() - current_time;
        if (PRINT_RAYTRACING_EXECUTION_TIME)
            PrintExecutionTime(current_time, "render camera " + current_camera->getName());

        images.push_back(current_image);
    }
}

/**
 * Function that applies post-processing to images
 */
void postprocessImages(){
    // Iterating the images and applying the post-process pipeline
    for (const auto & current_image : images) {
        // Clock time at the beginning of the rendering
        clock_t current_time = clock();

        // Applying the post-processing pipeline
        current_image->applyPostProcessing();

        // Printing useful data regarding the ray tracing time
        current_time = clock() - current_time;

        // Printing execution time
        if(PRINT_POST_PROCESSING_EXECUTION_TIME)
            PrintExecutionTime(current_time, "execute post processing");
    }
}

/**
 * Function tha reset the scene between frames generation
 */
inline void ResetScene() {
    // Clearing the containers of entities
    lights.clear();
    directional_lights.clear();
    primitives.clear();
    planes.clear();
    cameras.clear();

    // Clearing the photons map
    caustic_photons.clear();
    indirect_photons.clear();
    caustic_photons_root = nullptr;
    indirect_photons_root = nullptr;

    // Freeing the BVH memory
    bvh->clear();
}

inline void testingFunction () {
    // Setting up variables
    int width;
    int height;
    int channels;

    // Setting the flip on load
    stbi_set_flip_vertically_on_load(true);

    // Loading the image
    const unsigned char * current_texture = stbi_load("./Meshes/Textures/Bust.png",
                                                      & width, & height, & channels, 0);

    // Verifying the image was correctly parsed
    if(!current_texture) {
        cerr << "Error reading a texture" << endl;
        return;
    }

    // Printing some stats
    std::cout << "Texture loaded: " << width << " x " << height << ", Channels: " << channels << std::endl;

    // Initializing image stuff
    const string image_name ("Test Texture");
    // Creating an image
    const auto current_image = new Image(image_name, width, height);

    // Setting the value of the pixel
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            // Computing the current texture index
            const int current_index = (h * width + w) * channels;

            // Storing the data in the image
            current_image->setHDRPixel(h, w, glm::vec3(
                static_cast<float>(current_texture[current_index + 0]) / 255.0f,
                static_cast<float>(current_texture[current_index + 1]) / 255.0f,
                static_cast<float>(current_texture[current_index + 2]) / 255.0f
            ));
        }
    }

    // Writing the image
    current_image->writeImage("./" + current_image->getName() + ".ppm");
}
/**
 Main function
 @param argc Counter of input arguments
 @param argv Array of pointers to the arguments
 @return
 */
int main(int argc, const char * argv[]) {
    // Testing function
    // testingFunction();

    // Generating an arbitrary amount of frames, passing down the frame number to scene constructor
    for(int frame_number = 0; frame_number < FRAMES_GENERATED; frame_number++) {
        // Initializing the materials
        defineStandardMaterials(frame_number);

        // Defining the scene to be rendered
        defineDefaultScene(frame_number);

        // Inserting cameras in the scene
        defineCameras(frame_number + 1);

        // Building the BVH
        bvh = new BVH();

        // Applying photon mapping
        if(USE_PHOTON_MAPPING) {
            // Building the indirect lights KD tree
            if(USE_INDIRECT_LIGHTING) {
                PrintStartingProcess("construction of the indirect lights KD Tree");

                // Storing the starting time for KD Tree building
                clock_t current_time = clock();

                // Building the KD Tree
                indirect_photons_root = buildKDTree(indirect_photons);

                PrintExecutionTime(clock() - current_time, "build the KD tree");

                cout << "Amount of indirect photons : " << caustic_photons.size() << endl;
            }

            // Building the caustics KD tree
            if(USE_CAUSTIC) {
                // Generating photons
                traceCausticsPhotons();

                PrintStartingProcess("construction of the caustics KD Tree");

                // Storing the starting time for KD Tree building
                clock_t current_time = clock();

                // Building the KD Tree
                caustic_photons_root = buildKDTree(caustic_photons);

                // Printing execution time
                PrintExecutionTime(clock() - current_time, "build the KD tree");
            }

        }

        Ray test_ray {
            .origin = glm::vec3(0,0,0),
            .direction = glm::vec3(0, 0, 1),
        };

        traceRay(test_ray, 0);

        // Rendering the scene
        renderCurrentScene();

        // Resetting the scene
        ResetScene();
    }

    // Applying post-processing
    postprocessImages();

    // Iterating the images and applying the post-process pipeline
    for (const auto & current_image : images) {
        current_image->writeImage("./" + current_image->getName() + ".ppm");
    }

    return 0;
}