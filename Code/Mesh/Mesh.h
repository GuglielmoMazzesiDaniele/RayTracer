//
// Created by Guglielmo Mazzesi on 10/18/2024.
//

#ifndef MESH_H
#define MESH_H

using namespace std;

class Mesh {
    /**
    * Auxiliary function that reads and parse the OBJ file at given path
    * @param obj_path The path to the OBJ file
    */
    void readOBJFile(const char * obj_path) {
        // Opening the OBJ file
        ifstream file (obj_path);

        // Initializing the current object
        Object current_object = {};

        // Verifying that the file opened correctly
        if(file.is_open()) {
            // Reading the file up until EOF
            string line_type;

            // Initializing flag indicating weather or not am I reading vertices
            unsigned int objects_count = 0;

            // The current smoothing group (0 is disabled)
            bool current_smoothing = false;

            // The current material name
            string current_material_name;

            // Iterating the file up until EOF
            while(file >> line_type) {

                if(line_type == "o") {
                    // Reading the object name
                    string object_name;
                    file >> object_name;

                    // Verifying that this is the first object read
                    if(objects_count == 0) {
                        // Assigning the name
                        current_object.name = object_name;
                    }
                    else {
                        // Pushing the previous object
                        objects.emplace_back(current_object);

                        // Creating a new object
                        current_object = {};

                        // Assigning the name to the object
                        current_object.name = object_name;
                    }

                    // Increasing the objects counter
                    objects_count++;

                    continue;
                }

                // Reading a smoothing group line
                if(line_type == "s") {
                    // Initializing the container of the flag
                    string flag_value;

                    // Reading the flag
                    file >> flag_value;

                    // Case in which smoothing group is disabled
                    if(flag_value == "off" || flag_value == "0") {
                        current_smoothing = false;
                    }
                    // Case in which smoothing group is enabled or changed group
                    else {
                        current_smoothing = true;
                    }

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Reading a vertex line
                if(line_type == "v") {
                    // Initializing the variables for the vertices
                    float x_coordinate, y_coordinate, z_coordinate;

                    // Extracting the vertices
                    file >> x_coordinate >> y_coordinate >> z_coordinate;

                    // Inserting the newly read vertex into the data structure
                    this->vertices_coordinates.emplace_back(x_coordinate, y_coordinate, z_coordinate);

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Reading a vertex normal line
                if(line_type == "vn") {
                    // Initializing the variables for the normal
                    float x, y, z;

                    // Extracting the normal coordinates
                    file >> x >> y >> z;

                    // Creating the vector and normalizing it
                    glm::vec3 current_normal = glm::normalize(glm::vec3(x, y, z));

                    // Inserting the newly read normal into the data structure
                    this->vertices_normals.push_back(current_normal);

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Reading a UV coordinates line
                if(line_type == "vt") {
                    // Initializing the variables for the vertices
                    float u_coordinate, v_coordinate;

                    // Extracting the vertices
                    file >> u_coordinate >> v_coordinate ;

                    // Inserting the newly read vertex into the data structure
                    this->vertices_uv_coordinates.emplace_back(u_coordinate, v_coordinate);

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Reading a face line
                if(line_type == "f") {
                    // Reading the entire line and creating a string from it
                    string face_line;
                    getline(file, face_line);
                    istringstream face_line_stream(face_line);

                    // Initializing the entry container
                    string entry;

                    //Initializing the data of an entry
                    int vertex_index = 0;
                    int normal_index = 0;
                    int uv_coordinates_index = 0;

                    // Initializing the face struct
                    Face current_face;

                    // Reading the data from the line
                    for(unsigned int current_index = 0; face_line_stream >> entry; current_index++) {
                        // Converting the string into a stream
                        istringstream entry_stream(entry);

                        // Reading the vertex index
                        entry_stream >> vertex_index;

                        // Saving the vertex index in the face struct
                        current_face.vertices_indexes.push_back(vertex_index - 1);

                        // If I reached the end of the entry, then continue
                        // CASE V
                        if(entry_stream.peek() == EOF)
                            continue;

                        // If the next character is a slash, consume it
                        if(entry_stream.peek() == '/')
                            entry_stream.ignore(1);

                        // If the next character is another slash, consume it and read the normal index
                        // CASE V//VN
                        if(entry_stream.peek() == '/') {
                            entry_stream.ignore(1);
                            // Trying to read the normal index
                            entry_stream >> normal_index;

                            // Saving the normal index in the face struct
                            current_face.normals_indexes.push_back(normal_index - 1);

                            // Continue to the next line
                            continue;
                        }

                        // CASE V/VT
                        // Reading the UV coordinates index
                        entry_stream >> uv_coordinates_index;

                        // Saving the texture coordinates index in the face struct
                        current_face.uv_indexes.push_back(uv_coordinates_index - 1);

                        // CASE V/VT/VN
                        // If the next character is another slash, consume it and read the normal index
                        if(entry_stream.peek() == '/') {
                            entry_stream.ignore(1);
                            // Trying to read the normal index
                            entry_stream >> normal_index;

                            // Saving the normal index in the face struct
                            current_face.normals_indexes.push_back(normal_index - 1);
                        }
                    }

                    // Assigning the smoothing group
                    current_face.smoothing = current_smoothing;

                    // Assigning the material
                    current_face.material_name = current_material_name;

                    // Inserting the newly read face into the data structure
                    current_object.faces.emplace_back(current_face);

                    // Increasing the amount of primitives in the mesh
                    primitives_amount++;

                    continue;
                }

                // Reading a new material
                if(line_type == "usemtl") {
                    // Extracting the name of the material from file
                    file >> current_material_name;

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Default case: discarding the entire current line up until a new line
                file.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        else {
            // Printing an error in case the provided
            cerr << "Error while opening file " << obj_path << endl;
        }

        // Pushing the final object
        objects.emplace_back(current_object);

        elaborateFacesAndVertices();

        // Closing the file
        file.close();
    }

    /**
    * Auxiliary function that reads and parse the MTL file at given path
    * @param mtl_path The path to the OBJ file
    */
    void readMTLFile(const char * mtl_path) {
        // Opening the OBJ file
        ifstream file (mtl_path);

        // Initializing the current object
        Material buffer_material = {};

        // Initializing the actual current material
        Material current_material = {};

        // Initializing the name of the file
        string current_material_name;

        // Verifying that the file opened correctly
        if(file.is_open()) {
            // Reading the file up until EOF
            string line_type;

            // Initializing flag indicating weather or not am I reading vertices
            unsigned int materials_count = 0;

            // Iterating the file up until EOF
            while(file >> line_type) {

                // Case in which I have to create a new material
                if(line_type == "newmtl") {
                    // Verifying if it is the first
                    if(materials_count != 0)
                        materials.emplace(current_material_name, current_material);

                    // Reading the material name
                    file >> current_material_name;

                    // Increasing the amount of materials
                    materials_count++;

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the ambient coefficients
                if(line_type == "Ke") {
                    // TODO: Convert in light emission
                }

                // Case in which I read the ambient coefficients
                if(line_type == "Ka") {
                    // Initializing the values
                    float ambient_r, ambient_g, ambient_b;

                    // Reading the value
                    file >> ambient_r >> ambient_g >> ambient_b;

                    // Assigning the value to the material
                    buffer_material.ambient = glm::vec3(ambient_r, ambient_g, ambient_b);

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the diffuse coefficients
                if(line_type == "Kd") {
                    // Initializing the values
                    float diffuse_r, diffuse_g, diffuse_b;

                    // Reading the value
                    file >> diffuse_r >> diffuse_g >> diffuse_b;

                    // Assigning the value to the material
                    buffer_material.diffuse = glm::vec3(diffuse_r, diffuse_g, diffuse_b);

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the specular coefficients
                if(line_type == "Ks") {
                    // Initializing the values
                    float specular_r, specular_g, specular_b;

                    // Reading the value
                    file >> specular_r >> specular_g >> specular_b;

                    // Assigning the value to the material
                    buffer_material.specular = glm::vec3(specular_r, specular_g, specular_b);

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the material shininess (used in the Blinn-Phong)
                if(line_type == "Ns") {
                    // Initializing the float
                    float material_shininess;

                    // Reading the value
                    file >> material_shininess;

                    // Assigning the value to the material
                    buffer_material.shininess = material_shininess;

                    // Computing the roughness
                    float material_roughness = sqrt(2 / (material_shininess  + 2));

                    // Assigning the value to the material
                    buffer_material.roughness =  material_roughness;

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the material index of refraction
                if(line_type == "Ni") {
                    // Initializing the float
                    float material_index_of_refraction;

                    // Reading the value
                    file >> material_index_of_refraction;

                    // Assigning the value to the material
                    buffer_material.refraction_index = material_index_of_refraction;

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the material refractivity
                if(line_type == "d") {
                    // Initializing the float
                    float material_dissolve;

                    // Reading the value
                    file >> material_dissolve;

                    // Assigning the value to the material
                    buffer_material.refractivity = 1 - material_dissolve;

                    // Discarding the rest of the line
                    file.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Case in which I read the material illuminance model
                if(line_type == "illum") {
                    // Initializing the lighting model
                    int illumination_model;

                    // Extracting the model from file
                    file >> illumination_model;

                    // Applying the model
                    switch (illumination_model) {
                        case 0 : {
                            // Extracting the diffuse from the buffer
                            current_material.self_illuminance = buffer_material.diffuse;
                            break;
                        }
                        case 1 : {
                            // Extracting the diffuse from the buffer
                            current_material.ambient = buffer_material.ambient;
                            current_material.diffuse = buffer_material.diffuse;
                            break;
                        }
                        case 3 : {
                            // Extracting the diffuse from the buffer
                            current_material.ambient = buffer_material.ambient;
                            current_material.diffuse = buffer_material.diffuse;
                            current_material.specular = buffer_material.specular;

                            // Setting the material reflective
                            break;
                        }
                        case 6 : {
                            // Extracting the diffuse from the buffer
                            current_material.ambient = buffer_material.ambient;
                            current_material.diffuse = buffer_material.diffuse;
                            current_material.specular = buffer_material.specular;

                            // Setting the material refractivity
                            current_material.refractivity = buffer_material.refractivity;
                            current_material.refraction_index = buffer_material.refraction_index;
                            break;
                        }
                        default :
                        case 2 : {
                            // Extracting the diffuse from the buffer
                            current_material.ambient = buffer_material.ambient;
                            current_material.diffuse = buffer_material.diffuse;
                            current_material.specular = buffer_material.specular;
                            break;
                        }
                    }
                }

                // Default case: discarding the entire current line up until a new line
                file.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        else {
            // Printing an error in case the provided
            cerr << "Error while opening file " << mtl_path << endl;
        }

        // Pushing the final material
        this->materials.emplace(current_material_name, buffer_material);

        // Closing the file
        file.close();
    }

protected:
    /**
    * Auxiliary function that initializes the vertices struct contained within the faces' struct.
    * Also builds tangents and bitangets for both faces and vertices.
    */
    void elaborateFacesAndVertices() {
        // Sizing the vertices' adjacency list to the correct size
        adjacency_faces.resize(vertices_coordinates.size());

        // Initializing the vertices array contained within the faces struct
        for(auto & [faces, name] : objects) {
            for(auto & face : faces) {
                // Computing the edges of the face
                const glm::vec3 first_edge = vertices_coordinates[face.vertices_indexes[1]]
                                                - vertices_coordinates[face.vertices_indexes[0]];
                const glm::vec3 second_edge = vertices_coordinates[face.vertices_indexes[2]]
                                                - vertices_coordinates[face.vertices_indexes[0]];

                // Computing the face's normal
                face.normal = normalize(cross(first_edge, second_edge));

                // Computing the deltas in UV coordinates space
                const glm::vec2 first_delta = vertices_uv_coordinates[face.uv_indexes[1]]
                                                - vertices_uv_coordinates[face.uv_indexes[0]];
                const glm::vec2 second_delta = vertices_uv_coordinates[face.uv_indexes[2]]
                                                - vertices_uv_coordinates[face.uv_indexes[0]];

                // Computing the determinant of the UV matrix
                const float determinant_normalization = 1 / first_delta.x * second_delta.y - second_delta.y * first_delta.x;

                // Computing the tangent and bitangent
                face.tangent =  normalize(determinant_normalization * (second_delta.y * first_edge
                                                                        - first_delta.y * second_edge));

                face.bitangent = normalize(determinant_normalization * (-second_delta.x * first_edge
                                                                        + first_delta.x * second_edge));

                // Recomputing the bitangent while handling right/left hand coordinate system
                face.bitangent = cross(face.normal, face.tangent) *
                    dot(face.bitangent, cross(face.normal, face.tangent));

                // Initializing the vertices
                for(int i = 0; i < 3; i++) {
                    // Setting the pointer to the cartesian coordinates
                    face.vertices[i].coordinates = & this->vertices_coordinates[face.vertices_indexes[i]];
                    // If normals were given, set it
                    if(!face.normals_indexes.empty())
                        face.vertices[i].normal = this->vertices_normals[face.normals_indexes[i]];
                    // Otherwise add the face normal for future averaging
                    else
                        face.vertices[i].normal += face.normal;
                    // If UV coordinates were given, setting the pointer
                    if(!face.uv_indexes.empty())
                        face.vertices[i].uv_coordinates = & this->vertices_uv_coordinates[face.uv_indexes[i]];

                    // Adding the current face to the adjacency list of each vertex
                    adjacency_faces[face.vertices_indexes[i]].push_back(& face);

                    // Adding the tangents and bitangents to the vertex
                    face.vertices[i].tangent += face.tangent;
                    face.vertices[i].bitangent += face.bitangent;
                }
            }
        }

        // Finalizing the computation of tangent and bitangent for the vertices
        for(auto & [faces, name] : objects) {
            for(auto & face : faces) {
                for(auto & [coordinates, uv_coordinates, normal, tangent, bitangent]
                    : face.vertices) {
                    // Normalizing the normal, tangent and bitangent
                    normal = normalize(normal);
                    tangent = normalize(tangent);
                    bitangent = normalize(bitangent);

                    // Verifying the orthogonality of the tangent
                    tangent = tangent - dot(tangent, normal) * normal;

                    // Recomputing the bitangent, while handling right/left hand coordinates system
                    bitangent = cross(normal, tangent) * dot(bitangent, cross(normal, tangent));
                }
            }
        }
    }

public:
    struct Face {
        Vertex vertices [3]; ///< Vector of vertices

        vector<unsigned int> vertices_indexes; ///< Indexes of the vertices belonging to this face
        vector<unsigned int> normals_indexes; ///< Indexes of the normal belonging to each vertex
        vector<unsigned int> uv_indexes; ///< Indexes of the UV coordinates belonging to each vertex

        glm::vec3 normal; ///< Normal of the face
        glm::vec3 tangent; ///< Tangent of the face
        glm::vec3 bitangent; ///< Bitangent of the face

        string material_name; ///< String used to store the name of the material used by this face
        bool smoothing = false; ///< Smoothing group of this face
    };

    struct Object {
        vector<Face> faces; /// Vector of faces, populated from the OBJ file
        string name; ///< The name of the object
    };

    unsigned int primitives_amount = 0; ///< Counter of primitives within a mesh

    vector<Object> objects; ///< Vectors containing all the objects within this mesh

    map<string, Material> materials; ///< Map of materials

    vector<glm::vec3> vertices_coordinates; ///< Vector of vertices, populated from the OBJ file
    vector<glm::vec3> vertices_normals; ///< Vector of vertices normals, populated from the OBJ file
    vector<glm::vec2> vertices_uv_coordinates; ///< Vector of vertices UV coordinates, populated from the OBJ file
    vector<glm::vec3> vertices_tangents; ///< Vector of vertices tangents
    vector<glm::vec3> vertices_bitangents; ///< Vector of vertices bitangents

    vector<vector<Face *>> adjacency_faces; ///< Vector containing the adjacency list for each vertex

    const Texture * albedo = nullptr;
    const Texture * normal_map = nullptr;
    const Texture * AO_R_M = nullptr;
    const Texture * displacement = nullptr;

    string obj_path; ///< The mesh path in file system
    string mtl_path; ///< The mesh mtl path in the file system

    /**
    * Constructor used by children classes that procedurally generates the mesh geometry
    */
    // explicit Mesh();

    /**
    * Default constructor of a mesh
    * @param obj_path The path of the OBJ file that contains the model definition
    * @param mtl_path The path of the MTL file that contains the material used by this mesh
    * @param albedo Pointer to the albedo texture of the mesh
    * @param normal_map Pointer to the normal map of the mesh
    * @param AO_R_M Pointer to the ambient occlusion/roughness/metallic texture
    * @param displacement Pointer to the displacement texture of the mesh
    *
    */
    explicit Mesh (const char * obj_path = nullptr,
        const char * mtl_path = nullptr,
        const Texture * albedo = nullptr,
        const Texture * normal_map = nullptr,
        const Texture * AO_R_M = nullptr,
        const Texture * displacement = nullptr
        )
    : albedo(albedo),
    normal_map(normal_map),
    AO_R_M(AO_R_M),
    displacement(displacement),
    obj_path(obj_path? obj_path : ""),
    mtl_path(mtl_path ? mtl_path : ""){
        // Clock time at the beginning of OBJ parsing
        clock_t current_time = clock();

        if(PRINT_OBJ_PARSING_TIME)
            PrintStartingProcess("parsing of " + this->obj_path);

        // Reading the OBJ file at provided path
        if(!this->obj_path.empty())
            this->readOBJFile(obj_path);

        // Reading the mtl file, if a path was provided
        if(!this->mtl_path.empty())
            this->readOBJFile(mtl_path);

        if(PRINT_PRIMITIVES_AMOUNT)
            PrintGenericMessage("There are " + to_string(primitives_amount) + " primitives within " + this->obj_path);

        // Printing useful data regarding OBJ parsing time
        current_time = clock() - current_time;
        if(PRINT_OBJ_PARSING_TIME)
            PrintExecutionTime(current_time, "parse " + this->obj_path);
    }
};

#endif //MESH_H