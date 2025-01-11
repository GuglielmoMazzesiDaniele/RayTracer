//
// Created by Guglielmo Mazzesi on 11/20/2024.
//

#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "../Auxiliary/Printing.h"
#include "../Bounds/Bounding Box 3D.h"
#include "../Primitives/Plane.h"

/**
* Implementation of the BVH SDS, used to accelerate the ray intersections in the main loop
*/
class BVH {
    // Struct used to store information regarding a primitive
    struct BVHPrimitiveInfo {
        /**
        * Default constructor
        */
        BVHPrimitiveInfo(const size_t primitive_index, const BoundingBox3 & bounding_box)
            : primitive_index(primitive_index), bounding_box(bounding_box),
              centroid(.5f * bounding_box.min_coordinates + .5f * bounding_box.max_coordinates) { }
        size_t primitive_index; ///< The primitive index in the global vector of primitives
        BoundingBox3 bounding_box; ///< The primitive bounding box
        glm::vec3 centroid; ///< The primitive centroid
    };

    // Struct used to represent a node
    struct BVHNode {
        /**
        * Function that initializes leaves
        */
        void initializeLeaf(int index, int amount, const BoundingBox3 & bounding) {
            first_primitive_index = index;
            primitives_amount = amount;
            bounding_box = bounding;
            children[0] = children[1] = nullptr;
        }
        /**
        * Function that initializes an internal node
        */
        void initializeInternal(int axis, BVHNode * left_child, BVHNode * right_child) {
            children[0] = left_child;
            children[1] = right_child;
            bounding_box = BoundingBox3::Union(left_child->bounding_box, right_child->bounding_box);
            split_axis = axis;
            primitives_amount = 0;
        }
        BoundingBox3 bounding_box; ///< The bounding box wrapping this node
        BVHNode * children[2]; ///< The left and right children of the node
        int split_axis; ///< The split axis used for separating the children (0 = x, 1 = y, 2 = z)
        int first_primitive_index; ///< The index of the first primitives within the global vector
        int primitives_amount; ///< The amount of primitives contained in this node
    };

    // Struct used to represent a node in the linear BVH
    struct LinearBVHNode {
        BoundingBox3 bounding_box; ///< The bounding box of this node
        union {
            int first_primitive_index = -2; ///< An index offset to the first primitive belonging to this leaf
            int second_child_offset; ///< An index offset to the second child of this internal node
        };
        uint16_t primitives_amount; ///< Primitives contained in this leaf (0 if internal node)
        uint8_t split_axis; ///< Axis used to split the primitives within this node
    };

    // Internal enum used to distinguish between traversal mode
    enum traversal_mode {
        FIRST_WITHIN_DISTANCE,
        FIRST_NOT_TRANSPARENT_WITHIN_DISTANCE,
        CLOSEST
    };

