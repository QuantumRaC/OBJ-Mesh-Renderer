#define TINYOBJLOADER_IMPLEMENTATION
#include "include/tiny_obj_loader.h"

#include "meshObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "loadTexture.hpp"

// Initialize static member
int meshObject::nextId = 1;
std::map<int, meshObject*> meshObject::meshObjectMap;

meshObject::meshObject() : id(nextId++) {
	// Add this object to the map
	meshObjectMap[id] = this;

	// Initialize the model matrix
	modelMatrix = glm::mat4(1.0f);

	// Generate and bind VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Load and compile shaders
	shaderProgram = LoadShaders(
			"../source/meshVertexShader.glsl",
			"../source/meshFragmentShader.glsl"
	);
	pickingShaderProgram = LoadShaders(
			"../source/pickingVertexShader.glsl",
			"../source/pickingFragmentShader.glsl"
	);

	std::cout << "Default meshObject created with ID: " << id << std::endl;
}

// TODO: P1aTask3 - Modify the constructor of Object to accept the path to an obj file
meshObject::meshObject(const std::string& filepath) : id(nextId++) { // Assign current value of nextId to id and increment it
    
    // Add this object to the map
    meshObjectMap[id] = this;

    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);

    // Generate and bind VAO, VBO, and EBO
    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // TODO: P1aTask3 - Read from obj file instead.
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	if (!loadOBJ(filepath.c_str(), vertices, uvs, normals, indices)) {
		std::cerr << "Failed to load model: " << filepath << std::endl;
		return;
	}

	// Check if normals exist in the file
	//if (normals.empty()) {
		std::cerr << "Warning: OBJ file missing normals! Generating for" << filepath.c_str() << std::endl;
		// Compute normals from face data
		normals.resize(vertices.size(), glm::vec3(0.0f));  // Initialize all to (0,0,0)

		for (size_t i = 0; i < indices.size(); i += 3) {
			int i0 = indices[i];
			int i1 = indices[i + 1];
			int i2 = indices[i + 2];

			glm::vec3 v0 = vertices[i0];
			glm::vec3 v1 = vertices[i1];
			glm::vec3 v2 = vertices[i2];

			//face normal
			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;
			glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

			//assign the normal to the three vertices
			normals[i0] += normal;
			normals[i1] += normal;
			normals[i2] += normal;
		}

		// Normalize all vertex normals
		for (size_t i = 0; i < normals.size(); i++) {
			normals[i] = glm::normalize(normals[i]);
		}
	//}

	//TODO: P1bTask5 - Create normal buffer.
	// Create normal buffer
	std::vector<GLfloat> vertexData;
	for (size_t i = 0; i < vertices.size(); i++) {
		vertexData.push_back(vertices[i].x);
		vertexData.push_back(vertices[i].y);
		vertexData.push_back(vertices[i].z);

		if (i < uvs.size()) {
			vertexData.push_back(uvs[i].x);
			vertexData.push_back(uvs[i].y);
		} else {
			vertexData.push_back(0.0f);
			vertexData.push_back(0.0f);
		}

		if (i < normals.size()) {
			vertexData.push_back(normals[i].x);
			vertexData.push_back(normals[i].y);
			vertexData.push_back(normals[i].z);
		} else {
			vertexData.push_back(0.0f);
			vertexData.push_back(1.0f); //Default normal pointing up
			vertexData.push_back(0.0f);
		}
	}

	// create UV buffer
	for (size_t i = 0; i < vertices.size(); ++i) {
		vertexUVBuffer.push_back(vertices[i].x);
		vertexUVBuffer.push_back(vertices[i].y);
		vertexUVBuffer.push_back(vertices[i].z);
		vertexUVBuffer.push_back(uvs[i].x);
		vertexUVBuffer.push_back(uvs[i].y);
	}
	faceTexture = loadTexture("../models/facefront.jpg");

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);  // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));  // UV
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));  // Normals
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);

	numIndices = (GLsizei)indices.size();

    // Load and compile shaders
    shaderProgram = LoadShaders(
			"../source/meshVertexShader.glsl",
			"../source/meshFragmentShader.glsl");
    pickingShaderProgram = LoadShaders(
			"../source/pickingVertexShader.glsl",
			"../source/pickingFragmentShader.glsl");
}

meshObject::meshObject(int objectId){
	id = objectId;
	meshObjectMap[id] = this;
}

meshObject::~meshObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    // Remove this object from the map
    meshObjectMap.erase(id);
}

