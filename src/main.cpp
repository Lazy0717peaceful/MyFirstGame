#include <iostream>
#include "Shader.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "OpenGL Practice", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0)
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //ここまでウィンドウ作ってる

    float player[] = {
        0.0f, 0.0f, 0.0f,
        -0.125f,  -0.125f, 0.0f,
        0.125f,  -0.125f, 0.0f
    };

    float target[] = {
        0.5f, 0.5f, 0.0f,
        0.65f, 0.5f, 0.0f,
        0.65f, 0.45f, 0.0f,
        0.5f, 0.45f, 0.0f
    };

    unsigned int indices[] = {
        1, 2, 0,
        2, 3, 0
    };

    unsigned int VBOs[2], VAOs[2], EBO;
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(target), target, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);    

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glBindVertexArray(0); 

    Shader ourShader("shaders/basic.vert", "shaders/basic.frag");

    glm::vec3 position = glm::vec3(0.0f);
    glm::mat4 model;
    int modelLoc = glGetUniformLocation(ourShader.ID, "model");

    bool targetAlive = true;
    const float speed = 0.5f;

    glEnable(GL_DEPTH_TEST);

    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position.y += speed * deltaTime;

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position.x -= speed * deltaTime;

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position.y -= speed * deltaTime;

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position.x += speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if(targetAlive)
        {
            float playerLeft = position.x - 0.125f;
            float playerRight = position.x + 0.125f;
            float playerBottom = position.y - 0.125f;
            float playerTop = position.y;

            float targetLeft = 0.5f;
            float targetRight = 0.65f;
            float targetBottom = 0.45f;
            float targetTop = 0.5;

            bool colision = true;
            if(targetLeft > playerRight)
            {
                colision = false;
            }
            else if(targetRight < playerLeft)
            {
                colision = false;
            }
            else if(targetBottom > playerTop)
            {
                colision = false;
            }
            else if(targetTop < playerBottom)
            {
                colision = false;
            }

            if(colision) targetAlive = false;
        }

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        model = glm::translate(glm::mat4(1.0f), position);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if(targetAlive)
        {
            model = glm::mat4(1.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAOs[1]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}