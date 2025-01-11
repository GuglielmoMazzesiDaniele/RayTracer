//
// Created by Guglielmo Mazzesi on 14.11.24.
//

#ifndef CAMERA_H
#define CAMERA_H

/**
 * Implementation of the Camera class, which represents a camera
 */
class Camera : public Entity {

    glm::vec2 resolution; ///< The camera resolution
    float focal_distance; ///< The focal distance of the camera
    float aperture; ///< The camera aperture
    float fov; ///< The camera field of view
    string name; /// The camera name, used to generate images


public:
    /**
     * Default constructor
     * @param transform Transform of the camera
     * @param name Name of the camera
     * @param resolution Resolution of the camera
     * @param fov Field of view of the camera
     */
    Camera(const glm::mat4 & transform, string name, const glm::vec2 & resolution, const float fov)
        : Entity(transform), resolution(resolution), fov(fov), name(std::move(name)) {
    }

    /**
    * Complete constructor
    * @param transform Transform of the camera
    * @param name Name of the camera
    * @param resolution Resolution of the camera
    * @param fov Field of view of the camera
    * @param focal_distance The camera focal distance
    * @param aperture The camera aperture
    */
    Camera(const glm::mat4 & transform, string name, const glm::vec2 & resolution,
            const float fov, const float focal_distance, const float aperture)
        : Camera(transform, std::move(name), resolution, fov) {
        this->focal_distance = focal_distance;
        this->aperture = aperture;
    }


    /**
     * Function that globalize a local ray using the camera's transform
     * @param local_ray The local ray
     * @return The globalized local ray
     */
    [[nodiscard]] Ray globalizeRay (Ray local_ray) const {
        local_ray.direction = transform * glm::vec4(local_ray.direction, 0);
        local_ray.direction = normalize(local_ray.direction);
        local_ray.origin = transform * glm::vec4(local_ray.origin, 1);
        return local_ray;
    }

    /**
    * Camera resolution width getter
    */
    [[nodiscard]] float getWidth() const {
        return this->resolution.x;
    }

    /**
    * Camera resolution height getter
    */
    [[nodiscard]] float getHeight() const {
        return this->resolution.y;
    }

    /**
    * Camera fov getter
    */
    [[nodiscard]] float getFOV() const {
        return this->fov;
    }

    /**
    * Camera name getter
    */
    [[nodiscard]] string getName() const {
        return this->name;
    }

    /**
    * Camera focal distance getter
    */
    [[nodiscard]] float getFocalDistance() const {
        return this->focal_distance;
    }

    /**
    * Camera aperture getter
    */
    [[nodiscard]] float getAperture() const {
        return this->aperture;
    }
};

#endif //CAMERA_H
