//
// Created by Guglielmo Mazzesi on 10/17/2024.
//

#ifndef MATH_H
#define MATH_H

/**
* Computes the fade value using the original fade function 6 * weight^5 - 15 * weight^4 + 10 * weight^3
*/
constexpr double Fade (const double weight) {
    return weight * weight * weight * (weight * (weight * 6 - 15) + 10);
}

/**
* Computes the linear interpolation of given numbers using the given weight
* @param first_number The first number
* @param second_number The second number
* @param weight The weight to be used in the linear interpolation
* @return A float result from interpolation the numbers
*/
constexpr double Lerp(const double first_number, const double second_number, const double weight) {
    return first_number + weight * (second_number - first_number);
}

/**
* Computes a smoothed interpolation of given numbers using the given linear weight
* @param first_number The first number
* @param second_number The second number
* @param weight The weight to be smoothed and used in the interpolation
* @return A float result from interpolation the numbers
*/
constexpr double SmoothLerp(const double first_number, const double second_number, const double weight) {
    // Applying a smoothing function to the weight
    const double new_weight = Fade(weight);

    // Applying linear interpolation
    return Lerp(first_number, second_number, new_weight);
}

/**
* Computes the linear interpolation of the given vectors using the given weight
* @param first_vector The first vector
* @param second_vector The second vector
* @param weight The weight to be used in the linear interpolation
* @return A vector result from linear interpolation the vectors
*/
constexpr glm::vec3 Lerp(const glm::vec3 & first_vector, const glm::vec3 & second_vector, const double weight) {
    // Computing each channel individually
    const float interpolated_x = Lerp(first_vector.x, second_vector.x, weight);
    const float interpolated_y = Lerp(first_vector.y, second_vector.y, weight);
    const float interpolated_z = Lerp(first_vector.z, second_vector.z, weight);

    // Returning the interpolated vector
    return {interpolated_x, interpolated_y, interpolated_z};
}

/**
* Computes a smoothed interpolation of given vectors using the given linear weight
* @param first_vector The first vector
* @param second_vector The second vector
* @param weight The weight to be smoothed and used in the interpolation
* @return A vector result from interpolation the numbers
*/
constexpr glm::vec3 SmoothLerp(const glm::vec3 & first_vector, const glm::vec3 & second_vector, const double weight) {
    // Applying a smoothing function to the weight
    const double new_weight = Fade(weight);

    // Applying linear interpolation to the vectors
    return Lerp(first_vector, second_vector, new_weight);
}

/**
* Map the provided value from the [0, 255] range to [0, 1] range. Values above 255 and below 0 are clamped.
* @param value A float in range [0, 255]
*/
constexpr float from255to1(const float value) {
    return max(0.0f, min(255.0f, value / 255.0f));
}

#endif //MATH_H
