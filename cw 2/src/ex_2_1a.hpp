﻿#pragma once
#include "glew.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

#include "Shader_Loader.h"
#include "Render_Utils.h"

#include "Box.cpp"


GLuint program;
Core::Shader_Loader shaderLoader;

GLuint VAO,VBO;
void renderScene(GLFWwindow* window)
{
	float time = glfwGetTime();
	
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


	// ZADANIE: W zalaczonym wyzej pliku Box.cpp znajduja sie tablice pozycji i kolorow wierzcholkow prostopadloscianu. Przesun go o wektor (0.5, 0.5, -0.2) i wyswietl go.
	// 
	// Do shadera uzytego w tym zadaniu nalezy przeslac nie tylko pozycje, ale rowniez kolory wierzcholkow.
	// W funkcji init należy przesłać pozycje i kolory do GPU 

	glUseProgram(program);

	//przesylanie czasu do shadera
	glUniform1f(glGetUniformLocation(program, "time"), time);
	
	//przesylanie koloru tla
	glm::vec4 backgroundColor(0.0f, 0.3f, 0.3f, 1.0f);
	int backgroundColorLocation = glGetUniformLocation(program, "backgroundColor");
	glUniform4fv(backgroundColorLocation, 1, glm::value_ptr(backgroundColor));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Kąty Eulera w radianach
	float angleX = glm::radians(45.0f); // kąt 45 stopni wokół osi X
	float angleY = glm::radians(sin(time)* 60.0f); // kąt 3 stopni wokół osi Y
	float angleZ = glm::radians(0.0f); // kąt 60 stopni wokół osi Z

	// Tworzenie macierzy rotacji
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(angleX, angleY, angleZ);

	glm::vec3 translationVector(0.5f, 0.5f, -0.2f);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translationVector);
	//glm::mat4 transformation = translationMatrix * rotationMatrix;
	glm::mat4 transformation = rotationMatrix;
	//glm::mat4 transformation;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Powiąż stworzone VAO za pomocą funkcji glBindVertexArray
	// wykorzystaj glDrawArrays do narysowania prostopadłościanu

	glUseProgram(0);
	glfwSwapBuffers(window);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_2_1.vert", "shaders/shader_2_1.frag");
	// Dokładny opis działania funkcji z poniższych punktów można znaleźć na slajdach z wykładu, w dokumentacji https://www.khronos.org/registry/OpenGL-Refpages/gl4/ funkcje są opisane także na stronach https://learnopengl.com/Getting-started/Hello-Triangle https://learnopengl.com/Getting-started/Shaders oraz https://open.gl/drawing
	// W tablicy box są pozycje i kolory kostki, celem zajęć jest narysowanie tej kostki 
	// Zainicjalizuj VertexArrayObject (VAO) i zapisz w nim atrybuty prostopadłościanu
	// I Stwórz VAO
	// 1. Stwórz nowy VAO za pomocą glGenVertexArrays i przypisz jego adres do zmiennej globalnej
	glGenVertexArrays(1, &VAO);
	// 2. Powiąż stworzone VAO za pomocą funkcji glBindVertexArray
	glBindVertexArray(VAO);
	// II Zainicjalizuj VBO
	// 3. Stwórz VertexBufferObject do przechowywania pozycji wierzchołków za pomocą funkcji glGenBuffers
	glGenBuffers(1, &VBO);
	// 4. Powiąż stworzone VBO za pomocą funkcji glBindBuffer(GLenum target, GLuint )
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 5. Zaalokuj w GPU miejsce na tablice box i zaalakokuj ją za pomocą glBufferData
	//std::cout << sizeof(box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), &box, GL_STATIC_DRAW);
	// II Stwórz definicję danych w buforze
	// 6. Aktywuj atrybut powiązany z pozycją wierchołków za pomocą glEnableVertexAttribArray(GLuint index). Indeks jest zapisany w shaderze wierzchołków w 3. lini. Można odpytać shader o indeks za pomocą funkcji glGetAttribLocation(GL uint program, const GLchar *name)
	GLuint indexPos = glGetAttribLocation(program, "vertexPosition");
	glEnableVertexAttribArray(indexPos);
	// 7. Aktywuj atrybut powiązany z kolorem wierchołków za pomocą glEnableVertexAttribArray(GLuint index). Indeks jest zapisany w shaderze wierzchołków w 4. lini. Można odpytać shader o indeks za pomocą funkcji glGetAttribLocation(GL uint program, const GLchar *name)
	GLuint indexCol = glGetAttribLocation(program, "vertexColor");
	glEnableVertexAttribArray(indexCol);
	// 8. Zdefiniuj jak OpenGL powinien interpretować dane za pomocą glVertexAttribPointer
	glVertexAttribPointer(indexPos, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glVertexAttribPointer(indexCol, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	// w renderScene!
	//glBindVertexArray(VAO);`
	///glDrawArrays(GL_TRIANGLES, 0, 36);

	// 9. Uwolnij VAO za pomocą funkcji glBindVertexArray(0)

	glBindVertexArray(0);
}



void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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