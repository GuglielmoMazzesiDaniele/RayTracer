//
// Created by Guglielmo Mazzesi on 10/23/2024.
//

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

using namespace std;


/**
* Class representing a bounding box
*/
class BoundingBox3 {
public:
    glm::vec3 min_coordinates; ///< The min coordinates of the bounding box
    glm::vec3 max_coordinates; ///< The max coordinates of the bounding box

    /**
    * @return A const reference to the min (index 0) or max (index 1) coordinates
    */
    const glm::vec3 & operator[](const int index) const {
        switch (index) {
            case 0: {
                return min_coordinates;
                break;
            }
            case 1:
            default: {
                return max_coordinates;
                break;
            }
        }
    };
    /**
    * @return A reference to the min (index 0) or max (index 1) coordinates
    */
    glm::vec3 & operator[](const int index) {
        switch (index) {
            case 0: {
                return min_coordinates;
                break;
            }
            case 1:
            default: {
                return max_coordinates;
                break;
                }
        }
    };

    /**
    * Constructor that initializes the bounding box with the given max and min coordinates
    * @param minimum_coordinates A vector containing the bottom left coordinates of the box
    * @param maximum_coordinates A vector containing the top right coordinates of the box
    */
    BoundingBox3(const glm::vec3 minimum_coordinates, const glm::vec3 maximum_coordinates) {
        this->min_coordinates = minimum_coordinates;
        this->max_coordinates = maximum_coordinates;
    }

    /**
    * Constructor that initializes the bounding box with the given max and min coordinates
    */
    BoundingBox3() {
        this->min_coordinates = glm::vec3(INFINITY);
        this->max_coordinates = glm::vec3(-INFINITY);
    }

    /**
    * Getter function of the bounding box diagonal
    * @return The internal diagonal vector of the bounding box
    */
    [[nodiscard]] glm::vec3 getDiagonal() const {
        return this->max_coordinates - this->min_coordinates;
    }

    /**
    * Getter function of the bounding box most extended axis
    * @return The most extended axis (0 = x, 1 = y, 2 = z)
    */
    [[nodiscard]] int getMaximumExtend() const {
        // Getting the diagonal
        glm::vec3 diagonal = this->getDiagonal();
        float test = diagonal[0];
        // Case in which x is the most extended
        if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
            return 0;
        // Case in which y is the most extended
        else if (diagonal.y > diagonal.z)
            return 1;
        // Case in which z is the most extended
        else
            return 2;
    }

    /**
    * Getter for the bounding box surface area
    * @return A float indicating the bounding box surface area
    */
    [[nodiscard]] float getSurfaceArea() const {
        // Computing the diagonal
        const glm::vec3 diagonal = this->getDiagonal();

        // Returning the box surface area
        return 2 * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
    }

    /**
    * Given a point within the bounding box, returns its axis offset within range [0,1] with respect to the
    * min and max coordinates
    * @param point Vec3 containing a point within the bounding box
    * @return A vec3 containing offset for each axis
    */
    [[nodiscard]] glm::vec3 getOffset(const glm::vec3 & point) const {
        glm::vec3 origin = point - this->min_coordinates;

        if(this->max_coordinates.x > this->min_coordinates.x)
            origin.x /= this->max_coordinates.x - this->min_coordinates.x;

        if(this->max_coordinates.y > this->min_coordinates.y)
            origin.y /= this->max_coordinates.y - this->min_coordinates.y;

        if(this->max_coordinates.z > this->min_coordinates.z)
            origin.z /= this->max_coordinates.z - this->min_coordinates.z;

        return origin;
    }

    /**
    * Function that verifies if a ray intersect a bounding box. Core function of the BVH traversal algorithm
    * @param ray The ray in global coordinates
    * @param reciprocals The ray direction reciprocals
    * @param is_reciprocal_negative An array indicating weather or not the ray reciprocals are negative
    * @return True is the ray intersect the bounding box, false otherwise
    */
    bool DoesRayIntersect(const Ray & ray,
        const glm::vec3 & reciprocals,
        const int is_reciprocal_negative[3]) const {
        // Extracting the reference to the current bounding box
        const BoundingBox3 & bounding_box = * this;

        // Computing the lambda value on X axis
        float min_lambda = (bounding_box[    is_reciprocal_negative[0]].x - ray.origin.x) * reciprocals.x;
        float max_lambda = (bounding_box[1 - is_reciprocal_negative[0]].x - ray.origin.x) * reciprocals.x;

        // Computing the lambda value on Y axis
        float min_lambda_y = (bounding_box[    is_reciprocal_negative[1]].y - ray.origin.y) * reciprocals.y;
        float max_lambda_y = (bounding_box[1 - is_reciprocal_negative[1]].y - ray.origin.y) * reciprocals.y;

        // Verifying intersection condition
        if (min_lambda > max_lambda_y || min_lambda_y > max_lambda)
            return false;

        // Updating the min and max value of lambda
        if (min_lambda_y > min_lambda) min_lambda = min_lambda_y;
        if (max_lambda_y < max_lambda) max_lambda = max_lambda_y;

        // Computing the lambda value on Z axis
        float min_lambda_z = (bounding_box[    is_reciprocal_negative[2]].z - ray.origin.z) * reciprocals.z;
        float max_lambda_z = (bounding_box[1 - is_reciprocal_negative[2]].z - ray.origin.z) * reciprocals.z;

        // Verifying intersection condition
        if (min_lambda > max_lambda_z || min_lambda_z > max_lambda)
            return false;

        // Updating the min and max value of lambda
        if (min_lambda_z > min_lambda) min_lambda = min_lambda_z;
        if (max_lambda_z < max_lambda) max_lambda = max_lambda_z;

        // Verifying that intersection is not behind the ray
        return max_lambda > 0;
    }

