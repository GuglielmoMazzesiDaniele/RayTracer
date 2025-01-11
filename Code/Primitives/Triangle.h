//
// Created by Guglielmo Mazzesi on 10/18/2024.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

using namespace std;

/**
* Class representing a triangle
*/
class Triangle : public Primitive {
    const Vertex * vertices;

    glm::vec3 triangle_cross_product; ///< Cross product used to computed barycentric coordinates

    float barycentric_coordinate_denominator; ///< Denominator used to compute barycentric coordinates

    bool smooth_shading = false; ///< Flag indicating if the triangle should use smooth shading

protected:

    static constexpr float epsilon = 1e-6; ///< Epsilon used in floating point computation to avoid errors

    const glm::vec3 normal; ///< The normal of the triangle

    void computeMinMaxGlobal() override {
        // Looking for the min and max in global coordinates system
        for(int i = 0; i < 3; i++){
            // Extracting the current vertex
            const glm::vec3 & vertex = * vertices[i].coordinates;

            // Transforming current vertex
            const auto global_vertex = transform * glm::vec4(vertex, 1);

            // Computing the new minimum coordinates
            min_global_coord = glm::vec3(min(global_vertex.x, min_global_coord.x),
                                             min(global_vertex.y, min_global_coord.y),
                                             min(global_vertex.z, min_global_coord.z));
            // Computing the new maximum coordinates
            max_global_coord = glm::vec3(max(global_vertex.x, max_global_coord.x),
                                            max(global_vertex.y, max_global_coord.y),
                                            max(global_vertex.z, max_global_coord.z));
        }
    }

