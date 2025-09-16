//
// Created by Guglielmo Mazzesi on 12/20/2024.
//

#ifndef DEFAULT_SCENE_H
#define DEFAULT_SCENE_H

// MATERIALS
inline Material armor_material;

// TRANSFORMS

// SPHERES
inline glm::mat4 small_sphere_transform;
inline glm::mat4 normal_sphere_transform;
inline glm::mat4 big_sphere_transform;
inline glm::mat4 mesh_sphere_transform;

// PLANES
inline glm::mat4 right_plane_transform;
inline glm::mat4 left_plane_transform;
inline glm::mat4 bottom_plane_transform;
inline glm::mat4 top_plane_transform;
inline glm::mat4 front_plane_transform;
inline glm::mat4 back_plane_transform;

// MESHES
inline glm::mat4 polygon_transform;
inline glm::mat4 bunny_transform;
inline glm::mat4 armadillo_transform;
inline glm::mat4 lucy_transform;
inline glm::mat4 mesh_transform;
inline glm::mat4 mathilda_transform;
inline glm::mat4 armor_transform;

// POINT LIGHTS
inline glm::mat4 first_point_light_transform;
inline glm::mat4 second_point_light_transform;
inline glm::mat4 third_point_light_transform;

// AREA LIGHTS
inline glm::mat4 first_area_light_transform;
inline glm::mat4 second_area_light_transform;
inline glm::mat4 third_area_light_transform;
inline glm::mat4 fourth_area_light_transform;

/**
* Function that initialized the materials used in the default scene
* @param frame_number The current frame number
*/
inline void defineDefaultSceneMaterials(int frame_number) {
	// Armor material
	armor_material.specular = glm::vec3(0.85f);
	armor_material.roughness = 0.1f;
}

/**
* Function that initialized the transforms used in the default scene
* @param frame_number The current frame number
*/
inline void defineDefaultSceneTransform(int frame_number) {
    // Auxiliary variables initialization
    glm::mat4 T;
    glm::mat4 R;
    glm::mat4 S;

    // First sphere transform
    T = glm::translate(glm::mat4(1), glm::vec3(-1, -2.5, 6));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(0.5));
    small_sphere_transform = T * R * S;

    // Second sphere transform
    T = glm::translate(glm::mat4(1), glm::vec3(1, -2, 8));
    R = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    normal_sphere_transform = T * R * S;

    // Third sphere transform
    T = glm::translate(glm::mat4(1), glm::vec3(-2.5, -1, 8.5));
    R = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(2));
    big_sphere_transform = T * R * S;

	// Mesh sphere transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 5.5));
	R = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(-1, 0, 0));
	R = glm::rotate(R, glm::radians(45.f), glm::vec3(0, -1, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(2));
	mesh_sphere_transform = T * R * S;

    // Right plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(15, 0, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    right_plane_transform = T * R * S;

    // Left plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(-15, 0, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
	left_plane_transform = T * R * S;

    // Bottom plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    bottom_plane_transform = T * R * S;

    // Top plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, 27, 0));
    R = glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    top_plane_transform = T * R * S;

    // Front plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, 0, 30));
    R = glm::rotate(glm::mat4(1), glm::radians(270.f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    front_plane_transform = T * R * S;

    // Back plane transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, 0, -30));
    R = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    back_plane_transform = T * R * S;

    // Polygon transform
    T = glm::translate(glm::mat4(1), glm::vec3(0, 0, 4));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
    S = glm::scale(glm::mat4(1), glm::vec3(2));
    polygon_transform = T * R * S;

	// Bunny transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 8));
	R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(1));
	bunny_transform = T * R * S;

	// Armadillo transform
	T = glm::translate(glm::mat4(1), glm::vec3(-4, -3, 10));
	R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 1, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(1));
	armadillo_transform = T * R * S;

    // Lucy transform
    T = glm::translate(glm::mat4(1), glm::vec3(4, -3, 10));
    R = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0, 0, 1));
    S = glm::scale(glm::mat4(1), glm::vec3(1));
    lucy_transform = T * R * S;

 	// Bust transform
 	T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 7.5));
 	R = glm::rotate(glm::mat4(1), glm::radians(-160.0f), glm::vec3(0, 1, 0));
 	S = glm::scale(glm::mat4(1), glm::vec3(3));
 	mesh_transform = T * R * S;

	// Matilda transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 7.5));
	R = glm::rotate(glm::mat4(1), glm::radians(-160.0f), glm::vec3(0, 1, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(1));
	mathilda_transform = T * R * S;

	// Armor transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, -3, 1));
	R = glm::rotate(glm::mat4(1), glm::radians(-160.0f), glm::vec3(0, 1, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(2.5));
	armor_transform = T * R * S;

	// First point light transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, 26, 5));
	R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
	S = glm::scale(glm::mat4(1), glm::vec3(1));
	first_point_light_transform = T * R * S;

	// Second point light transform
	T = glm::translate(glm::mat4(1), glm::vec3(3, 4, 12));
	R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
	S = glm::scale(glm::mat4(1), glm::vec3(1));
	second_point_light_transform = T * R * S;

	// Third point light transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, 5, 1));
	R = glm::rotate(glm::mat4(1), glm::radians(0.f), glm::vec3(0, 0, 1));
	S = glm::scale(glm::mat4(1), glm::vec3(1));
	third_point_light_transform = T * R * S;

	// First area light transform
	T = glm::translate(glm::mat4(1), glm::vec3(7, 8, 7));
	R = glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(1, 0, 0));
	R = glm::rotate(R, glm::radians(35.0f), glm::normalize(glm::vec3(-0.5, 0, 1)));
	S = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	first_area_light_transform = T * R * S;

	// Second area light transform
	T = glm::translate(glm::mat4(1), glm::vec3(-6, 8, 9));
	R = glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(1, 0, 0));
	R = glm::rotate(R, glm::radians(35.0f), glm::normalize(glm::vec3(0.5, 0, -1)));
	S = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	second_area_light_transform = T * R * S;

	// Third area light transform
	T = glm::translate(glm::mat4(1), glm::vec3(-2, 8, 2));
	R = glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(1, 0, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	third_area_light_transform = T * R * S;

	// Third area light transform
	T = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
	R = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0));
	S = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	fourth_area_light_transform = T * R * S;
}


