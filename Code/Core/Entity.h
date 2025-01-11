//
// Created by Guglielmo Mazzesi on 11/20/2024.
//

#ifndef ENTITY_H
#define ENTITY_H

class Entity {
protected:
    glm::vec3 local_origin = glm::vec3(0, 0, 0); ///< Position in local coordinates

    glm::mat4 transform = glm::mat4(1); ///< 4x4 matrix representing the transform of the object
    glm::mat4 inverse_transform = glm::mat4(1); ///< 4x4 matrix representing the inverse transform

public:

    glm::vec3 global_origin = glm::vec3(0, 0, 0); ///< Position in global coordinates

    /**
    * Default constructor
    * @param transform The entity transform
    */
    explicit Entity(const glm::mat4 & transform) {
        this->transform = transform;
        this->inverse_transform = inverse(transform);
        this->global_origin = this->transform * glm::vec4(this->local_origin, 1);
    }

    /**
    * Functions the return the origin of the light in global coordinates
    */
    [[nodiscard]] glm::vec3 getGlobalOrigin() const {
        return this->global_origin;
    }
};


#endif //ENTITY_H