    /**
    * Function that build a BVH handling the primitives within range [start_index, end_index)
    * @param primitives_info Vector containing information regarding the primitives
    * @param start_index The index of the first primitive included in the node
    * @param end_index The index of the first primitive not included in the node
    * @param total_nodes Number of nodes created
    * @param ordered_primitives Vector containing primitives ordered based on leaf creation order
    * @return The root node of the BVH tree
    */
    BVHNode * buildNode(vector<BVHPrimitiveInfo> & primitives_info, int start_index, int end_index,
        int * total_nodes, vector<Primitive *> & ordered_primitives) {
        // Verifying that there are primitives in the scene
        if (primitives.empty())
            return nullptr;

        // Allocating new node
        // TODO: This is a memory leak, fix at some point in the future (tree deconstruction?)
        auto current_node = new BVHNode();

        // Increasing the total amount of node created
        (*total_nodes)++;

        // Creating the bounding box that wraps the entire node
        BoundingBox3 bounding_box;
        for(int i = start_index; i < end_index; i++)
            bounding_box = BoundingBox3::Union(bounding_box, primitives_info[i].bounding_box);

        // cout << "X axis: " << 0.5f * bounding_box.max_coordinates.x + 0.5f * bounding_box.min_coordinates.x << endl;
        // cout << "Y axis: " << 0.5f * bounding_box.max_coordinates.y + 0.5f * bounding_box.min_coordinates.y << endl;
        // cout << "Z axis: " << 0.5f * bounding_box.max_coordinates.z + 0.5f * bounding_box.min_coordinates.z << endl;

        // Computing the amount of primitives in this node
        int primitives_amount = end_index - start_index;

        // Case in which I reached a leaf node
        if (primitives_amount == 1) {
            // Extracting the first free spot in the ordered primitives vector
            int first_primitive_index = ordered_primitives.size();

            // Pushing all the primitives in the ordered vector
            for(int i = start_index; i < end_index; i++) {
                int primitive_index = primitives_info[i].primitive_index;
                ordered_primitives.push_back(primitives[primitive_index]);
            }

            // Initializing the leaf
            current_node->initializeLeaf(first_primitive_index, primitives_amount, bounding_box);

            return current_node;
        }
        // Case in which I reached an internal node
        else {
            // Computing the bounding box wrapping the centroids of all primitives
            BoundingBox3 centroids_bounding_box;
            for(int i = start_index; i < end_index; i++)
                centroids_bounding_box = BoundingBox3::Union(centroids_bounding_box, primitives_info[i].centroid);

            // Computing which axis has the maximum extend on the centroids bounding box (0 = x, 1 = y, 2 = z)
            int most_extended_axis = centroids_bounding_box.getMaximumExtend();

            // Computing the middle index
            int middle_index = (start_index + end_index) / 2;;

            // Case in which the centroids bounding box has volume 0 (a leaf is created)
            if(centroids_bounding_box.max_coordinates[most_extended_axis]
                == centroids_bounding_box.min_coordinates[most_extended_axis]) {
                // Extracting the first free spot in the ordered primitives vector
                int first_primitive_index = ordered_primitives.size();

                // Pushing all the primitives in the ordered vector
                for(int i = start_index; i < end_index; i++) {
                    int primitive_index = primitives_info[i].primitive_index;
                    ordered_primitives.push_back(primitives[primitive_index]);
                }

                // Initializing the leaf
                current_node->initializeLeaf(first_primitive_index, primitives_amount, bounding_box);

                return current_node;
            }

            switch(SPLIT_METHOD) {
                case MIDDLE: {
                    // Computing the middle point on the most extended axis of the centroids bounding box
                    float middle_point = (centroids_bounding_box.min_coordinates[most_extended_axis]
                        + centroids_bounding_box.max_coordinates[most_extended_axis]) / 2;

                    // Sorting the primitives belonging to this node based on their centroid position relative
                    // to the middle point (within the centroids bounding box)
                    BVHPrimitiveInfo * middle_pointer = std::partition(& primitives_info[start_index],
                        & primitives_info[end_index - 1] + 1,
                        [most_extended_axis, middle_point](const BVHPrimitiveInfo & primitive_info) {
                            return primitive_info.centroid[most_extended_axis] < middle_point;
                        });

                    // Reassigning the middle index based on recent sorting process
                    middle_index = middle_pointer - & primitives_info[0];

                    // Verifying that the sorting processes did not create an unbalanced split
                    if(middle_index != start_index && middle_index != end_index)
                        break;
                }
                case EQUAL_COUNTS: {
                    // Computing the middle index
                    middle_index = (start_index + end_index) / 2;

                    // Sorting the primitives in a way that all primitives to the left of middle index have smaller
                    // centroid[most_extended_axis] than all primitives to its right
                    nth_element(& primitives_info[start_index],
                        & primitives_info[middle_index],
                        & primitives_info[end_index - 1] + 1,
                        [most_extended_axis](const BVHPrimitiveInfo & first, const BVHPrimitiveInfo & second) {
                            return first.centroid[most_extended_axis] < second.centroid[most_extended_axis];
                        });
                    break;
                }
                case SAH:
                default: {
                    // Case in which the number of primitives is too small to be worth SAH computational cost
                    // Using equal counts instead
                    if(primitives_amount == 4) {
                        // Computing the middle index
                        middle_index = (start_index + end_index) / 2;

                        // Sorting the primitives in a way that all primitives to the left of middle index have smaller
                        // centroid[most_extended_axis] than all primitives to its right
                        nth_element(& primitives_info[start_index],
                            & primitives_info[middle_index],
                            & primitives_info[end_index - 1] + 1,
                            [most_extended_axis](const BVHPrimitiveInfo & first, const BVHPrimitiveInfo & second) {
                                return first.centroid[most_extended_axis] < second.centroid[most_extended_axis];
                            });
                    }
                    // Case in which there are enough primitives to use SAH
                    else {
                        // Struct representing a bucket, used to split primitives based on their centroids
                        struct BucketInfo {
                            int primitives_amount = 0; ///< Amount of primitives stored in the bucket
                            BoundingBox3 bounding_box; ///< Bounding box wrapping primitives within the buket
                        };

                        // Allocating the array of buckets
                        BucketInfo buckets[SAH_BUCKETS_AMOUNT];

                        // Initializing the buckets
                        for(int i = start_index; i < end_index; i++) {
                            // Computing the index of the bucket containing the current primitive
                            int primitive_bucket_index = SAH_BUCKETS_AMOUNT *
                                centroids_bounding_box.getOffset(primitives_info[i].centroid)[most_extended_axis];

                            // Flooring the index to the lengths of buckets - 1
                            if(primitive_bucket_index == SAH_BUCKETS_AMOUNT)
                                primitive_bucket_index = SAH_BUCKETS_AMOUNT - 1;

                            // Updating the bucket
                            buckets[primitive_bucket_index].primitives_amount++;
                            buckets[primitive_bucket_index].bounding_box =
                                BoundingBox3::Union(buckets[primitive_bucket_index].bounding_box,
                                                                primitives_info[i].bounding_box);
                        }

                        // Allocating array of buckets' cost
                        float buckets_cost[SAH_BUCKETS_AMOUNT - 1];

                        // Computing cost of each bucket
                        for(int i = 0; i < SAH_BUCKETS_AMOUNT - 1; i++) {
                            // Initializing the left and right bounding box (B & C)
                            BoundingBox3 left_box, right_box;

                            // Initializing auxiliary variables
                            int left_count = 0, right_count = 0;

                            // Initializing left area (B)
                            for(int j = 0; j <= i; j++) {
                                // Creating the left bounding box
                                left_box = BoundingBox3::Union(left_box, buckets[j].bounding_box);
                                // Increasing the amount of primitives in left box
                                left_count += buckets[j].primitives_amount;
                            }

                            // Initializing right area (C)
                            for(int j = i + 1; j < SAH_BUCKETS_AMOUNT; j++) {
                                // Creating the left bounding box
                                right_box = BoundingBox3::Union(right_box, buckets[j].bounding_box);
                                // Increasing the amount of primitives in left box
                                right_count += buckets[j].primitives_amount;
                            }

                            // Computing the cost of this bucket
                            buckets_cost[i] = .125f
                            + (static_cast<float>(left_count) * left_box.getSurfaceArea() +
                                static_cast<float>(right_count) * right_box.getSurfaceArea())
                                / bounding_box.getSurfaceArea();
                        }

                        // Finding the least expensive bucket (basic algorithm, I will not comment)
                        float min_cost = buckets_cost[0];
                        int least_expensive_bucket_index = 0;
                        for(int i = 1; i < SAH_BUCKETS_AMOUNT - 1; i++) {
                            if(buckets_cost[i] < min_cost) {
                                min_cost = buckets_cost[i];
                                least_expensive_bucket_index = i;
                            }
                        }

                        // Deciding what to create between a leaf or an internal node based on computed cost

                        // Traversing a primitive has arbitrary cost O(1), so left cost = amount of primitives
                        float leaf_cost = primitives_amount;

                        // Case in which I create an internal node
                        if(min_cost < leaf_cost) {
                            // Computing the middle pointer
                            BVHPrimitiveInfo * middle_pointer = std::partition(& primitives_info[start_index],
                                & primitives_info[end_index - 1] + 1, [=](const BVHPrimitiveInfo & primitive_info) {
                                    // TODO: Improve by storing the indexes and passing them to the lambda function?
                                    // Computing the index of the bucket containing the current primitive
                                    int primitive_index_bucket = SAH_BUCKETS_AMOUNT *
                                       centroids_bounding_box.getOffset(primitive_info.centroid)[most_extended_axis];

                                    // Flooring the index to the lengths of buckets - 1
                                    if (primitive_index_bucket == SAH_BUCKETS_AMOUNT)
                                        primitive_index_bucket = SAH_BUCKETS_AMOUNT - 1;

                                    // Returning true if primitive is on the left side of the least expensive bucket
                                    return primitive_index_bucket <= least_expensive_bucket_index;
                                });

                            // Reassigning the middle index based on recent sorting process
                            middle_index = middle_pointer - & primitives_info[0];
                        }
                        // Case in which I create a leaf
                        else {
                            // Extracting the primitive index
                            int first_primitive_index = ordered_primitives.size();
                            // Pushing the primitives on the ordered primitives array
                            for(int i = start_index; i < end_index; i++) {
                                int primitive_index = primitives_info[i].primitive_index;
                                ordered_primitives.push_back(primitives[primitive_index]);
                            }
                            // Initializing the leaf
                            current_node->initializeLeaf(first_primitive_index, primitives_amount, bounding_box);
                            // Returning the leaf
                            return current_node;
                        }
                    }
                    break;
                }
            }

            // Building the internal node by recursively calling the buildNode function
            current_node->initializeInternal(most_extended_axis,
                buildNode(primitives_info, start_index, middle_index, total_nodes, ordered_primitives),
                buildNode(primitives_info, middle_index, end_index, total_nodes, ordered_primitives));
        }
        return current_node;
    }

