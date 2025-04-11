#include "GLRenderSystem.h"
#include "../include/glad.h"
#include <vector>
#include <iostream>

// Simple shader loader helpers
GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc);

namespace {
    glm::mat4 worldMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projMatrix{1.0f};

    GLuint shaderProgram = 0;
    GLuint vao = 0;
    GLuint vbo = 0;

    GLint uWorld, uView, uProj;

    struct Light {
        glm::vec3 position;
        glm::vec3 Ia, Id, Is;
        bool enabled = false;
    };

    Light lights[8];

    RenderMode renderMode = RenderMode::Triangles;
}

void GLRenderSystem::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    const char* vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;

        uniform mat4 uWorld;
        uniform mat4 uView;
        uniform mat4 uProj;

        out vec3 FragPos;
        out vec3 Normal;

        void main() {
            FragPos = vec3(uWorld * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(uWorld))) * aNormal;
            gl_Position = uProj * uView * vec4(FragPos, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        in vec3 FragPos;
        in vec3 Normal;
        out vec4 FragColor;
        struct Light {
            vec3 position;
            vec3 Ia;
            vec3 Id;
            vec3 Is;
            bool enabled;
        };

        uniform Light lights[8];
        uniform vec3 viewPos;
        uniform vec3 materialColor;

        void main() {
            vec3 color = vec3(0.0);
            vec3 N = normalize(Normal);
            vec3 V = normalize(viewPos - FragPos);

            for (int i = 0; i < 8; ++i) {
                if (!lights[i].enabled) continue;
                vec3 L = normalize(lights[i].position - FragPos);
                vec3 R = reflect(-L, N);
                float diff = max(dot(N, L), 0.0);
                float spec = pow(max(dot(R, V), 0.0), 32.0);
                color += lights[i].Ia * materialColor
               + lights[i].Id * diff * materialColor
               + lights[i].Is * spec;
            }

            FragColor = vec4(color, 1.0);
        }
    )";

    shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);

    uWorld = glGetUniformLocation(shaderProgram, "uWorld");
    uView = glGetUniformLocation(shaderProgram, "uView");
    uProj = glGetUniformLocation(shaderProgram, "uProj");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

void GLRenderSystem::clearDisplay(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderSystem::setViewport(double x, double y, double width, double height)
{
    glViewport((int)x, (int)y, (int)width, (int)height);
}

void GLRenderSystem::renderTriangleSoup(const std::vector<Vertex>& vertices, const glm::vec3& matColor)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(uWorld, 1, GL_FALSE, &worldMatrix[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(uProj, 1, GL_FALSE, &projMatrix[0][0]);

    

    for (int i = 0; i < 8; ++i) {
        std::string idx = "lights[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(shaderProgram, (idx + ".position").c_str()), 1, &lights[i].position[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, (idx + ".Ia").c_str()), 1, &lights[i].Ia[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, (idx + ".Id").c_str()), 1, &lights[i].Id[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, (idx + ".Is").c_str()), 1, &lights[i].Is[0]);
        glUniform1i(glGetUniformLocation(shaderProgram, (idx + ".enabled").c_str()), lights[i].enabled);
        
    }

    glm::vec3 cameraPos = glm::vec3(glm::inverse(viewMatrix)[3]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &cameraPos[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "materialColor"), 1, &matColor[0]);
    
    
    switch (renderMode) {
        case RenderMode::Triangles:
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
            break;
        case RenderMode::Edges:
            for (size_t i = 0; i + 2 < vertices.size(); i += 3)
                glDrawArrays(GL_LINE_LOOP, static_cast<GLint>(i), 3);
            break;
        case RenderMode::Vertices:
            glPointSize(5.0f); 
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));
            break;
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void GLRenderSystem::setupLight(uint32_t index, glm::vec3 position, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is)
{
    if (index < 8) {
        lights[index].position = position;
        lights[index].Ia = Ia;
        lights[index].Id = Id;
        lights[index].Is = Is;
        lights[index].enabled = true;
    }
}

void GLRenderSystem::turnLight(uint32_t index, bool enable)
{
    if (index < 8) {
        lights[index].enabled = enable;
    }
}

void GLRenderSystem::setWorldMatrix(const glm::mat4& matrix)
{
    worldMatrix = matrix;
}

const glm::mat4& GLRenderSystem::getWorldMatrix() const
{
    return worldMatrix;
}

void GLRenderSystem::setViewMatrix(const glm::mat4& matrix)
{
    viewMatrix = matrix;
}

const glm::mat4& GLRenderSystem::getViewMatrix() const
{
    return viewMatrix;
}

void GLRenderSystem::setProjMatrix(const glm::mat4& matrix)
{
    projMatrix = matrix;
}

const glm::mat4& GLRenderSystem::getProjMatrix() const
{
    return projMatrix;
}



void GLRenderSystem::setRenderMode(RenderMode mode) { renderMode = mode; }
const RenderMode GLRenderSystem::getRenderMode() const { return renderMode; }

// Helper functions
GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Error: " << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader Linking Error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
