#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"

#include "glew.h"
#include "glm.hpp"
#include "gtx/matrix_decompose.hpp"
#include "gtx/transform.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <ctime>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"


#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <assimp/f.h>

#include "Texture.h"


#ifndef M_PI
#define M_PI (std::acos(-1.0))
#endif

//obliczyc kwateriony nastepnie interpolowac.

glm::quat rotationCamera = glm::quat(1, 0, 0, 0);



std::vector<glm::quat> keyRotation;

int index = 0;
bool FOLLOW_CAR = false;

GLuint programPhong;
Core::Shader_Loader shaderLoader;



Core::RenderContext armContext;


std::vector<Core::RenderContext> armContexts;

std::vector<Core::Node> city;

std::vector<Core::Node> robotArm;


float cameraAngle = 0;
glm::vec3 cameraSide;
glm::vec3 cameraPos = glm::vec3(0, 2, 5);
glm::vec3 cameraDir = glm::normalize(-(cameraPos*glm::vec3(1,0,1)));
glm::quat shoulderOrientation;
glm::quat elbowOrientation;

glm::mat4 cameraMatrix, perspectiveMatrix;


glm::vec3 lightDir = glm::normalize(glm::vec3(1, 1, 1));

float oldX, oldY = -1;
float deltaX, deltaY = 0;
bool startAnimation = false;
float animationStartTime = -1;

bool wasZeroKeyPressed = false;

//glm::quat rotation_y;
//glm::quat rotation_x;// = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 1, 0));
glm::quat rotation_y = glm::normalize(glm::angleAxis(209 * 0.03f, glm::vec3(1, 0, 0)));
glm::quat rotation_x = glm::normalize(glm::angleAxis(307 * 0.03f, glm::vec3(0, 1, 0)));
float dy = 0;

float last_time = -1;

int shoulderNodeIndex = 2;
int elbowNodeIndex = 4;

