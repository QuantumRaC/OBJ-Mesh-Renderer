// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "meshObject.hpp"
#include "gridObject.hpp"


// Function prototypes
int initWindow(void);
static void mouseCallback(GLFWwindow*, int, int, int);
int getPickedId();

float baseRotation = 0.0f;
float arm1Rotation = 0.0f;
float arm2Rotation = 0.0f;
glm::vec3 baseTranslation(0.0f, 0.0f, 0.0f);

const GLuint windowWidth = 1024, windowHeight = 768;
GLFWwindow* window;

// The object that is currently selected for P1bTask4
int currSelected = 0;

bool showWireframe = false;
bool keyFWasPressed = false;
bool keyFIsPressed = false;

bool useSmoothMesh = false; // Bonus 1
bool keyPWasPressed = false;
bool keyPIsPressed = false;

int main() {
    // ATTN: REFER TO https://learnopengl.com/Getting-started/Creating-a-window
    // AND https://learnopengl.com/Getting-started/Hello-Window to familiarize yourself with the initialization of a window in OpenGL
    
    if (initWindow() != 0) return -1;

    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for Project 2, use an ortho camera :
    // gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

    
    //TODO: P1aTask3 - Create each robot arm piece by creating instances class meshObject.
    gridObject grid;
    //meshObject obj("../models/head.obj");
	meshObject obj("../models/texturedhead.obj");
	//GLuint faceTexture = loadTexture("models/facefront.jpg");


    // TODO: P1aTask3 - Translate each robot arm piece to its approapriate location
    //obj.translate(glm::vec3(0,0,3));
//    base.translate(glm::vec3(0,0,0));
	//arm1.translate(glm::vec3(0, -30.0f, 0));
//	joint.translate(glm::vec3(0,-1.0f,0));
//	arm2.translate(glm::vec3(0,-1.0f,0));

    //TODO: P1aTask2 - Create variables to keep track of camera angles.
	float cameraTheta = glm::radians(45.0f); //horizontal rotation angle around Y-axis
	float cameraPhi = glm::radians(45.0f); //vertical rotation angle around X-axis
	float cameraRadius = 20.0f; //Fixed distance from origin

    //TODO: P1bTask5 - Create variables to store lighting info.
	// Lighting variables
	//Light 1:
	glm::vec3 lightPos1(20.0f, 2.0f, 10.0f);  // Light 1 (20, 2, 10)
	glm::vec3 specColor1(0.2f, 0.2f, 0.2f);   // spec 1 (dim white)
	glm::vec3 diffColor1(0.5f, 0.5f, 0.5f); //diffuse 1 (brighter white)
	//Light 2:
	glm::vec3 lightPos2(-20.0f, 20.0f, -10.0f);  // Light 2 (-20, 20, 0)
	glm::vec3 specColor2(1.0f, 1.0f, 1.0f);   // spec 2 (white)
	glm::vec3 diffColor2(0.6f, 0.6f, 0.6f); //diffuse 2 (dim white)

	glm::vec3 ambientColor1(0.1f, 0.1f, 0.1f); // very dim white ambient
	glm::vec3 ambientColor2(0.1f, 0.1f, 0.1f);


	double lastTime = glfwGetTime();
    int nbFrames = 0;
    do {
        
        // Timing
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        //TODO: P1aTask2 - set currSelected to 0 when key c is pressed.
		float rotationSpeed = 0.06f;
		float deltaTime = (currentTime-lastTime);
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {//TODO: P1aTask2 - adjust the camera rotation.// Note: to make adjustments independent of frame rate, use time since last frame to make adjustment.
			//Set currSelected to 0 when key c pressed
			// currSelected = 0;
		}
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            //TODO: P1aTask2 - adjust the camera rotation.
            // Note: to make adjustments independent of frame rate, use time since last frame to make adjustment.
//        	cameraTheta -= rotationSpeed * deltaTime; //Rotate left (negative Y-axis)
			cameraTheta -= rotationSpeed;
			//std::cout<<"LEFT key pressed"<<std::endl;
		}
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            //TODO: P1aTask2 - adjust the camera rotation.
			//cameraTheta += rotationSpeed * deltaTime; //Rotate right (positive Y-axis)
			cameraTheta += rotationSpeed;
			//std::cout<<"RIGHT key pressed"<<std::endl;
        }
        
        //TODO: P1aTask2 - Add the cases for movment along the other axis.
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			//TODO: P1aTask2 - adjust the camera rotation.
			if(cameraPhi<glm::radians(89.0f)){
				cameraPhi += rotationSpeed;
			} //To prevent flipping from the top
			//cameraPhi += rotationSpeed * deltaTime;
			//std::cout<<"UP key pressed"<<std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			//TODO: P1aTask2 - adjust the camera rotation.
			if(cameraPhi>glm::radians(-89.0f)){
				cameraPhi -= rotationSpeed;
			} //To prevent flipping from the top
