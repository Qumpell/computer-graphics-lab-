#pragma once
#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"

#include "Shader_Loader.h"
#include "Render_Utils.h"





float randomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}
int randomInt(int a, int b)
{
    if (a > b)
        return randomInt(b, a);
    if (a == b)
        return a;
    return a + (rand() % (b - a));
}
float randomFloat(int a, int b)
{
    if (a > b)
        return randomFloat(b, a);
    if (a == b)
        return a;

    return (float)randomInt(a, b) + randomFloat();
}
// funkcja renderujaca scene    
void renderScene(GLFWwindow* window)
{
    float a = randomFloat(0, 1);
    float b = randomFloat(0, 1);
    float c = randomFloat(0, 1);
    float d = randomFloat(0, 1);
    float time = glfwGetTime();
    
    // ZADANIE: Przesledz kod i komentarze
    // ZADANIE: Zmien kolor tla sceny, przyjmujac zmiennoprzecinkowy standard RGBA
    //glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    // zmiana za pomoca time
   // glClearColor(sin(time)*0.5f + 0.5f, cos(time) *0.5f + 0.5f,sin(time)*0.5f * cos(time) * 0.5f, abs(sin(time)));
    //zmiana za pomoca losowego float
    glClearColor(a, b, c, d);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Powinno byc wywolane po kazdej klatce
    glfwSwapBuffers(window);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void shutdown(GLFWwindow* window)
{
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