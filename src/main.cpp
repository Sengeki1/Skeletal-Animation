#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "importer.h"
#include "shader.h"

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

    glViewport(0, 0, 900.0f, 800.0f);

    MeshImporter mesh("Assets/Models/Arms/source/arms.fbx", "Assets/Models/Arms/texture");
    Shader shader("default.vert", "default.frag");

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Activate();
        for (size_t i = 0; i < mesh.m_VAO.size(); i++) {
            if (mesh.m_Textures[mesh.m_materialIndex[i]] != nullptr) {
                mesh.m_Textures[mesh.m_materialIndex[i]]->Bind(0);
                glUniform1i(glGetUniformLocation(shader.ID, "tex0"), 0);
            }
            mesh.m_VAO[i]->Bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(10.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
            glm::mat4 projection = glm::perspective(glm::radians(90.0f), 900.0f / 800.0f, 0.1f, 1000.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 1000);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}