    /**
    * Function that flattens the BVH Tree for faster traversal
    * @param current_node The current node to be flattened (provide root to start process)
    * @param offset Index offset used to link internal nodes with children
    * @return The created linear node offset
    */
    int flattenBVHTree(const BVHNode * current_node, int * offset) {
        // Initializing the current linear node
        LinearBVHNode * current_linear_node = & linear_nodes[*offset];

        // Assigning the bounding box
        current_linear_node->bounding_box = current_node->bounding_box;

        // Increasing the offset
        int current_node_offset = (*offset)++;

        // Case in which the current node is a leaf
        if(current_node->primitives_amount > 0) {
            // Extracting data from the tree leaf
            current_linear_node->first_primitive_index = current_node->first_primitive_index;
            current_linear_node->primitives_amount = current_node->primitives_amount;
        }
        // Case in which the current node is an internal node
        else {
            // Setting primitives to 0, since current node is an internal node
            current_linear_node->primitives_amount = 0;
            // Extracting data from the tree leaf
            current_linear_node->split_axis = current_node->split_axis;
            // Flattening the left child (DFS approach)
            flattenBVHTree(current_node->children[0], offset);
            // Flattening the right child and storing his offset
            current_linear_node->second_child_offset = flattenBVHTree(current_node->children[1], offset);
        }
        return current_node_offset;
    }

