#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"

#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

#define M_PI 3.14159265358979323846

GLuint program, program_box;
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;

glm::vec3 cameraPos = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f);
GLuint VAO,VBO;

float aspectRatio = 1.f;


glm::mat4 createCameraMatrix(){
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	glm::mat4 cameraRotationMatrix = glm::mat4({
		cameraSide.x, cameraSide.y, cameraSide.z, 0.,
		cameraUp.x, cameraUp.y, cameraUp.z, 0.,
		-cameraDir.x, -cameraDir.y, -cameraDir.z,0.,
		0., 0., 0., 1.,
	});
	cameraRotationMatrix = glm::transpose(cameraRotationMatrix);

	glm::mat4 cameraTranslationMatrix = glm::mat4({
		1.,0.,0.,-cameraPos.x,
		0.,1.,0.,-cameraPos.y,
		0.,0.,1.,-cameraPos.z,
		0.,0.,0.,1.,
	});
	cameraTranslationMatrix = glm::transpose(cameraTranslationMatrix);
	glm::mat4 cameraMatrix = cameraRotationMatrix * cameraTranslationMatrix;

	return cameraMatrix;

}

glm::mat4 createPerspectiveMatrix(float fov, float aspectRatio){


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	float n = 0.1f;
	float f = 100.0f;

	float S = 1.0f / tan((fov/2.0f) * M_PI/180.0f);
	//printf("%f\n", aspectRatio);
	glm::mat4 perspectiveMatrix = glm::mat4({
		S,0.,0.,0.,
		0.,S * aspectRatio,0.,0.,
		0.,0.,(n+f)/(n-f), (2*n*f)/(n - f),
		0.,0.,-1.,0.,
		});

	
	perspectiveMatrix=glm::transpose(perspectiveMatrix);

	//return perspectiveMatrix;
	return glm::perspective(glm::radians(fov), aspectRatio, n, f);
}
glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
	glm::mat4 transformationMatrix = glm::mat4(1.0f);

	// Translacja
	transformationMatrix = glm::translate(transformationMatrix, translation);

	// Rotacja
	transformationMatrix = glm::rotate(transformationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Skala
	transformationMatrix = glm::scale(transformationMatrix, scale);

	return transformationMatrix;
}
void renderScene(GLFWwindow* window)
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 transformation;

	glUseProgram(program_box);
	
	float fov = 45.0f;



	glBindVertexArray(VAO);
	transformation = createPerspectiveMatrix(fov, aspectRatio) * createCameraMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program_box, "transformation"), 1, GL_FALSE, (float*)&transformation);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	Core::DrawContext(sphereContext);


	glm::vec3 translation = glm::vec3(-1.0f, -0.5f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 60.0f, 0.0f);
	glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f);
	
	// Translacja
	glm::mat4 transformationMatrix = createTransformationMatrix(translation, rotation, scale);
	glm::mat4 transformation1 = transformationMatrix * createPerspectiveMatrix(fov, aspectRatio)  * createCameraMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program_box, "transformation"), 1, GL_FALSE, (float*)&transformation1);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	Core::DrawContext(shipContext);

	glm::vec3 translation1 = glm::vec3(0.5f, -1.0f, 0.0f);
	glm::vec3 rotation1 = glm::vec3(0.0f, 70.0f, 0.0f);
	glm::vec3 scale1 = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 transformationMatrix1 = createTransformationMatrix(translation1, rotation1, scale1);
	glm::mat4 transformation2 = transformationMatrix1 * createPerspectiveMatrix(fov, aspectRatio) * createCameraMatrix();
	//glUniformMatrix4fv(glGetUniformLocation(program_box, "transformation"), 1, GL_FALSE, (float*)&transformation2);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	


	glBindVertexArray(0);

	glUseProgram(0);
	glfwSwapBuffers(window);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = float(width) / float(height);

	//bonus task
	//aspectRatio = (float)height / float(width);
	glViewport(0, 0, width, height);
}
void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	
    std::vector<float> combinedData; // Zbiorcza tablica dla wierzchołków, normalnych i współrzędnych tekstur
    std::vector<unsigned int> indices;

	context.initFromAssimpMesh(scene->mMeshes[0]);
	
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		
	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_3_1.vert", "shaders/shader_3_1.frag");
	program_box = shaderLoader.CreateProgram("shaders/shader_2_1.vert", "shaders/shader_2_1.frag");

	loadModelToContext("./models/sphere.obj", sphereContext);
	loadModelToContext("./models/spaceship.obj", shipContext);

	

	glGenVertexArrays(1, &VAO);
	// 2. Powiąż stworzone VAO za pomocą funkcji glBindVertexArray
	glBindVertexArray(VAO);
	// II Zainicjalizuj VBO
	// 3. Stwórz VertexBufferObject do przechowywania pozycji wierzchołków za pomocą funkcji glGenBuffers
	glGenBuffers(1, &VBO);
	// 4. Powiąż stworzone VBO za pomocą funkcji glBindBuffer(GLenum target, GLuint )
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 5. Zaalokuj w GPU miejsce na tablice box i zaalakokuj ją za pomocą glBufferData
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	// II Stwórz definicję danych w buforze
	// 6. Aktywuj atrybut powiązany z pozycją wierchołków za pomocą glEnableVertexAttribArray(GLuint index). Indeks jest zapisany w shaderze wierzchołków w 3. lini. Można odpytać shader o indeks za pomocą funkcji glGetAttribLocation(GL uint program, const GLchar *name)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));


}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f,1.f,0.f)));



	float angleSpeed = 0.05f;
	float moveSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += cameraSide * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		p.y += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		p.y -= moveSpeed;
	
	
	
	cameraDir = glm::normalize(p - cameraPos);
}

// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		renderScene(window);
		glfwPollEvents();
	}
}
//}