//				cameraPhi -= rotationSpeed * deltaTime;
			//std::cout<<"DOWN key pressed"<<std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			cameraTheta = glm::radians(45.0f);
			cameraPhi = glm::radians(45.0f);
			useSmoothMesh = false;
		}

		keyFIsPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
		if (keyFIsPressed && !keyFWasPressed) {
			showWireframe = !showWireframe;
		}
		keyFWasPressed = keyFIsPressed;


		//TODO: P1aTask2 - Create the view matrix based on camera angles.
        glm::vec3 cameraPos = glm::vec3(
				cameraRadius * cos(cameraPhi) * sin(cameraTheta), //X pos
				cameraRadius * sin(cameraPhi), //Y pos
				cameraRadius * cos(cameraPhi) * cos(cameraTheta) //Z pos
				);

		glm::mat4 viewMatrix = glm::lookAt(
            cameraPos,
            glm::vec3(0.0f),  // Look at the origin
            glm::vec3(0, 1, 0)  // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
        );


//        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            //TODO: P1bBonus2 draw all robot arm pieces using drawPicking function
////			base.drawPicking(viewMatrix, projectionMatrix);
//            int pickedId = getPickedId();
//		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // DRAWING the SCENE
		grid.draw(viewMatrix, projectionMatrix);

        //TODO: P1bTask5 - Pass the lighting info to the draw function.
		glm::vec3 viewPos = cameraPos;
		glm::vec3 defaultColor(0.0f, 0.0f, 0.0f);  // Default black color
		obj.translate(glm::vec3(0.0f, 0.0f, 0.0f));

		keyPIsPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
		if (keyPIsPressed && !keyPWasPressed) {
			useSmoothMesh = !useSmoothMesh;
		}
		keyPWasPressed = keyPIsPressed;
		if (useSmoothMesh && obj.smoothVAO == 0) {
			obj.generateSmoothPNMesh();
		}

		if (showWireframe){
			obj.draw(viewMatrix,
					 projectionMatrix,
					 lightPos1,
					 lightPos2,
					 viewPos,
					 defaultColor,
					 defaultColor,
					 defaultColor,
					 defaultColor,
					 defaultColor,
					 defaultColor,
					 defaultColor,
					 currSelected,
					 useSmoothMesh);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
//			obj.draw(viewMatrix,
//					 projectionMatrix,
//					 lightPos1,
//					 lightPos2,
//					 viewPos,
//					 defaultColor,
//					 defaultColor,
//					 defaultColor,
//					 defaultColor,
//					 defaultColor,
//					 defaultColor,
//					 defaultColor,
//					 currSelected,
//					 useSmoothMesh);
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal filled mode
		}
		else{
			obj.draw(viewMatrix,
					 projectionMatrix,
					 lightPos1,
					 lightPos2,
					 viewPos,
					 specColor1,
					 diffColor1,
					 ambientColor1,
					 specColor2,
					 diffColor2,
					 ambientColor2,
					 defaultColor,
					 currSelected,
					 useSmoothMesh);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal filled mode
		}
		// obj.draw(viewMatrix,projectionMatrix);
		glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}

// Initialize GLFW and create a window
int initWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // FOR MAC

    window = glfwCreateWindow(windowWidth, windowHeight, "Yao, Cynthia(81111656)", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Set up inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    glfwSetMouseButtonCallback(window, mouseCallback);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.7f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    return 0;
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left mouse button pressed" << std::endl;
    }
}

int getPickedId(){
    glFlush();
    // --- Wait until all the pending drawing commands are really done.
    // Ultra-mega-over slow !
    // There are usually a long time between glDrawElements() and
    // all the fragments completely rasterized.
    glFinish();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    unsigned char data[4];

    //TODO: P1bBonus2 - Use glfwGetCursorPos to get the x and y value of the cursor.
    double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
    //TODO: P1bBonus2 - Use glfwGetFramebufferSize and glfwGetWindowSize to get the frame buffer size and window size. On high resolution displays, these sizes might be different.
    int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	int x= static_cast<int>(xpos);
	int y = static_cast<int>(height-ypos); //flip y axis
    
    //TODO: P1bBonus2 - Use glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data) to read the pixel data.
    // Note that y position has to be flipped as glfwGetCursorPos gives the cursor position relative to top left of the screen. The read location must also be multiplied by (buffer size / windowSize) for some displays.
	glReadPixels(x,y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedId =
			data[0] +
			data[1] * 256 +
			data[2] * 256*256;
    return pickedId;
}
