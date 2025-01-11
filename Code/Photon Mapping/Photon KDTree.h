//
// Created by Guglielmo Mazzesi on 12/14/2024.
//

#ifndef PHOTON_KDTREE_H
#define PHOTON_KDTREE_H

// KD-Tree Node
struct KDTreeNode {
    Photon photon;
    KDTreeNode* left = nullptr;
    KDTreeNode* right = nullptr;

    // The axis on which the split occurs (0 for x, 1 for y, 2 for z)
    int axis = 0;

    KDTreeNode(const Photon& photon, const int axis) : photon(photon), axis(axis) {}
};

// Comparator for sorting photons based on a specific axis (x, y, or z)
inline bool comparePhotons (const Photon& a, const Photon& b, const int axis) {
    switch (axis) {
        // Compare by x-axis
        case 0:
            return a.position.x < b.position.x;
        // Compare by y-axis
        case 1:
            return a.position.y < b.position.y;
        // Compare by z-axis
        case 2:
            return a.position.z < b.position.z;
        default:
            return false;
    }
}

// Function to build the KD-Tree
inline KDTreeNode * buildKDTree(std::vector<Photon>& photons, const int depth = 0) {
    // Case in which there are no photons
    if (photons.empty())
        return nullptr;

    int axis = depth % 3;  // Cycle through the x, y, z axes
    // Sort photons by the current axis
    std::sort(photons.begin(), photons.end(), [axis](const Photon& a, const Photon& b) {
        return comparePhotons(a, b, axis);
    });

    int medianIndex = photons.size() / 2;  // Choose the median as the root
    Photon medianPhoton = photons[medianIndex];

    // Create the current node
    KDTreeNode* node = new KDTreeNode(medianPhoton, axis);

    // Recursively build left and right subtrees
    std::vector leftPhotons(photons.begin(), photons.begin() + medianIndex);
    std::vector rightPhotons(photons.begin() + medianIndex + 1, photons.end());

    node->left = buildKDTree(leftPhotons, depth + 1);
    node->right = buildKDTree(rightPhotons, depth + 1);

    return node;
}

// Struct for priority queue entries
struct Neighbor {
    Photon photon;
    float squaredDistance;

    bool operator<(const Neighbor& other) const {
        // Invert comparison for max-heap behavior
        return squaredDistance < other.squaredDistance;
    }
};

// Function to find the n nearest neighbors of a query point
inline void findNearestNeighbors(KDTreeNode* node, const glm::vec3& queryPoint,
                                 std::priority_queue<Neighbor>& maxHeap,
                                 int n, int depth = 0) {
    if (!node) return;

    // Get the current axis based on the depth
    int axis = depth % 3; // 0 = x, 1 = y, 2 = z

    // Compute the squared distance from the query point to the current photon's position
    float dist2 = glm::distance2(queryPoint, node->photon.position);

    // Add current photon to the max-heap if it fits in the top n neighbors
    if (maxHeap.size() < n) {
        maxHeap.push({ node->photon, dist2 });
    } else if (dist2 < maxHeap.top().squaredDistance) {
        // Replace the farthest neighbor with the current photon
        maxHeap.pop();
        maxHeap.push({ node->photon, dist2 });
    }

    // Determine which subtree to explore first
    KDTreeNode* firstSubtree = nullptr;
    KDTreeNode* secondSubtree = nullptr;

    if (queryPoint[axis] < node->photon.position[axis]) {
        firstSubtree = node->left;
        secondSubtree = node->right;
    } else {
        firstSubtree = node->right;
        secondSubtree = node->left;
    }

    // Recursively search the first subtree
    findNearestNeighbors(firstSubtree, queryPoint, maxHeap, n, depth + 1);

    // Check if we need to explore the other subtree
    float axisDist = queryPoint[axis] - node->photon.position[axis];
    if (maxHeap.size() < n || axisDist * axisDist < maxHeap.top().squaredDistance) {
        findNearestNeighbors(secondSubtree, queryPoint, maxHeap, n, depth + 1);
    }
}

// Function to get the nearest neighbors from the max-heap
inline std::vector<Photon> getNearestNeighbors(KDTreeNode* root, const glm::vec3& queryPoint, int n) {
    std::priority_queue<Neighbor> maxHeap;
    findNearestNeighbors(root, queryPoint, maxHeap, n);

    // Extract neighbors from the max-heap into a vector
    std::vector<Photon> nearestNeighbors;
    while (!maxHeap.empty()) {
        nearestNeighbors.push_back(maxHeap.top().photon);
        maxHeap.pop();
    }

    // Reverse the vector to get nearest-to-farthest order
    std::reverse(nearestNeighbors.begin(), nearestNeighbors.end());
    return nearestNeighbors;
}

#endif //PHOTON_KDTREE_H