    [[nodiscard]] Interaction traversal(const Ray & ray, traversal_mode mode, const float max_distance) const{
        // Initializing the hit struct
        Interaction closest_interaction {
            .hit = false,
            .distance = INFINITY,
        };

        // Initializing static variables
        const glm::vec3 reciprocals(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);
        const int is_direction_negative[3] = {reciprocals.x < 0, reciprocals.y < 0, reciprocals.z < 0};

        // Offset used
        int to_visit_offset = 0;
        int current_index = 0;
        // An array used as FIFO stack containing the nodes to visit
        int nodes_to_visit[64];

        // Iterating the BVH
        while(true && !primitives.empty()) {
            // Extracting the current node from the array
            const LinearBVHNode * current_node = & linear_nodes[current_index];

            // Computing the Hit struct with the bounding box
            Interaction box_intersection = current_node->bounding_box.Intersect(ray, reciprocals, is_direction_negative);

            // Case in which the ray intersects the node's bounding box
            // if(box_intersection.hit && box_intersection.distance <= closest_interaction.distance) {
            if(current_node->bounding_box.DoesRayIntersect(ray, reciprocals, is_direction_negative)) {
                // Case in which the node is a leaf
                if(current_node->primitives_amount > 0) {
                    // Iterating all primitives
                    for(int i = 0; i < current_node->primitives_amount; i++) {
                        // Initializing the tentative interaction with the current primitive
                        Interaction tentative_interaction {
                            .hit = false,
                            .distance = INFINITY
                        };

                        // Computing the intersection with the primitive
                        primitives[current_node->first_primitive_index + i]->Intersect(ray, tentative_interaction);

                        // Verifying if the hit is valid and better than the current closest hit
                        if(tentative_interaction.hit) {
                            switch(mode) {
                                case FIRST_NOT_TRANSPARENT_WITHIN_DISTANCE: {
                                    if(tentative_interaction.distance < max_distance
                                        && tentative_interaction.material->transparency < 1.0f)
                                        return tentative_interaction;
                                    break;
                                }
                                case FIRST_WITHIN_DISTANCE: {
                                    if(tentative_interaction.distance <= max_distance)
                                        return tentative_interaction;
                                    break;
                                }
                                case CLOSEST:
                                default: {
                                    if(tentative_interaction.distance < closest_interaction.distance)
                                        closest_interaction = tentative_interaction;
                                    break;
                                }
                            }
                        }
                    }

                    // Verifying if stack is empty
                    if (to_visit_offset == 0)
                        break;

                    // Popping current node from the stack
                    current_index = nodes_to_visit[--to_visit_offset];
                }
                // Case in which the node is an internal node
                else {
                    // Case in which the ray will intersect the second child first
                    if(is_direction_negative[current_node->split_axis]) {
                        // Pushing the first child in the stack
                        nodes_to_visit[to_visit_offset++] = current_index + 1;
                        // Setting next node to iterate to be the second one
                        current_index = current_node->second_child_offset;
                    }
                    // Case in which the ray will intersect the first child first
                    else {
                        // Pushing second child in the stack
                        nodes_to_visit[to_visit_offset++] = current_node->second_child_offset;
                        // Setting the next node to iterate to be the first child
                        current_index = current_index + 1;
                    }
                }
            }
            // Case in which the ray does not intersect the node's bounding box
            else {
                // Case in which stack is empty
                if (to_visit_offset == 0)
                    break;
                // Popping current node from the stack
                current_index = nodes_to_visit[--to_visit_offset];
            }
        }

        for(auto & plane : planes) {
            // Initializing the tentative interaction
            Interaction tentative_interaction {
                .hit = false,
                .distance = INFINITY
            };

            // Computing the tentative interaction
            plane->Intersect(ray, tentative_interaction);

            // Updating if necessary
            if(tentative_interaction.hit && tentative_interaction.distance <= closest_interaction.distance)
                closest_interaction = tentative_interaction;
        }

        return closest_interaction;
    }

