#include <iostream>
#include <random>
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
        glfwCreateWindow(800, 600, "My First Game", nullptr, nullptr);

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
        0.0f, 0.0f, 0.0f, // 左上
        0.15f, 0.0f, 0.0f, // 右上
        0.15f, -0.05f, 0.0f, // 右下
        0.0f, -0.05f, 0.0f //左下
    };

    float scoreLeft = 0.0f, scoreRight = 0.1f;
    float scoreboard[] = { //座標, テクスチャ座標
        -1.0f, 1.0f, 0.0f, scoreLeft, 1.0f, // 左上
        -0.85f, 1.0f, 0.0f, scoreRight, 1.0f, // 右上
        -0.85f, 0.75f, 0.0f, scoreRight, 0.0f, // 右下
        -1.0f, 0.75f, 0.0f, scoreLeft, 0.0f //左下
    };

    float result[] = { //座標, テクスチャ座標
        -0.25f, 0.1f, 0.0f, 0.0f, 1.0f, // 左上
        0.25f, 0.1f, 0.0f, 1.0f, 1.0f, // 右上
        0.25f, -0.1f, 0.0f, 1.0f, 0.0f, // 右下
        -0.25f, -0.1f, 0.0f, 0.0f, 0.0f //左下
    };

    unsigned int indices[] = {
        1, 2, 0,
        2, 3, 0
    };

    unsigned int VBOs[4], VAOs[4], EBO;
    glGenBuffers(4, VBOs);
    glGenVertexArrays(4, VAOs);
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(scoreboard), scoreboard, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(result), result, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    unsigned int scoreTexture, resultTexture;
    glGenTextures(1, &scoreTexture);
    glBindTexture(GL_TEXTURE_2D, scoreTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    int texWidth1, texHeight1, nrChannels1;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data1 = stbi_load("assets/score.png", &texWidth1, &texHeight1, &nrChannels1, 0);
    if (data1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth1, texHeight1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);

    glGenTextures(1, &resultTexture);
    glBindTexture(GL_TEXTURE_2D, resultTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    int texWidth2, texHeight2, nrChannels2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("assets/clear.png", &texWidth2, &texHeight2, &nrChannels2, 4);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth2, texHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    Shader ourShader("shaders/basic.vert", "shaders/basic.frag");
    Shader scoreShader("shaders/score.vert", "shaders/score.frag");

    glm::vec3 playerPosition = glm::vec3(0.0f);
    glm::mat4 model;
    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    int offsetLoc = glGetUniformLocation(scoreShader.ID, "texOffset");

    bool collision = true, isClear = false;
    const float speed = 0.5f;

    glm::vec3 targetPosition(0.5f, 0.5f, 0.0f);

    int score = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(-1.0f, 0.85f);
    std::uniform_real_distribution<float> distY(-0.95f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if(isClear)
        {
            scoreShader.use();
            glm::vec2 zero = glm::vec2(0.0f);
            glUniform2fv(offsetLoc, 1, glm::value_ptr(zero));
            glBindTexture(GL_TEXTURE_2D, resultTexture);
            glBindVertexArray(VAOs[3]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        else
        {
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                playerPosition.y += speed * deltaTime;
                if(playerPosition.y >= 1.0f) playerPosition.y = 1.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                playerPosition.x -= speed * deltaTime;
                if(playerPosition.x <= -0.875f) playerPosition.x = -0.875f;
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                playerPosition.y -= speed * deltaTime;
                if(playerPosition.y <= -0.875f) playerPosition.y = -0.875f;
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                playerPosition.x += speed * deltaTime;
                if(playerPosition.x >= 0.875f) playerPosition.x = 0.875f;
            }

            collision = true;

            if(targetPosition.x > playerPosition.x + 0.125f)
            {
                collision = false;
            }
            else if(targetPosition.x + 0.15f < playerPosition.x - 0.125f)
            {
                collision = false;
            }
            else if(targetPosition.y - 0.05f > playerPosition.y)
            {
                collision = false;
            }
            else if(targetPosition.y < playerPosition.y - 0.125f)
            {
                collision = false;
            }

            if(collision)
            {
                score++;
                if(score == 10)
                {
                    isClear = true;
                }
                else
                {
                    targetPosition.x = distX(gen);
                    targetPosition.y = distY(gen);
                }
            }

            ourShader.use();

            //自機の描画
            model = glm::translate(glm::mat4(1.0f), playerPosition);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            //ターゲットの描画
            model = glm::translate(glm::mat4(1.0f), targetPosition);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAOs[1]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            scoreShader.use();

            //スコアの描画
            glm::vec2 offset(score * 0.1f, 0.0f);
            glUniform2fv(offsetLoc, 1, glm::value_ptr(offset));
            glBindTexture(GL_TEXTURE_2D, scoreTexture);
            glBindVertexArray(VAOs[2]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &scoreTexture);
    glDeleteTextures(1, &resultTexture);

    glDeleteVertexArrays(4, VAOs);
    glDeleteBuffers(4, VBOs);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}