// Function to load an .obj file into OpenGL compatible arrays
bool meshObject::loadOBJ(const char* path,
						 std::vector<glm::vec3>& out_vertices,
						 std::vector<glm::vec2>& out_uvs,
						 std::vector<glm::vec3>& out_normals,
						 std::vector<unsigned int>& out_indices) {
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	tinyobj::ObjReader reader;
	if (!reader.ParseFromFile(path, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjLoader Error: " << reader.Error() << std::endl;
		}
		return false;
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjLoader Warning: " << reader.Warning() << std::endl;
	}

	const tinyobj::attrib_t& attrib = reader.GetAttrib();
	const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();

	std::vector<unsigned int> temp_indices;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			// Extract vertex positions
			glm::vec3 vertex = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
			};
			out_vertices.push_back(vertex);

			// Extract texture coordinates if available, and flip vertically
			if (!attrib.texcoords.empty() && index.texcoord_index >= 0) {
				glm::vec2 uv = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1] // Flip Y-axis
				};
				out_uvs.push_back(uv);
			} else {
				out_uvs.emplace_back(0.0f, 0.0f);
			}

			// Extract normals if available
			if (!attrib.normals.empty() && index.normal_index >= 0) {
				glm::vec3 normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
				};
				out_normals.push_back(normal);
			} else {
				out_normals.emplace_back(0.0f, 0.0f, 0.0f);
			}

			temp_indices.push_back(out_vertices.size() - 1);
		}
	}

	out_indices = temp_indices;
	modelMatrix = glm::scale(modelMatrix, glm::vec3(25.0f)); // Scales model manually
	std::cout << "Successfully loaded OBJ file: " << path << std::endl;
	this->vertices = out_vertices;
	this->texcoords = out_uvs;
	this->normals = out_normals;
	this->indices = out_indices;


	return true;
}



//TODO: P1bTask5 - Modify to accept lighiting info as arguement.
void meshObject::draw(const glm::mat4& view,
					  const glm::mat4& projection,
					  const glm::vec3& lightPos1,
					  const glm::vec3& lightPos2,
					  const glm::vec3& viewPos,
					  const glm::vec3& specColor1,
					  const glm::vec3& diffColor1,
					  const glm::vec3& ambientColor1,
					  const glm::vec3& specColor2,
					  const glm::vec3& diffColor2,
					  const glm::vec3& ambientColor2,
					  const glm::vec3& objectColor,
					  int currSelected,
					  bool useSmoothMesh) {
	glUseProgram(shaderProgram);

	// Compute MVP matrix
	glm::mat4 MVP = projection * view * modelMatrix;
	GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

	// Send model matrix separately for lighting calculations
	GLuint modelID = glGetUniformLocation(shaderProgram, "modelMatrix");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Send normal matrix (transpose inverse model matrix for correct normal transformations)
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
	GLuint normalMatrixID = glGetUniformLocation(shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// lighting information
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos1"), 1, glm::value_ptr(lightPos1));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos2"), 1, glm::value_ptr(lightPos2));
	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));

	glUniform3fv(glGetUniformLocation(shaderProgram, "specColor1"), 1, glm::value_ptr(specColor1));
	glUniform3fv(glGetUniformLocation(shaderProgram, "diffColor1"), 1, glm::value_ptr(diffColor1));
	glUniform3fv(glGetUniformLocation(shaderProgram, "specColor2"), 1, glm::value_ptr(specColor2));
	glUniform3fv(glGetUniformLocation(shaderProgram, "diffColor2"), 1, glm::value_ptr(diffColor2));

	glUniform3fv(glGetUniformLocation(shaderProgram, "ambientColor1"), 1, glm::value_ptr(ambientColor1));
	glUniform3fv(glGetUniformLocation(shaderProgram, "ambientColor2"), 1, glm::value_ptr(ambientColor2));

	int chosenPart = (this->id == currSelected) ? 1 : 0;
	GLuint isSelectedID = glGetUniformLocation(shaderProgram, "chosenPart");
	glUniform1i(isSelectedID, chosenPart);


//	// Determine object color based on selection
//	glm::vec3 finalColor = (this->id == currSelected) ? glm::vec3(1.0f, 1.0f, 0.0f) : objectColor;
	glm::vec3 finalColor = objectColor;
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(finalColor));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, faceTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

	// Bonus 1 - smooth mesh
	if (useSmoothMesh) {
		glBindVertexArray(smoothVAO);
		glDrawElements(GL_TRIANGLES, smoothVertexCount, GL_UNSIGNED_INT, 0);
	} else {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}