    vector<BVHPrimitiveInfo> primitives_info; ///< Vector of primitive info, used to create the BVH
    vector<Primitive *> ordered_primitives;
    int total_nodes = 0;
    BVHNode * root = nullptr;
    LinearBVHNode * linear_nodes = nullptr;

public:

    /**
    * Default constructor
    */
    BVH() {
        // Verifying that there are primitives in the scene
        if(primitives.size() == 0) {
            PrintError("There are not primitives in the scene, cannot build BVH");
            return;
        }


        // Clock time at the beginning of the BVH construction
        clock_t current_time = clock();

        // Initializing the primitives info
        primitives_info.reserve(primitives.size());

        // Building the array of primitives information
        for(size_t i = 0; i < primitives.size(); i++)
            primitives_info.emplace_back(i, primitives[i]->getWorldSpaceBoundingBox());

        // Printing the execution time
        if(PRINT_SDS_BUILDING_TIME)
            cout << "Starting construction of the BVH..." << endl;

        // Building the BVH
        root = buildNode(primitives_info, 0,
            primitives.size(), & total_nodes, ordered_primitives);

        // Swapping the global primitives for the ordered primitives
        primitives.swap(ordered_primitives);

        // Flattening the BVH
        int flattening_offset = 0;
        linear_nodes = new LinearBVHNode[total_nodes];
        flattenBVHTree(root, & flattening_offset);

        // Computing the execution time
        current_time = clock() - current_time;

        // Printing the execution time
        if(PRINT_SDS_BUILDING_TIME)
            cout << "It took " << ((float)current_time)/CLOCKS_PER_SEC<< " seconds (" <<
                (float)current_time / CLOCKS_PER_SEC / 60 << " minutes) to build the BVH"<< endl << endl;


        // TODO: Tree deconstruction
    }

    /**
    * Reset the BVH at factory settings
    */
    void clear() {
        this->primitives_info.clear();
        this->ordered_primitives.clear();
        this->total_nodes = 0;
        this->root = nullptr;
        this->linear_nodes = nullptr;
    }

    /**
    * Function that given a ray returns the closest intersection with a primitive
    * @param ray A ray expressed in global coordinates
    * @return Hit struct containing all the data regarding the intersection
    */
    [[nodiscard]] Interaction intersect(const Ray & ray) const {
        return traversal(ray, CLOSEST, INFINITY);
    }

    /**
    * Function that given a ray returns the first intersection within the given distance (included)
    * @param ray A ray expressed in global coordinates
    * @param max_distance The max distance between the ray origin and the found intersection point
    * @return Hit struct containing all the data regarding the intersection
    */
    [[nodiscard]] Interaction intersectWithinDistance(const Ray & ray, const float max_distance) const {
        return traversal(ray, FIRST_WITHIN_DISTANCE, max_distance);
    }

    /**
    * Function that given a ray returns the first intersection with non trasparent object
    * within the given distance (included)
    * @param ray A ray expressed in global coordinates
    * @param max_distance The max distance between the ray origin and the found intersection point
    * @return Hit struct containing all the data regarding the intersection
    */
    [[nodiscard]] Interaction intersectNoTransparentWithinDistance(const Ray & ray, const float max_distance) const {
        return traversal(ray, FIRST_NOT_TRANSPARENT_WITHIN_DISTANCE, max_distance);
    }

};

#endif //BVH_H