    /**
    * Function that verifies if a ray intersect a bounding box. Core function of the BVH traversal algorithm
    * @param ray The ray in global coordinates
    * @param reciprocals The ray direction reciprocals
    * @param is_reciprocal_negative An array indicating weather or not the ray reciprocals are negative
    * @return An interaction struct containing the details of the closest intersection ray -> box
    */
    [[nodiscard]] Interaction Intersect(const Ray & ray,
        const glm::vec3 & reciprocals,
        const int is_reciprocal_negative[3]) const {
        // Extracting the reference to the current bounding box
        const BoundingBox3 & bounding_box = * this;

        // Computing the lambda value on X axis
        float min_lambda = (bounding_box[    is_reciprocal_negative[0]].x - ray.origin.x) * reciprocals.x;
        float max_lambda = (bounding_box[1 - is_reciprocal_negative[0]].x - ray.origin.x) * reciprocals.x;

        // Computing the lambda value on Y axis
        const float min_lambda_y = (bounding_box[    is_reciprocal_negative[1]].y - ray.origin.y) * reciprocals.y;
        const float max_lambda_y = (bounding_box[1 - is_reciprocal_negative[1]].y - ray.origin.y) * reciprocals.y;

        // Verifying intersection condition
        if (min_lambda > max_lambda_y || min_lambda_y > max_lambda)
            return {.hit = false,
               .distance = INFINITY
            };

        // Updating the min and max value of lambda
        if (min_lambda_y > min_lambda) min_lambda = min_lambda_y;
        if (max_lambda_y < max_lambda) max_lambda = max_lambda_y;

        // Computing the lambda value on Z axis
        const float min_lambda_z = (bounding_box[    is_reciprocal_negative[2]].z - ray.origin.z) * reciprocals.z;
        const float max_lambda_z = (bounding_box[1 - is_reciprocal_negative[2]].z - ray.origin.z) * reciprocals.z;

        // Verifying intersection condition
        if (min_lambda > max_lambda_z || min_lambda_z > max_lambda)
            return {.hit = false,
               .distance = INFINITY
            };

        // Updating the min and max value of lambda
        if (min_lambda_z > min_lambda) min_lambda = min_lambda_z;
        if (max_lambda_z < max_lambda) max_lambda = max_lambda_z;

        // Verifying that intersection is not behind the ray
        if(max_lambda < 0)
            return {.hit = false,
                .distance = INFINITY
            };

        // Returning the Hit struct
        return {
            .hit = true,
            // If min < 0 and max > 0, then the ray is within the bounding box
            .intersection = min_lambda < 0 ? ray.origin : ray.origin + min_lambda * ray.direction,
            .distance = min_lambda < 0 ? 0 : max_lambda
        };
    }

    /**
    * Function that given two bounding box, computes their union
    * @return A bounding box result of the union of the input
    */
    static BoundingBox3 Union(const BoundingBox3 & first_box, const BoundingBox3 & second_box) {
        // Computing the new minimum coordinates
        auto min_coordinates = glm::vec3(min(first_box.min_coordinates.x, second_box.min_coordinates.x),
                                         min(first_box.min_coordinates.y, second_box.min_coordinates.y),
                                         min(first_box.min_coordinates.z, second_box.min_coordinates.z));
        // Computing the new maximum coordinates
        auto max_coordinates = glm::vec3(max(first_box.max_coordinates.x, second_box.max_coordinates.x),
                                        max(first_box.max_coordinates.y, second_box.max_coordinates.y),
                                        max(first_box.max_coordinates.z, second_box.max_coordinates.z));

        return {min_coordinates, max_coordinates};
    }

    /**
    * Function that given a bounding box and a point computes their union
    * @return A bounding box result of the union of a bounding box and a point
    */
    static BoundingBox3 Union(const BoundingBox3 & bounding_box, const glm::vec3 & point) {
        // Computing the new minimum coordinates
        auto min_coordinates = glm::vec3(min(bounding_box.min_coordinates.x, point.x),
                                         min(bounding_box.min_coordinates.y, point.y),
                                         min(bounding_box.min_coordinates.z, point.z));
        // Computing the new maximum coordinates
        auto max_coordinates = glm::vec3(max(bounding_box.max_coordinates.x, point.x),
                                        max(bounding_box.max_coordinates.y, point.y),
                                        max(bounding_box.max_coordinates.z, point.z));

        return {min_coordinates, max_coordinates};
    }
};

inline void BoundingBox3Tests() {
    // Initializing the testing box
    const auto testing_box = BoundingBox3(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));

    // Initializing the testing ray
    const Ray testing_ray {
        .origin = glm::vec3(0, 0, -2),
        .direction = normalize(glm::vec3(1e-6, 1e-6, 1))
    };

    // Initializing auxiliary variables
    const glm::vec3 reciprocals(1 / testing_ray.direction.x,
        1 / testing_ray.direction.y, 1 / testing_ray.direction.z);
    int is_direction_negative[3] = {reciprocals.x < 0, reciprocals.y < 0, reciprocals.z < 0};

    // Ray should intersect
    if(!testing_box.DoesRayIntersect(testing_ray, reciprocals, is_direction_negative)) {
        cerr << "Test within the Bounding Box 3D class failed" << endl;
    }
    else {
        cout << "Ray intersection is correct" << endl;
    }
}


#endif //BOUNDING_BOX_H
