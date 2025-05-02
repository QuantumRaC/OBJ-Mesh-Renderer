#include "gridObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


gridObject::gridObject() { // Assign current value of nextId to id and increment it
    
    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);

    // Generate and bind VAO, VBO, and EBO
    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // TODO: P1aTask1 - Modify vertices and indices to draw grid and axis lines. Note: multiple vertices at the origin will be needed due to different color of axis lines.
    /*
     * Creates a grid from (-5,0,-5) to (5,0,5);
     * Modifies index buffer indices and uses GL_LINES to define line segments;
     * Updates draw() function
     */
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	//Gray grid lines - X and Y grids on the Y=0 plane
	for(int i=-5; i<=5; ++i){
		//X-axis lines in the Z direction
		vertices.insert(vertices.end(), {(float)i, 0.0f, -5.0f, 0.5f, 0.5f, 0.5f}); //Start point
		vertices.insert(vertices.end(), {(float)i, 0.0f, 5.0f, 0.5f, 0.5f, 0.5f}); //End point

		unsigned int start = (i+5)*2; //index offset
		indices.insert(indices.end(), {start, start+1});

		//Z-axis lines in the X direction
		vertices.insert(vertices.end(), {-5.0f, 0.0f, (float)i, 0.5f, 0.5f, 0.5f}); //Start point
		vertices.insert(vertices.end(), {5.0f, 0.0f, (float)i, 0.5f, 0.5f, 0.5f}); //End point

		start += 2*11; //Offset by previous grid lines
		indices.insert(indices.end(), {start, start+1});

	}
	//Add coordinate axes in their respective colors
	unsigned int base = vertices.size()/6; //get index offset after grid points

	//X-axis (Red)
	vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}); //Origin
	vertices.insert(vertices.end(),{5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}); //End of X+
	indices.insert(indices.end(), {base, base+1});
	base+=2;

	//Y-axis (Green)
	vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}); //Origin
	vertices.insert(vertices.end(),{0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f}); //End of Y+
	indices.insert(indices.end(), {base, base+1});
	base+=2;

	//Z-axis (Blue)
	vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}); //Origin
	vertices.insert(vertices.end(),{0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f}); //End of Z+
	indices.insert(indices.end(), {base, base+1});

//    // Tetrahedron vertices with positions and colors
//    std::vector<GLfloat> vertices = {
//        // Positions          // Colors
//         0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 0.0f,  // Top vertex
//        -1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f,  // Front-left
//         1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,  // Front-right
//         0.0f, -1.0f,  1.0f,   1.0f, 1.0f, 0.0f   // Back
//    };
//
//    // Tetrahedron indices
//    std::vector<GLuint> indices = {
//        0, 1,
//        0, 2,
//        0, 3,
//        1, 2,
//        2, 3,
//        1, 3
//    };
    
    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders(
			"../source/gridVertexShader.glsl",
			"../source/gridFragmentShader.glsl");
}

gridObject::~gridObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
}

void gridObject::draw(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    
    // TODO: P1aTask1 - Compute the MVP matrix and send to shader as a uniform. Refer to meshObject.cpp as reference.
    //computing the model-view-projection matrix:
	glm::mat4 mvp = projection * view * modelMatrix;
	GLint mvpLoc = glGetUniformLocation(shaderProgram, "MVP"); //Get the uniform location in the shader
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp)); //Send the mvp value to the shader
    // TODO: P1aTask1 - Bind the VAO and draw the grid using glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(VBO);
	glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0); //Draws the grid and axes using GL_LINES
	//unbind VAO after drawing
    glBindVertexArray(0);
}