void processInput(GLFWwindow* window)
{
	float deltaTime = 1;
	if (last_time > 0) {
		float deltaTime = (glfwGetTime()- last_time)*60.;
	}
	last_time = glfwGetTime();
	glm::vec3 side = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	float angleSpeed = 0.05f* deltaTime;
	float moveSpeed = 0.05f * deltaTime;

	

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos += side * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos -= side * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += up * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= up * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		startAnimation = true;
		animationStartTime = glfwGetTime();
		wasZeroKeyPressed = wasZeroKeyPressed ? false : true;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (oldX > 0 && oldY > 0) {
		deltaX = x - oldX;
		deltaY = y - oldY;
	}
	oldX = x;
	oldY = y;
	// Oblicz kwaterniony rotacji w osiach X i Y
	glm::quat rotationX = glm::angleAxis(glm::radians(deltaX * 0.01f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat rotationY = glm::angleAxis(glm::radians(deltaY * 0.01f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Zastosuj rotacje do shoulderOrientation
	shoulderOrientation = rotationX * rotationY * shoulderOrientation;
	
	//do myszki
	//robotArm[shoulderNodeIndex].localTransformation = robotArm[shoulderNodeIndex].initialTransformation * glm::toMat4(shoulderOrientation);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		// Obrót wokół osi Z w prawo
		glm::quat rotationZ = glm::angleAxis(glm::radians(angleSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
		shoulderOrientation = rotationZ * shoulderOrientation;
		robotArm[shoulderNodeIndex].localTransformation = robotArm[shoulderNodeIndex].initialTransformation * glm::toMat4(shoulderOrientation);
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		// Obrót wokół osi Z w lewo
		glm::quat rotationZ = glm::angleAxis(glm::radians(-angleSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
		shoulderOrientation = rotationZ * shoulderOrientation;
		robotArm[shoulderNodeIndex].localTransformation = robotArm[shoulderNodeIndex].initialTransformation * glm::toMat4(shoulderOrientation);
	}

	//elbow 
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		// Obrót wokół osi X w górę
		glm::quat rotationX = glm::angleAxis(glm::radians(angleSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		elbowOrientation = rotationX * elbowOrientation;
		robotArm[elbowNodeIndex].localTransformation = robotArm[elbowNodeIndex].initialTransformation * glm::toMat4(elbowOrientation);
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		// Obrót wokół osi X w dół
		glm::quat rotationX = glm::angleAxis(glm::radians(-angleSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		elbowOrientation = rotationX * elbowOrientation;
		robotArm[elbowNodeIndex].localTransformation = robotArm[elbowNodeIndex].initialTransformation * glm::toMat4(elbowOrientation);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		// Obrót wokół osi Y w lewo
		glm::quat rotationY = glm::angleAxis(glm::radians(angleSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
		elbowOrientation = rotationY * elbowOrientation;
		robotArm[elbowNodeIndex].localTransformation = robotArm[elbowNodeIndex].initialTransformation * glm::toMat4(elbowOrientation);
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		// Obrót wokół osi Y w prawo
		glm::quat rotationY = glm::angleAxis(glm::radians(-angleSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
		elbowOrientation = rotationY * elbowOrientation;
		robotArm[elbowNodeIndex].localTransformation = robotArm[elbowNodeIndex].initialTransformation * glm::toMat4(elbowOrientation);
	}


}



glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	return cameraMatrix;
}



void drawObject(Core::RenderContext context, glm::mat4 modelMatrix)
{
	GLuint program = context.material->program;
	glUseProgram(program);
	context.material->init_data();
	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	context.render();
	glUseProgram(0);
}

void renderRecursive(std::vector<Core::Node>& nodes) {
	for (auto node : nodes) {
		if (node.renderContexts.size() == 0) {
			continue;
		}

		glm::mat4 transformation;
		if (node.cacheTransformation != glm::mat4()) {
			// Użyj zcache'owanej macierzy transformacji
			transformation = node.cacheTransformation;

		}
		else{
			transformation = node.localTransformation;
			int parent_id = node.parent;

			while (parent_id > -1) {

				transformation = nodes[parent_id].localTransformation * transformation;
				//std::cout << nodes[parent_id].name << " " << parent_id << std::endl;
				parent_id = nodes[parent_id].parent;

			}
			node.cacheTransformation = transformation;
		}

	
		
		// dodaj odwolania do nadrzednych zmiennych
		for (auto context : node.renderContexts) {
			drawObject(context, transformation);
			
		}
	}

}

float easeInSine(float x) {
	return 1 - std::cos((x *M_PI) / 2);
}


float animationDuration = 3;
void runAnimation() {
	float time = glfwGetTime();
	float timePassed = time - animationStartTime;
	if (timePassed < animationDuration) {
		//float t = timePassed / animationDuration;
		float t = glm::clamp(timePassed / animationDuration, 0.0f, 1.0f);

		t = easeInSine(t);

		//here add animation code
		 // Ustaw orientacje początkowe i końcowe dla stawu shoulder
		/*glm::quat startOrientationShoulder = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
		glm::quat endOrientationShoulder = glm::angleAxis(0.8f, glm::vec3(0, 1, 0));*/

		glm::quat startOrientationShoulder = wasZeroKeyPressed ? glm::angleAxis(0.f, glm::vec3(1, 0, 0)) : glm::angleAxis(0.8f, glm::vec3(0, 1, 0));
		glm::quat endOrientationShoulder = wasZeroKeyPressed ? glm::angleAxis(0.8f, glm::vec3(0, 1, 0)) : glm::angleAxis(0.f, glm::vec3(1, 0, 0));

		// Oblicz interpolowaną orientację dla stawu shoulder
		glm::quat interpolatedOrientationShoulder = glm::slerp(startOrientationShoulder, endOrientationShoulder, t);

		// Ustaw orientacje początkowe i końcowe dla stawu elbow
		/*glm::quat startOrientationElbow = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
		glm::quat endOrientationElbow = glm::angleAxis(-0.8f, glm::vec3(1, 1, 0));*/


		glm::quat startOrientationElbow = wasZeroKeyPressed ? glm::angleAxis(0.f, glm::vec3(1, 0, 0)) : glm::angleAxis(-0.8f, glm::vec3(1, 1, 0));
		glm::quat endOrientationElbow = wasZeroKeyPressed ? glm::angleAxis(-0.8f, glm::vec3(1, 1, 0)) : glm::angleAxis(0.f, glm::vec3(1, 0, 0));


		// Oblicz interpolowaną orientację dla stawu elbow
		glm::quat interpolatedOrientationElbow = glm::slerp(startOrientationElbow, endOrientationElbow, t);


		// Ustaw staw shoulder w interpolowanej orientacji
		robotArm[shoulderNodeIndex].localTransformation = robotArm[shoulderNodeIndex].initialTransformation * glm::toMat4(interpolatedOrientationShoulder);

		// Ustaw staw elbow w interpolowanej orientacji
		robotArm[elbowNodeIndex].localTransformation = robotArm[elbowNodeIndex].initialTransformation * glm::toMat4(interpolatedOrientationElbow);
	}
	else {
		startAnimation = false;
	/*	robotArm[shoulderNodeIndex].localTransformation = robotArm[shoulderNodeIndex].initialTransformation;
		robotArm[elbowNodeIndex].localTransformation = robotArm[elbowNodeIndex].initialTransformation;*/
	}
	
}

void runAnimationContionus() {
	float time = glfwGetTime();
	//float t = glm::sin(time) / 2 + 0.5;

	float period = 2.0f;  // Okres funkcji sinusoidalnej

   // Użyj funkcji sinusoidalnej zamiast triangleWave
	float t = std::fmod(time, period) / period;

	auto start = glm::angleAxis(0.f, glm::vec3(0, 0, 1));
	
	// Ustal orientację końcową dla gripper r
	glm::quat endOrientationR = glm::angleAxis(0.8f, glm::vec3(1, 0, 0));

	// Ustal orientację końcową dla gripper l
	glm::quat endOrientationL = glm::angleAxis(-0.8f, glm::vec3(1, 0, 0));

	// Oblicz interpolowaną orientację dla gripper r
	glm::quat interpolatedOrientationR = glm::slerp(start, endOrientationR, t);

	// Oblicz interpolowaną orientację dla gripper l
	glm::quat interpolatedOrientationL = glm::slerp(start, endOrientationL, t);

	//// Ustaw gripper r i gripper l w interpolowanych orientacjach
	int gripperRIndex = 7;
	int gripperLIndex = 8;

	robotArm[gripperRIndex].localTransformation = robotArm[gripperRIndex].initialTransformation * glm::toMat4(interpolatedOrientationR);

	robotArm[gripperLIndex].localTransformation = robotArm[gripperLIndex].initialTransformation * glm::toMat4(interpolatedOrientationL);

}

void renderScene(GLFWwindow* window)
{
	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix(0.1, 2000);
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.03f, 0.1f, 1.0f);

	if (!robotArm.empty()) {
		renderRecursive(robotArm);
	}
	glUseProgram(0);
	glfwSwapBuffers(window);
}






Core::Material* loadColorMaterial(aiMaterial* material) {
	aiString colorPath;
	// use for loading textures

	aiString name;
	material->Get(AI_MATKEY_NAME, name);
	aiColor3D color(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

	printf("material named %s\n", name.C_Str());
	printf("diffuse color value: %f %f %f\n", color.r, color.g, color.b);
	Core::ColorPhongMaterial* result = new Core::ColorPhongMaterial();
	result->color = glm::vec3(color.r, color.g, color.b);
	result->program = programPhong;

	return result;
}




void loadRecusive(const aiScene* scene, aiNode* node, std::vector<Core::Node>& nodes, std::vector<Core::Material*> materialsVector, int parentIndex) {
	int index = nodes.size();
	nodes.push_back(Core::Node());
	nodes[index].parent = parentIndex;
	nodes[index].localTransformation = Core::mat4_cast(node->mTransformation);

	nodes[index].initialTransformation = Core::mat4_cast(node->mTransformation);

	nodes[index].name= node->mName.C_Str();
	for (int i = 0; i < node->mNumMeshes; i++) {
		Core::RenderContext context;
		context.initFromAssimpMesh(scene->mMeshes[node->mMeshes[i]]);
		context.material = materialsVector[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];
		nodes[index].renderContexts.push_back(context);
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		loadRecusive(scene, node->mChildren[i], nodes, materialsVector, index);
	}
}
void loadRecusive(const aiScene* scene, std::vector<Core::Node>& nodes, std::vector<Core::Material*> materialsVector) {

	loadRecusive(scene, scene->mRootNode, nodes, materialsVector, -1);
}

void initModels() {
	Assimp::Importer importer;
	//replace to get more buildings, unrecomdnded
	//const aiScene* scene = importer.ReadFile("models/blade-runner-style-cityscapes.fbx", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	const aiScene* scene = importer.ReadFile("models/robot_arm_color.fbx", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	 std::vector<Core::Material*> materialsVector;

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	for (int i = 0; i < scene->mNumMaterials; i++) {
		materialsVector.push_back(loadColorMaterial(scene->mMaterials[i]));
	}
	loadRecusive(scene, robotArm, materialsVector);


	//Recovering points from fbx
	const aiScene* points = importer.ReadFile("models/path.fbx", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	auto root = points->mRootNode;

	for (int i = 0; i < root->mNumChildren; i++) {
		auto node = root->mChildren[i];
		//std::cout << "glm::vec3(" << node->mTransformation.a4 << "f, " << node->mTransformation.b4 << "f, " << node->mTransformation.c4 << "f), " << std::endl;
		//std::cout << node->mName.C_Str() << std::endl;
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void init(GLFWwindow* window){
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	programPhong = shaderLoader.CreateProgram("shaders/shader_spec.vert", "shaders/shader_spec.frag");
	initModels();
	

}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(programPhong);
}


// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		if (startAnimation) {
			runAnimation();
		}
		else {
			//runAnimationContionus();
			processInput(window);
		}

		renderScene(window);
		glfwPollEvents();
	}
}
