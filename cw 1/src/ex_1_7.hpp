#pragma once
#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"

GLuint program; // Shader ID

GLuint quadVAO;

Core::Shader_Loader shaderLoader;

std::vector<glm::vec3> quadsPositions;
glm::vec3 quadsPosition(0.0f, 0.0f, 0.0f);
float rotationAngle = 0.0f;


void renderScene(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	float time = glfwGetTime();

    glUseProgram(program);
    
    //glm::mat4 transformation;
    //float y = sin(time) * 0.5;
    //float angle = time * 50.0f;
    //glm::vec3 translationVector(0.0f, y, 0.0f);
    glm::vec3 rotationVector(0.0f, 0.0f, 1.0f);
    //glm::mat4 glm::translate(glm::vec3 const& v);
    //glm::mat4 transformation = glm::rotate(glm::translate(glm::mat4(1.0f), translationVector), glm::radians(angle), rotationVector);
    //glm::mat4 transformation = glm::rotate(glm::translate(glm::mat4(1.0f), quadsPosition), glm::radians(rotationAngle), rotationVector);



    //glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
    //Core::drawVAOIndexed(quadVAO, 6);
    
    for (const auto& position : quadsPositions) {
        glm::mat4 transformation = glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotationAngle), rotationVector);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
        Core::drawVAOIndexed(quadVAO, 6);
    }

    glUseProgram(0);

    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{   
    //int width, height;
    //glfwGetWindowSize(window, &width, &height);
    //quadsPosition.x = ((2 * xpos) / width) - 1;
    //quadsPosition.y = -((2*ypos / height) - 1);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xpos, ypos, newXPos, newYPos;
        glfwGetCursorPos(window, &xpos, &ypos);
        newXPos = ((2 * xpos) / width) - 1;
        newYPos = -((2 * ypos / height) - 1);
        quadsPositions.push_back(glm::vec3(newXPos, newYPos, 0.0f));
        printf("%f,%f\n", xpos, ypos);
    }
}

void joystick_callback(int jid, int event) {
    int count;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

    if (axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0.5f) {
        float triggerValue = axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
        rotationAngle = 5.0f * triggerValue; 
        std::cout << "Szybkoœæ obrotu: " << rotationAngle << std::endl;
    }
}

void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetJoystickCallback(joystick_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    program = shaderLoader.CreateProgram("shaders/shader_1_2.vert", "shaders/shader_1_2.frag");

    float points[] = {
        -0.1,-0.1,0.0,1.0,
         0.1,-0.1,0.0,1.0,
         0.1, 0.1,0.0,1.0,
        -0.1, 0.1,0.0,1.0,
    };
    unsigned int indices[] = {
        0,1,3,
        1,2,3,
    };
    quadVAO = Core::initVAOIndexed(points, indices, 4, 4, 6);

    //Przekopiuj kod do ladowania z poprzedniego zadania
}

void shutdown(GLFWwindow* window)
{
    shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{   
    float speed = 0.01f;
    float rotationSpeed = 5.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        quadsPosition.y += speed;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        quadsPosition.y -= speed;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        quadsPosition.x -= speed;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        quadsPosition.x += speed;
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        rotationAngle -= rotationSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        rotationAngle += rotationSpeed;
    }

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