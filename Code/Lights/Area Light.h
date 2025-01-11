//
// Created by Guglielmo Mazzesi on 11/15/2024.
//

#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

class AreaLight {

public:
    /**
    * Default constructor
    * @param transform Transform of the spotlight
    * @param intensity Total light intensity of the area light
    * @param normalized_intensity Normalized intensity of the area light
    * @param disk_radius The radius of the disk
    * @param aperture The aperture of the area light
    * @param generate_disk Bool indicating weather of not this area light should spawn a disk
    */
    AreaLight(const glm::mat4 & transform, const glm::vec3 & intensity, const glm::vec3 normalized_intensity,
        const float disk_radius, const float aperture, const bool generate_disk) {
        // Generating the samples
        for(int i = 0; i < AREA_LIGHT_SAMPLES_AMOUNT; i++) {
            // Generating a random coordinate on a disk
            const glm::vec2 random_coordinates = glm::diskRand(disk_radius);

            // Generating the transform of the samples
            glm::mat4 sample_transform = translate(transform,glm::vec3(random_coordinates.x, 0, random_coordinates.y));

            // Pushing back into the lights vector
            directional_lights.push_back(new DirectionalLight(sample_transform, 1.0f / AREA_LIGHT_SAMPLES_AMOUNT * intensity, aperture));
        }

        // Generating the disk
        if(generate_disk) {
            // Creating the disk material
            const auto disk_material = new Material;
            disk_material->self_illuminance = normalized_intensity;

            // Generating an actual disk
            primitives.push_back(new Disk(transform, disk_material));
        }
    }
};

#endif //AREA_LIGHT_H