void meshObject::draw(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    
    // Compute the MVP matrix
    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    //TODO: P1bTask5 - Send lighting info to shader using uniform. May also need to send the model matrix seperatily as a uniform.

    // Draw the object
    glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void meshObject::translate(const glm::vec3& translation) {
    // Apply translation to the model matrix
    modelMatrix = glm::translate(modelMatrix, translation);
	//debugging:
	//std::cout << "Object translated by: " << translation.x << ", " << translation.y << ", " << translation.z << std::endl;
}

void meshObject::rotate(float angle, const glm::vec3& axis) {
    // Apply rotation to the model matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
}

void meshObject::drawPicking(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(pickingShaderProgram); // Use the picking shader

    //TODO: P1bBonus2 - Send the value of the id variable to the shader as a uniform. Use glUniform1f

    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(pickingShaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

	//convert "id", the integer mesh ID, into an RGB color
	int r = (id & 0x000000FF) >>  0;
	int g = (id & 0x0000FF00) >>  8;
	int b = (id & 0x00FF0000) >> 16;
	glUniform4f(glGetUniformLocation(pickingShaderProgram, "PickingColor"),
				r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program

	//recursively render children
	for(meshObject* child : children){
		child->drawPicking(view, projection);
	}
}

meshObject* meshObject::getMeshObjectById(int id) {
    // Look up the object by ID in the map
    auto it = meshObjectMap.find(id);
    if (it != meshObjectMap.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if ID not found
}

// Bonus 1 - smoothing out model mesh
void meshObject::generateSmoothPNMesh() {
	std::vector<glm::vec3> new_vertices;
	std::vector<glm::vec2> new_texcoords;
	std::vector<glm::vec3> new_normals;
	std::vector<unsigned int> new_indices;

	const int tessellationLevel = 4;

	for (size_t i = 0; i < indices.size(); i += 3) {
		glm::vec3 v0 = vertices[indices[i]];
		glm::vec3 v1 = vertices[indices[i + 1]];
		glm::vec3 v2 = vertices[indices[i + 2]];

		glm::vec3 n0 = normals[indices[i]];
		glm::vec3 n1 = normals[indices[i + 1]];
		glm::vec3 n2 = normals[indices[i + 2]];

		glm::vec2 uv0 = texcoords[indices[i]];
		glm::vec2 uv1 = texcoords[indices[i + 1]];
		glm::vec2 uv2 = texcoords[indices[i + 2]];

		for (int iu = 0; iu <= tessellationLevel; ++iu) {
			for (int iv = 0; iv <= tessellationLevel - iu; ++iv) {
				float u = (float)iu / tessellationLevel;
				float v = (float)iv / tessellationLevel;
				float w = 1.0f - u - v;

				glm::vec3 interpolatedVertex = w * v0 + u * v1 + v * v2;
				glm::vec3 interpolatedNormal = glm::normalize(w * n0 + u * n1 + v * n2);
				glm::vec2 interpolatedUV = w * uv0 + u * uv1 + v * uv2;

				new_vertices.push_back(interpolatedVertex);
				new_normals.push_back(interpolatedNormal);
				new_texcoords.push_back(interpolatedUV);
			}
		}

		int base = (int)new_vertices.size() - ((tessellationLevel + 1) * (tessellationLevel + 2)) / 2;
		for (int iu = 0; iu < tessellationLevel; ++iu) {
			for (int iv = 0; iv < tessellationLevel - iu; ++iv) {
				int id0 = base + (iu * (tessellationLevel + 1) - (iu * (iu - 1)) / 2) + iv;
				int id1 = id0 + 1;
				int id2 = id0 + (tessellationLevel - iu + 1);

				new_indices.push_back(id0);
				new_indices.push_back(id2);
				new_indices.push_back(id1); // reversed id1 and id2


				if (iv < tessellationLevel - iu - 1) {
					int id3 = id2 + 1;
					new_indices.push_back(id1);
					new_indices.push_back(id2);
					new_indices.push_back(id3); //Reversed id2 and id3

				}
			}
		}
	}

	// Upload new smooth VAO/VBO
	smoothVertexCount = (int)new_indices.size();

	glGenVertexArrays(1, &smoothVAO);
	glBindVertexArray(smoothVAO);

	glGenBuffers(1, &smoothVBO);
	glBindBuffer(GL_ARRAY_BUFFER, smoothVBO);
	glBufferData(GL_ARRAY_BUFFER, new_vertices.size() * sizeof(glm::vec3), new_vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &smoothUV);
	glBindBuffer(GL_ARRAY_BUFFER, smoothUV);
	glBufferData(GL_ARRAY_BUFFER, new_texcoords.size() * sizeof(glm::vec2), new_texcoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &smoothNormal);
	glBindBuffer(GL_ARRAY_BUFFER, smoothNormal);
	glBufferData(GL_ARRAY_BUFFER, new_normals.size() * sizeof(glm::vec3), new_normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &smoothEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smoothEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, new_indices.size() * sizeof(unsigned int), new_indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}



void meshObject::reuploadMesh() {
	glBindVertexArray(VAO);

	std::vector<float> vertexData;
	for (size_t i = 0; i < vertices.size(); i++) {
		vertexData.push_back(vertices[i].x);
		vertexData.push_back(vertices[i].y);
		vertexData.push_back(vertices[i].z);

		if (i < texcoords.size()) {
			vertexData.push_back(texcoords[i].x);
			vertexData.push_back(texcoords[i].y);
		} else {
			vertexData.push_back(0.0f);
			vertexData.push_back(0.0f);
		}

		if (i < normals.size()) {
			vertexData.push_back(normals[i].x);
			vertexData.push_back(normals[i].y);
			vertexData.push_back(normals[i].z);
		} else {
			vertexData.push_back(0.0f);
			vertexData.push_back(1.0f);
			vertexData.push_back(0.0f);
		}
	}

	// reupload packed data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}



