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



GLuint program;
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;
Core::RenderContext asteroidContext1;
Core::RenderContext asteroidContext2;

//glm::vec3 spaceshipPos;
//glm::vec3 spaceshipDir;
glm::vec3 cameraPos = glm::vec3(-4.f, 0, 0);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 spaceshipPos = cameraPos + 1.5f * cameraDir + glm::vec3(0, -0.5f, 0);
glm::vec3 spaceshipDir = cameraDir;
float lastFrameTime;
float deltaTime;



GLuint VAO,VBO;

float aspectRatio = 1.f;

glm::vec3 lightColor = glm::vec3(0.9, 0.7, 0.8);

glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir,glm::vec3(0.f,1.f,0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide,cameraDir));
	//glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});

	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);

	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	//cameraMatrix = glm::mat4({
	//	1.,0.,0.,cameraPos.x,
	//	0.,1.,0.,cameraPos.y,
	//	0.,0.,1.,cameraPos.z,
	//	0.,0.,0.,1.,
	//	});

	//cameraMatrix = glm::transpose(cameraMatrix);
	//return Core::createViewMatrix(cameraPos, cameraDir, up);

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{
	
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float f = 20.;
	float a1 = glm::min(aspectRatio, 1.f);
	float a2 = glm::min(1 / aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,1,0.,0.,
		0.,0.,(f+n) / (n - f),2*f * n / (n - f),
		0.,0.,-1.,0.,
		});

	
	perspectiveMatrix=glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}