/**
 * Function that contains the initialization of the objects in the scene
 * @param frame_number The current frame number
 */
inline void defineDefaultScene (const int frame_number) {
	// Defining materials
	defineDefaultSceneMaterials(frame_number);

	// Defining transforms
	defineDefaultSceneTransform(frame_number);

    // POINT LIGHTS
    // lights.push_back(new PointLight(first_point_light_transform, glm::vec3(150.0f), glm::vec3(1)));
    // lights.push_back(new PointLight(second_point_light_transform, glm::vec3(80.0f), glm::vec3(1)));
    // lights.push_back(new PointLight(third_point_light_transform, glm::vec3(80.0f), glm::vec3(1)));

    // AREA LIGHTS
    AreaLight(first_area_light_transform, glm::vec3(40, 60, 40), glm::vec3(0.66, 1, 0.66), 1.5, 45, true);
    AreaLight(second_area_light_transform, glm::vec3(60, 40, 40), glm::vec3(1, 0.66, 0.66), 1.5, 45, true);
    AreaLight(third_area_light_transform, glm::vec3(40, 40, 60), glm::vec3(0.66, 0.66, 1), 1.5, 45, true);
    // AreaLight(fourth_area_light_transform, glm::vec3(50), glm::vec3(1), 1, 60, false);

    // SPHERES
    primitives.push_back(new Sphere(small_sphere_transform, & green_material));
    primitives.push_back(new Sphere(normal_sphere_transform, & smooth_reflective_material));
    primitives.push_back(new Sphere(big_sphere_transform, & glass_material));
    // 	new Texture("./Textures/Sphere/Fabric/Albedo.jpg"),
    // 	new Texture("./Textures/Sphere/Fabric/Normal.jpg"),
    // 	new Texture("./Textures/Sphere/Fabric/AO-R-M.jpg")));

	// MESH SPHERES
	// buildMaterialMesh(new SphereMesh(1, 3, false, 0.25,
	// 	new Texture("./Textures/Sphere/Bark/Albedo.jpg"),
	// 	new Texture("./Textures/Sphere/Bark/Normal.jpg"),
	// 	new Texture("./Textures/Sphere/Bark/AO-R-M.jpg"),
	// 	new Texture("./Textures/Sphere/Bark/Displacement.jpg")),
	// 	mesh_sphere_transform, grey_material);

    // PLANES
    planes.push_back(new Plane(bottom_plane_transform, & grey_material));
    // planes.push_back(new Plane(top_plane_transform, grey_material));
    // planes.push_back(new Plane(left_plane_transform, grey_material));
    // planes.push_back(new Plane(right_plane_transform, grey_material));
    // planes.push_back(new Plane (front_plane_transform, grey_material));

    // MESHES
	// buildMaterialMesh(new Mesh("./Meshes/Course Mesh/Armadillo.obj"), armadillo_transform, grey_material);
	// buildMaterialMesh(new Mesh("./Meshes/Course Mesh/Bunny.obj"), bunny_transform, grey_material);
	// buildMaterialMesh(new Mesh("./Meshes/Course Mesh/Lucy.obj"), lucy_transform, grey_material);
    // buildMaterialMesh( new Mesh("./Meshes/OBJ/Polygon.obj"), polygon_transform, grey_material);
    // buildMaterialMesh( new Mesh("./Meshes/Mathilda/Mathilda.obj"), mathilda_transform, grey_material);
	// buildMaterialMesh(new Mesh("./Meshes/Armor/Armor.obj",
	// 	"./Meshes/Armor/Albedo.png"),
	// 	armor_transform, armor_material);
}

#endif //DEFAULT_SCENE_H
