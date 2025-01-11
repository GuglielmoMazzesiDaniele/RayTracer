//
// Created by Guglielmo Mazzesi on 11/20/2024.
//

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

/**
 General class for the primitive
 */
class Primitive : public Entity {
protected:
    glm::mat4 normal_matrix; ///< Inverse matrix used to globalize normals

    glm::vec3 min_local_coord = glm::vec3(INFINITY); ///< Entity minimum local coordinates (used in some computations)
    glm::vec3 max_local_coord = glm::vec3(-INFINITY); ///< Entity maximum local coordinates (used in some computations)

    glm::vec3 min_global_coord = glm::vec3(INFINITY); ///< Entity minimum global coordinates (used in bounding boxes)
    glm::vec3 max_global_coord = glm::vec3(-INFINITY); ///< Entity maximum global coordinates (used in bounding boxes)

    const Material * material; ///< Structure describing the material of the primitive

    const Texture * albedo_texture; ///< Pointer to the albedo texture
    const Texture * normal_map; ///< Pointer to the normal map
    const Texture * AO_R_M_texture; ///< Pointer to the ambient occlusion/roughness/metal

    virtual void computeMinMaxGlobal() {
        // Transforming the local minimum and maximum in global coordinates
        const auto temp_1 = transform * glm::vec4(min_local_coord, 1);;
        const auto temp_2 = transform * glm::vec4(max_local_coord, 1);

        // Computing the new minimum coordinates
        min_global_coord = glm::vec3(min(temp_1.x, temp_2.x),
                                         min(temp_1.y, temp_2.y),
                                         min(temp_1.z, temp_2.z));

        // Computing the new maximum coordinates
        max_global_coord = glm::vec3(max(temp_1.x, temp_2.x),
                                        max(temp_1.y, temp_2.y),
                                        max(temp_1.z, temp_2.z));
    }

    /**
     * Function that converts a given global_ray in primitive local coordinate system
     * @param global_ray The global_ray
     * @return The given ray localized with the primitive local coordinate system
     */
    [[nodiscard]] Ray localizeRay(Ray global_ray) const {
        // Converting the direction in homogenous coordinates and applying the inverse transform (does nothing)
        global_ray.direction = inverse_transform * glm::vec4(global_ray.direction, 0);
        global_ray.direction = normalize(global_ray.direction);
        // Converting the origin in homogenous coordinates and applying the inverse transform
        global_ray.origin= inverse_transform * glm::vec4(global_ray.origin, 1);
        return global_ray;
    }

    /**
     * Function that converts a localized ray to its counterpart in global coordinate system
     * @param interaction The hit primitive
     * @param ray_origin The ray origin in global coordinates
     * @return The given hit struct expressed in global coordinate system
     */
    void delocalizeInteraction(Interaction & interaction, const glm::vec3 & ray_origin) const {
        // Converting the intersection point in global coordinates
        interaction.intersection = transform * glm::vec4(interaction.intersection, 1);

        // Converting the intersection normal in global coordinates
        interaction.normal = normal_matrix * glm::vec4(interaction.normal, 0);
        interaction.normal = normalize(interaction.normal);

        // Computing the new distance
        interaction.distance = distance(ray_origin, interaction.intersection);
    }

public:

    /** A function computing an intersection, which returns the structure Hit */
    virtual void Intersect(const Ray & ray, Interaction & interaction) = 0;

    virtual glm::vec3 computeTangent(glm::vec3 normal, glm::vec3 surface_point) = 0;

    /**
     * Default constructor of a primitive
     * @param transform Transform of the sphere
     * @param material Material of the sphere
     * @param albedo Pointer to the albedo texture
     * @param normal Pointer to the normal map
     * @param AO_R_M Pointer to the ambient occlusion/roughness/metallix texture
     */
    explicit Primitive(
        const glm::mat4 & transform,
        const Material * material = nullptr,
        const Texture * albedo = nullptr,
        const Texture * normal = nullptr,
        const Texture * AO_R_M = nullptr)
    : Entity(transform),
    normal_matrix(transpose(inverse_transform)),
    material(material),
    albedo_texture(albedo),
    normal_map(normal),
    AO_R_M_texture(AO_R_M){
    }

    /**
    * Returns the diffuse coefficients at the given uv coordinates
    * @param UV 2 sized vector containing a surface point expressed in UV coordinates
    */
    [[nodiscard]] glm::vec3 getDiffuse(const glm::vec2 & UV) const {
        return this->albedo_texture ? this->albedo_texture->getPixel(UV) : this->material->diffuse;
    }

    /**
    * Returns the roughness coefficients at the given UV coordinates
    * @param UV 2 sized vector containing a surface point expressed in UV coordinates
    */
    [[nodiscard]] float getRoughness(const glm::vec2 UV) const {
        return this->AO_R_M_texture ? this->AO_R_M_texture->getPixel(UV).r : this->material->roughness;
    }

    /**
    * Returns the roughness coefficients at the given UV coordinates
    * @param UV 2 sized vector containing a surface point expressed in UV coordinates
    */
    [[nodiscard]] float getMetallic(const glm::vec2 UV) const {
        return this->AO_R_M_texture ? this->AO_R_M_texture->getPixel(UV).g : this->material->roughness;
    }

    /**
    * Returns the ambient occlusion coefficients at the given UV coordinates
    * @param UV 2 sized vector containing a surface point expressed in UV coordinates
    */
    [[nodiscard]] float getAmbientOcclusion(const glm::vec2 UV) const {
        return this->AO_R_M_texture ? this->AO_R_M_texture->getPixel(UV).b : 1.0f;
    }


    /**
    * Function that returns the primitive bounding box in world space
    */
    BoundingBox3 getWorldSpaceBoundingBox () {
        return {min_global_coord, max_global_coord};
    }

    /**
    * Returns the centroid of the primitive
    */
    [[nodiscard]] glm::vec3 getCentroid() const {
        return 0.5f * min_global_coord + 0.5f * max_global_coord;
    }

    /**
     * Function that returns the material struct of the primitive
     */
    [[nodiscard]] Material getMaterial() const {
        return * this->material;
    }

    /**
    * Getter of the
    */
    [[nodiscard]] virtual Material getMaterial(const glm::vec3 point) {
        return * this->material;
    }

    /**
    * Getter of the triangles minimum coordinates
    */
    [[nodiscard]] glm::vec3 getLocalMinCoord() const {
        return this->min_local_coord;
    }

    /**
    * Getter of the triangles minimum coordinates
    */
    [[nodiscard]] glm::vec3 getMaxLocalCoord() const {
        return this->max_local_coord;
    }

    /**
    * Setter for the plane global max coordinates
    * @param new_max_global_coordinates Vec3 containing the new coordinates
    */
    void setGlobalMaxCoordinates(const glm::vec3 & new_max_global_coordinates) {
        this->max_global_coord = new_max_global_coordinates;
    }

    /**
    * Setter for the plane global min coordinates
    * @param new_min_global_coordinates Vec3 containing the new coordinates
    */
    void setGlobalMinCoordinates(const glm::vec3 & new_min_global_coordinates) {
        this->min_global_coord = new_min_global_coordinates;
    }
};

#endif //PRIMITIVE_H