void drawObjectColor(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color) {

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glUniform3f(glGetUniformLocation(program, "color"),color.x, color.y, color.z);
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	Core::DrawContext(context);
}
void drawPlanet(Core::RenderContext& context, const glm::mat4& parentMatrix, float radius, const glm::vec3& color, float rotationSpeed, float distance, bool is_with_moon) {
	float time = glfwGetTime();

	// Kąt obrotu
	float rotationAngle = glm::radians(time * rotationSpeed);


	glm::vec3 planetPosition(0.0f, 0.0f, 0.0f);
	// Wyznacz nową pozycję na okręgu dla planety
	glm::vec3 newPosition;
	newPosition.x = planetPosition.x;
	newPosition.y = planetPosition.y + distance * cos(rotationAngle);
	newPosition.z = planetPosition.z + distance * sin(rotationAngle);

	// Macierz modelu planety
	glm::mat4 planetMatrix = glm::scale(glm::translate(parentMatrix, newPosition), glm::vec3(radius));

	// Narysuj planetę
	drawObjectColor(context, planetMatrix, color);

	if (is_with_moon) {
		drawPlanet(context, planetMatrix, 0.2f, glm::vec3(0.3f, 0.0f, 1.0f), 80.0f, 1.4f, false);
	}

}
void drawAsteroidBelt(Core::RenderContext& context, const glm::mat4& sunMatrix, int count, float innerRadius, float outerRadius) {
	float time = glfwGetTime();
	float speed = 1.0f;
	for (int i = 0; i < count; ++i) {
		float angle = glm::radians(time * speed * i);  // Przykładowa prędkość obrotu asteroidy

		// Pozycja asteroidy na pasie
		float radius = glm::mix(innerRadius, outerRadius, glm::sin(angle * 2.0f));  // Użyj funkcji sinus, aby uzyskać efekt falowania
		glm::vec3 asteroidPosition(
			0.0f,
			radius * sin(angle),
			radius * cos(angle)
		);

		// Macierz modelu asteroidy
		glm::mat4 asteroidMatrix = glm::scale(glm::translate(sunMatrix, asteroidPosition), glm::vec3(0.001f));  // Skaluj asteroidy według potrzeb

		// Narysuj asteroidę
		drawObjectColor(context, asteroidMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	}
}

void drawSolarSystem(Core::RenderContext& context) {
	float time = glfwGetTime();

	// Narysuj słońce
	drawObjectColor(context, glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	// Narysuj planety
	drawPlanet(context, glm::mat4(1.0f), 0.5f, glm::vec3(0.0f, 0.0f, 1.0f), 45.0f, 1.9f, true);
	drawPlanet(context, glm::mat4(1.0f), 0.4f, glm::vec3(0.5f, 0.0f, 0.5f), 50.0f, 3.1f, false);
	drawPlanet(context, glm::mat4(1.0f), 0.3f, glm::vec3(0.0f, 0.5f, 0.0f), 55.0f, 4.5f, false);
	drawPlanet(context, glm::mat4(1.0f), 0.2f, glm::vec3(0.8f, 0.2f, 0.2f), 60.0f, 6.0f, false);
	drawPlanet(context, glm::mat4(1.0f), 0.15f, glm::vec3(0.2f, 0.2f, 0.8f), 65.0f, 7.5f, false);

	// Narysuj pas asteroid
	//drawAsteroidBelt(asteroidContext1, glm::mat4(1.0f), 100, 12.0f, 14.0f);
	drawAsteroidBelt(asteroidContext1, glm::mat4(1.0f), 100, 13.0f, 15.0f);
	drawAsteroidBelt(asteroidContext2, glm::mat4(1.0f), 100, 15.0f, 17.0f);
}

glm::mat4 createSpaceshipRotationMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, spaceshipDir));
	//glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 cameraRotationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-spaceshipDir.x,-spaceshipDir.y,-spaceshipDir.z,0,
		0.,0.,0.,1.,
		});



	cameraRotationMatrix = glm::transpose(cameraRotationMatrix);

	//glm::mat4 cameraMatrix = cameraRotationMatrix * glm::translate(-cameraPos);

	return cameraRotationMatrix;
}
glm::mat4 createSpaceshipMatrix()
{
	// Ustal wektor w górę (załóżmy, że oś Y)
	glm::vec3 spaceshipUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Utwórz macierz rotacji, która obraca statek w kierunku, w którym się porusza
	glm::mat4 spaceshipRotationMatrix = glm::lookAt(glm::vec3(0.0f), spaceshipDir, spaceshipUp);

	// Utwórz macierz translacji dla pozycji statku
	glm::mat4 spaceshipTranslationMatrix = glm::translate(glm::mat4(1.0f), spaceshipPos);

	// Połącz obie macierze, aby uzyskać macierz modelu statku
	glm::mat4 spaceshipModelMatrix = spaceshipTranslationMatrix * spaceshipRotationMatrix;

	return spaceshipModelMatrix;
}
void renderScene(GLFWwindow* window)
{
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glDisable(GL_DEPTH_TEST);
	drawObjectColor(sphereContext, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(4.f, 3.f, 5.f)), glm::vec3(0.03f)), glm::vec3(1.0f, 0.9f, 0.2f));
	drawObjectColor(sphereContext, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(7.f, 4.f, 2.f)), glm::vec3(0.03f)), glm::vec3(1.0f, 0.9f, 0.2f));
	drawObjectColor(sphereContext, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -3.f, -2.f)), glm::vec3(0.03f)), glm::vec3(1.0f, 0.9f, 0.2f));
	drawObjectColor(sphereContext, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 5.f, 3.f)), glm::vec3(0.03f)), glm::vec3(1.0f, 0.9f, 0.2f));
	drawObjectColor(sphereContext, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-3.f, -3.f, -4.f)), glm::vec3(0.03f)), glm::vec3(1.0f, 0.9f, 0.2f));
	drawObjectColor(sphereContext, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 5.f)), glm::vec3(0.03f)), glm::vec3(1.0f, 0.9f, 0.2f));
	glEnable(GL_DEPTH_TEST);




	glm::mat4 transformation;
	float time = glfwGetTime();

	deltaTime = time - lastFrameTime;
	lastFrameTime = time;
	// Ucięcie deltaTime od góry do 0.1
	deltaTime = glm::min(deltaTime, 0.1f);

	glUseProgram(program);
	
	drawSolarSystem(sphereContext);

	glm::mat4 spaceshipTranslationMatrix = glm::translate(createSpaceshipRotationMatrix(), spaceshipPos);
	//drawObjectColor(shipContext,spaceshipTranslationMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
	//drawObjectColor(shipContext, glm::inverse(createSpaceshipRotationMatrix() * spaceshipTranslationMatrix), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 spaceshipModelMatrix = createSpaceshipMatrix();
	drawObjectColor(shipContext, spaceshipModelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
	//drawObjectColor(asteroidContext1, glm::scale(spaceshipModelMatrix, glm::vec3(0.0005f)), glm::vec3(1.0f, 0.0f, 0.0f));

	glUseProgram(0);
	glfwSwapBuffers(window);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / float(height);
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
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_4_1.vert", "shaders/shader_4_1.frag");

	loadModelToContext("./models/sphere.obj",sphereContext);
	loadModelToContext("./models/asteroid.obj", asteroidContext1);
	loadModelToContext("./models/asteroid2.obj", asteroidContext2);
	loadModelToContext("./models/spaceship.obj",shipContext);
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
		cameraPos += cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));


	// Poruszanie statkiem
	float spaceshipSpeed = 5.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		spaceshipPos += spaceshipDir * spaceshipSpeed * deltaTime;;  // Przesunięcie do przodu
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		spaceshipPos -= spaceshipDir * spaceshipSpeed * deltaTime; ;  // Przesunięcie do tyłu
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		//spaceshipDir = glm::rotateY(spaceshipDir, glm::radians(5.0f));  // Obrót w lewo
		//spaceshipDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(spaceshipDir, 0)) * deltaTime;;
		spaceshipPos -= glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.0f, 1.0f, 0.0f))) * spaceshipSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		//spaceshipDir = glm::rotateY(spaceshipDir, glm::radians(-5.0f));  // Obrót w prawo
		//spaceshipDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(spaceshipDir, 0)) * deltaTime;
		spaceshipPos += glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.0f, 1.0f, 0.0f))) * spaceshipSpeed * deltaTime;
	}



	// Aktualizacja kamery
	//cameraPos = spaceshipPos + 1.5f * spaceshipDir + glm::vec3(0, -0.5f, 0);
	//cameraDir = spaceshipDir;
	cameraPos = spaceshipPos + 1.5f * spaceshipDir + glm::vec3(0.0f, -0.5f, 0.0f);
	cameraDir = -spaceshipDir;
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