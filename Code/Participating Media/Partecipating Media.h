//
// Created by Guglielmo Mazzesi on 12/8/2024.
//

#ifndef PARTICIPATING_MEDIA_H
#define PARTICIPATING_MEDIA_H

class ParticipatingMedia : public Entity {

    float interaction_coefficient; ///< The alpha coefficient used in the interaction probability distribution

public:
    /**
    * Computed the probability that a ray travelling through the participating media for the given distance does
    * not interact with it
    * @param distance The distance travelled into the medium
    * @param interaction_coefficient The interaction coefficient of the probability function
    * @return A float in range [0, 1] indicating the probability the ray travelled undisturbed within the medium
    */
    [[nodiscard]] static float computeInteractionProbability(const float distance, const float interaction_coefficient) {
        return exp(-distance * interaction_coefficient);
    }

    ParticipatingMedia (const glm::mat4 & transform, const float interaction_coefficient)
        : Entity(transform), interaction_coefficient(interaction_coefficient) {
    }
};

#endif //PARTICIPATING_MEDIA_H
