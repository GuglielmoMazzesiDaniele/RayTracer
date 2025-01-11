//
// Created by Guglielmo Mazzesi on 11/9/2024.
//

#ifndef RANDOM_H
#define RANDOM_H


class RandomGenerator {
protected:
    static RandomGenerator * singleton_instance;

    RandomGenerator() {
        singleton_instance = this;
        std::srand(std::time(nullptr));
    }

public:

    static RandomGenerator * getInstance();

    int getRandomInt() {
        return rand();
    }

    /**
    * Returns a flot in range [0. 1]
    */
    float getRandomFloat() {
        return rand() / (float)RAND_MAX;
    }
};

RandomGenerator * RandomGenerator::singleton_instance = nullptr;

/**
* Return the singleton instance of the random generator
*/
inline RandomGenerator * RandomGenerator::getInstance()  {
    if(singleton_instance == nullptr)
        singleton_instance = new RandomGenerator();
    return singleton_instance;
}

/**
* Returns a random direction within the given sphere radius
* @param sphere_radius The sphere radius
*/
inline glm::vec3 generateSphericalRandomDirection(const float sphere_radius) {
    // Generating pseudo random coordinates, used to create the random direction
    const float x = ((float)RandomGenerator::getInstance()->getRandomInt() - (float)RAND_MAX / 2) / (float)RAND_MAX;
    const float y = ((float)RandomGenerator::getInstance()->getRandomInt() - (float)RAND_MAX / 2) / (float)RAND_MAX;
    const float z = ((float)RandomGenerator::getInstance()->getRandomInt() - (float)RAND_MAX / 2) / (float)RAND_MAX;

    // Initializing the ray
    glm::vec3 random_direction = glm::vec3(x, y, z);

    // Normalizing the vector and multiplying it for the sphere radius
    random_direction = normalize(random_direction) * sphere_radius;

    // Returning the vector
    return random_direction;
}

#endif //RANDOM_H
