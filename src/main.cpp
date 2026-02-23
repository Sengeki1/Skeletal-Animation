#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "importer.h"
#include "shader.h"
#include "animation.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(900, 800, "Skeletal", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();

    glViewport(0, 0, 900, 800);

    MeshImporter mesh("Assets/Models/Arms/source/arms.fbx", "Assets/Models/Arms/texture");
    Animation anim(
        mesh.animation, 
        mesh.rootNode, 
        mesh.bonesMapping, 
        mesh.bonesOffsetMatrix, 
        mesh.finalBoneMatrix
    );
    Shader shader("default.vert", "default.frag");
    shader.Create();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    float lastFrame = float(glfwGetTime());
    float currentFrame, deltaTime = 0.0f;
    float time = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        time += deltaTime * 1.0f;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Activate();
        anim.update(mesh.rootNode, glm::mat4(1.0f), time);

        for (size_t i = 0; i < mesh.m_VAO.size(); i++) {
            if (mesh.m_Textures[mesh.m_materialIndex[i]] != nullptr) {
                mesh.m_Textures[mesh.m_materialIndex[i]]->Bind(0);
                glUniform1i(glGetUniformLocation(shader.ID, "tex0"), 0);
            }
            mesh.m_VAO[i]->Bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(26.0f, -900.0f, -140.0f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.3f));
            
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "bones"), 
                anim.finalBoneMatrix.size(), GL_FALSE, glm::value_ptr(anim.finalBoneMatrix[0]));
            
            glm::mat4 projection = glm::perspective(glm::radians(90.0f), 900.0f / 800.0f, 0.1f, 1000.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glDrawElements(GL_TRIANGLES, mesh.m_numIndices[i], GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}