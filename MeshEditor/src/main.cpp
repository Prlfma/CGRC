#include "../ThirdParty/glm/glm.hpp"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
#include "../ThirdParty/glm/gtx/transform.hpp"
#include "GLWindow.h"
#include "GLRenderSystem.h"

#include <iostream>
#include <vector>
#include <GL/gl.h>
#include "../../Parser/STLParser.h"
TriangleSoup triangleSoup;

bool looksLikePath(const std::string& str) {
    return str.find('/') != std::string::npos || str.find('\\') != std::string::npos;
}

void loadModelFromArgs(int argc, char* argv[]) {
    STLParser parser;

    if (argc > 1) {
        std::string input = argv[1];
        std::string path = looksLikePath(input) ? input : "../../Assets/" + input;

        std::cout << "Attempting to load: " << path << std::endl;
        triangleSoup = parser.read(path);

        if (triangleSoup.empty()) {
            std::cerr << "Failed to load model: " << path << "\nUsing default cube instead.\n";
        }
    } else {
        std::cout << "No filename provided. Using default cube.\n";
    }

    if (triangleSoup.empty()) {
        triangleSoup = {
            {{-1, 0, -1}, {0, -1, 0}}, {{1, 0, -1}, {0, -1, 0}}, {{1, 0, 1}, {0, -1, 0}},
            {{1, 0, 1}, {0, -1, 0}}, {{-1, 0, 1}, {0, -1, 0}}, {{-1, 0, -1}, {0, -1, 0}},
        
            {{-1, 0, -1}, {0, 0.707f, -0.707f}}, {{1, 0, -1}, {0, 0.707f, -0.707f}}, {{0, 1.5, 0}, {0, 0.707f, -0.707f}},
        
            {{1, 0, -1}, {0.707f, 0.707f, 0}}, {{1, 0, 1}, {0.707f, 0.707f, 0}}, {{0, 1.5, 0}, {0.707f, 0.707f, 0}},
        
            {{1, 0, 1}, {0, 0.707f, 0.707f}}, {{-1, 0, 1}, {0, 0.707f, 0.707f}}, {{0, 1.5, 0}, {0, 0.707f, 0.707f}},
        
            {{-1, 0, 1}, {-0.707f, 0.707f, 0}}, {{-1, 0, -1}, {-0.707f, 0.707f, 0}}, {{0, 1.5, 0}, {-0.707f, 0.707f, 0}}
        };
    }
}


GLRenderSystem rs;
std::vector<Vertex> loadedModel;
glm::vec3 modelPosition = glm::vec3(0.0f);
glm::vec3 matColor = glm::vec3(0.0f, 0.0f, 0.0f); 
const float colorStep = 0.05f;


glm::vec3 toGlmVec(const Vec& v) {
    return glm::vec3(v.x, v.y, v.z);
}

void renderScene(GLRenderSystem& rs)
{
    glm::mat4 modelMatrix = glm::translate(modelPosition);
    rs.setWorldMatrix(modelMatrix);
    rs.renderTriangleSoup(loadedModel, matColor);
}

void moveCube(GLRenderSystem& rs, glm::vec3 offset)
{
    modelPosition += offset;
}

void clampColor(glm::vec3& color) {
    color.r = glm::clamp(color.r, 0.0f, 1.0f);
    color.g = glm::clamp(color.g, 0.0f, 1.0f);
    color.b = glm::clamp(color.b, 0.0f, 1.0f);
}
void onKeyCallback(KeyCode key, Action action, Modifier mods)
{
    if (action == Action::Press || action == Action::Repeat) {
        if (key == KeyCode::UP)
            moveCube(rs, glm::vec3(0.0f, 0.2f, 0.0f));
        if (key == KeyCode::DOWN)
            moveCube(rs, glm::vec3(0.0f, -0.2f, 0.0f));
        if (key == KeyCode::LEFT)
            moveCube(rs, glm::vec3(-0.2f, 0.0f, 0.0f));
        if (key == KeyCode::RIGHT)
            moveCube(rs, glm::vec3(0.2f, 0.0f, 0.0f));

            if (key == KeyCode::R)  matColor.r += colorStep;
            if (key == KeyCode::T)  matColor.r -= colorStep;
    
            if (key == KeyCode::G)  matColor.g += colorStep;
            if (key == KeyCode::H)  matColor.g -= colorStep;
    
            if (key == KeyCode::B)  matColor.b += colorStep;
            if (key == KeyCode::N)  matColor.b -= colorStep;
    
            clampColor(matColor);

            if (key == KeyCode::NUM_1) rs.setRenderMode(RenderMode::Triangles);
            if (key == KeyCode::NUM_2) rs.setRenderMode(RenderMode::Edges);
            if (key == KeyCode::NUM_3) rs.setRenderMode(RenderMode::Vertices);
    }
}

int main(int argc, char *argv[])
{
    glfwInit();
    GLWindow window("myWindow", 1280, 960);
    window.setKeyCallback(onKeyCallback);

    rs.init();
    rs.setupLight(0, glm::vec3{0, 5, 0},glm::vec3(0.2f), glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1});
    

    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    rs.setViewMatrix(viewMatrix);

    glm::mat4 projMatrix = glm::perspective(glm::radians(60.0f), 640.0f / 480.0f, 0.1f, 500.f);
    rs.setProjMatrix(projMatrix);
    loadModelFromArgs(argc,argv);
    auto soup = triangleSoup;
    for (const auto& v : soup) {
        loadedModel.push_back(Vertex{
            toGlmVec(v.pos),
            toGlmVec(v.normal)
        });
    }

    while (!glfwWindowShouldClose(window.getGLFWHandle()))
    {
        rs.setViewport(0, 0, window.getWidth(), window.getHeight());
        rs.clearDisplay(0.5f, 0.5f, 0.5f);

        renderScene(rs);

        glfwSwapBuffers(window.getGLFWHandle());
        glfwWaitEvents();
    }

    return 0;
}