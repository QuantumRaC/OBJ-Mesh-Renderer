#ifndef meshObject_hpp
#define meshObject_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>
#include <map>
#include <vector>
#include <string>


class meshObject {
public:
	meshObject();

	meshObject(const std::string &filepath);

	meshObject(int objectId);

	~meshObject();

    void draw(const glm::mat4& view, const glm::mat4& projection);
    void drawPicking(const glm::mat4& view, const glm::mat4& projection);
    void translate(const glm::vec3& translation); // Translate the object
    void rotate(float angle, const glm::vec3& axis); // Rotate the object

    int getId() const { return id; } // Getter for the ID
    
    static meshObject* getMeshObjectById(int id); // Retrieve object by ID

	glm::mat4 modelMatrix;
	std::vector<float> vertexUVBuffer;
	int id;
	GLuint faceTexture;

	// Bonus 1 - smoothing out the model
	void generateSmoothPNMesh();
	void reuploadMesh();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	GLuint smoothVAO = 0;
	GLuint smoothVBO = 0;
	GLuint smoothUV = 0;
	GLuint smoothNormal = 0;
	GLuint smoothEBO = 0;
	int smoothVertexCount = 0;


	// TODO: P1bTask4 - Create a list of children.
	std::vector<meshObject*> children;

	void draw(const glm::mat4& view,
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
			  bool useSmoothMesh);

private:
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    GLuint pickingShaderProgram;

	static int nextId; // Static counter for unique IDs
	// ID for this specific object
    
    GLsizei numIndices;
	bool loadOBJ(const char* path,
				 std::vector<glm::vec3>& out_vertices,
				 std::vector<glm::vec2>& out_uvs,
				 std::vector<glm::vec3>& out_normals,
				 std::vector<unsigned int>& out_indices);
	static std::map<int, meshObject*> meshObjectMap; // Static map of ID to Object
};

#endif