    /**
    * Computes the intersections (if any) and stores
    */
    void computeIntersection(const Ray & global_ray, Interaction & interaction) {
        // Converting the localized_ray in local coordinates
        const Ray localized_ray = localizeRay(global_ray);

        // Dot products
        const float dot_normal_ray = dot(localized_ray.direction, this->normal);

        // Verifying that ray and plane are not parallels
        if(abs(dot_normal_ray) == epsilon)
            return;

        // Computing the lambda (ie ray = ray_origin + lambda * ray_direction)
        const float lambda = dot(this->normal, (* vertices[0].coordinates - localized_ray.origin)) / dot_normal_ray;

        // If lambda is negative, then intersection point is behind the ray origin
        if(lambda < 0)
            return;

        // Computing the plane intersection point
        const glm::vec3 intersection = localized_ray.origin + lambda * localized_ray.direction;

        // Initializing array of barycentric coordinates
        float barycentric_coord[3];

        // Computing the barycentric coordinates for all vertices
        for(int i = 0; i < 3; i++) {
            // Computing the cross product of the sub triangles
            const glm::vec3 cross_product = cross(* vertices[(i + 1) % 3].coordinates - intersection,
                * vertices[(i + 2) % 3].coordinates - intersection);

            // Computing the barycentric value for current_vertex
            barycentric_coord[i] = dot(triangle_cross_product, cross_product) * barycentric_coordinate_denominator;

            // If the barycentric coordinate is negative, no intersection with the triangle
            if(barycentric_coord[i] < -epsilon)
                return;
        }

        // Initializing the intersection details
        glm::vec3 normal = this->normal;
        glm::vec2 uv_coordinates = this->vertices[0].uv_coordinates ?
            (barycentric_coord[0] * * vertices[0].uv_coordinates
                + barycentric_coord[1] * * vertices[1].uv_coordinates
                + barycentric_coord[2] * * vertices[2].uv_coordinates) : glm::vec2(0.0f);

        // Case in which smooth shading is active
        if(smooth_shading) {
            // Case in which a normal map use active
            if(this->normal_map) {
                // Computing the interpolated tangent, bitangent and normal tangent space matrix
                const glm::mat3 tangent_space_basis (
                    barycentric_coord[0] * vertices[0].tangent
                        + barycentric_coord[1] * vertices[1].tangent
                        + barycentric_coord[2] * vertices[2].tangent,
                    barycentric_coord[0] * vertices[0].bitangent
                        + barycentric_coord[1] * vertices[1].bitangent
                        + barycentric_coord[2] * vertices[2].bitangent,
                    barycentric_coord[0] * vertices[0].normal
                        + barycentric_coord[1] * vertices[1].normal
                        + barycentric_coord[2] * vertices[2].normal
                    );

                // Extracting the data from the normal map
                const glm::vec3 normal_map_data = 2.0f * normal_map->getPixel(uv_coordinates) - glm::vec3(1.0f);

                // Computing the new normal based on the normal map
                normal = normalize(tangent_space_basis * normal_map_data);
            }
            // Case in which Phong shading is to be used
            else {
                normal = normalize(barycentric_coord[0] * vertices[0].normal
                            + barycentric_coord[1] * vertices[1].normal
                            + barycentric_coord[2] * vertices[2].normal);
            }
        }

        // Updating the value within the provided interaction
        interaction.hit = true;
        interaction.normal = normal;
        interaction.uv_coordinates = uv_coordinates;
        interaction.intersection = intersection;
        interaction.primitive = this;
        interaction.material = material;

        // De-localizing the interaction
        delocalizeInteraction(interaction, global_ray.origin);
    }

public:
    /**
    * Constructor that initialized the triangle without the vertices normals
    * @param transform The triangle transform
    * @param vertices 3 sized array of vertices
    * @param phong_shading A boolean indicating weather or not use the phong shading
    * @param material Pointer to the triangle material
    * @param albedo Pointer to the albedo texture
    * @param normal Pointer to the normal map
    * @param AO_R_M Pointer to the ambient occlusion/roughness/metallix texture
    */
    Triangle(const glm::mat4 & transform,
        const Vertex vertices [3],
        const bool phong_shading = false,
        const Material * material = nullptr,
        const Texture * albedo = nullptr,
        const Texture * normal = nullptr,
        const Texture * AO_R_M = nullptr

    )
    : Primitive(transform, material, albedo, normal, AO_R_M),
    vertices(vertices),
    triangle_cross_product(cross(* vertices[1].coordinates - * vertices[0].coordinates,
        * vertices[2].coordinates - * vertices[0].coordinates)),
    smooth_shading(phong_shading),
    normal(normalize(triangle_cross_product)){
        // Computing the denominator used to compute the value of each barycentric coordinate
        barycentric_coordinate_denominator = 1.0f / pow(length(triangle_cross_product), 2);

        // Populating the minimum and max local coordinates
        for (int i = 0; i < 3; i++) {
            const glm::vec3 & vertex_coordinates = * vertices[i].coordinates;
            min_local_coord.x = glm::min(min_local_coord.x, vertex_coordinates.x);
            min_local_coord.y = glm::min(min_local_coord.y, vertex_coordinates.y);
            min_local_coord.z = glm::min(min_local_coord.z, vertex_coordinates.z);

            max_local_coord.x = glm::max(max_local_coord.x, vertex_coordinates.x);
            max_local_coord.y = glm::max(max_local_coord.y, vertex_coordinates.y);
            max_local_coord.z = glm::max(max_local_coord.z, vertex_coordinates.z);
        }

        // Computing the global coordinates
        Triangle::computeMinMaxGlobal();
    }

    /**
    *  Function that verifies if a global ray intersects the triangle
    *  @param global_ray Ray in global coordinates
    *  @param interaction The interaction struct
    */
    void Intersect(const Ray & global_ray, Interaction & interaction) override {
        // Computing the intersection
        computeIntersection(global_ray, interaction);
    }

    /**
    * Return the tangent for this primitive with respect to the given normal
    */
    [[nodiscard]] glm::vec3 computeTangent(glm::vec3 normal, const glm::vec3 surface_point) override {
        // Computing the coordinates for the tangent
        const float tangent_x = this->max_global_coord.x - this->min_global_coord.x;
        const float tangent_y = this->max_global_coord.y - this->min_global_coord.y;
        const float tangent_z = this->max_global_coord.z - this->min_global_coord.z;

        return normalize(glm::vec3(0, tangent_y, tangent_z));
    }
};

#endif //TRIANGLE_H
