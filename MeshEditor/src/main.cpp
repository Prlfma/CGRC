#include "../ThirdParty/glm/glm.hpp"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
#include "../ThirdParty/glm/gtx/transform.hpp"
#include "GLWindow.h"
#include "GLRenderSystem.h"
#include "Viewport.h"

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
Viewport viewport;
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

void moveCamera(Camera& camera, glm::vec3 offset) {
     glm::vec3 eye = camera.getEye();
     glm::vec3 target = camera.getTarget();
     glm::vec3 up = camera.getUp(); 
     
     glm::vec3 forward = glm::normalize(target - eye);
     glm::vec3 right = glm::normalize(glm::cross(forward, up));
     up = glm::normalize(glm::cross(right, forward)); 
     
     glm::vec3 worldOffset = offset.x * right + offset.y * up + offset.z * forward;
     
     camera.setEyeTargetUp(eye + worldOffset, target + worldOffset, up);
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
            else if (key == KeyCode::F1)
                viewport.getCamera().setFrontView();
            else if (key == KeyCode::F2)
                viewport.getCamera().setTopView();
            else if (key == KeyCode::F3)
                viewport.getCamera().setRearView();
            else if (key == KeyCode::F4)
                viewport.getCamera().setRightView();
            else if (key == KeyCode::F5)
                viewport.getCamera().setLeftView();
            else if (key == KeyCode::F6)
                viewport.getCamera().setBottomView();
            else if (key == KeyCode::F7)
                viewport.getCamera().setIsoView();
            else if (key == KeyCode::F8)
                viewport.setParallelProjection(!viewport.isParallelProjection());
    }
}

bool leftMousePressed = false;
bool rightMousePressed = false;
double lastX = 0.0, lastY = 0.0;

void onMouseButtonCallback(ButtonCode button, Action action, Modifier mods,double x, double y) {
    if (button == ButtonCode::Left) {
        leftMousePressed = (action == Action::Press);
    }
    if (button == ButtonCode::Right) {
        rightMousePressed = (action == Action::Press);
    }
}

void onMouseMoveCallback(double xpos, double ypos) {
    double dx = xpos - lastX;
    double dy = ypos - lastY;

    if (leftMousePressed) {
        Camera& camera = viewport.getCamera();
        const float sensitivity = 0.005f;
        
        glm::vec3 lastPos(lastX, lastY, 0);
        glm::vec3 currentPos(xpos, ypos, 0);
        
        camera.orbit(lastPos, currentPos);
    } else if (rightMousePressed) {
        viewport.getCamera().pan(-dx * 0.01, dy * 0.01); 
    }

    lastX = xpos;
    lastY = ypos;
}

void onScrollCallback(double xoffset, double yoffset) {
    viewport.getCamera().zoom(yoffset * 0.5); 
}

int main(int argc, char *argv[])
{
    glfwInit();
    GLWindow window("myWindow", 1280, 960);
    window.setKeyCallback(onKeyCallback);
    window.setMouseCallback(onMouseButtonCallback);
    window.setCursorPosCallback(onMouseMoveCallback);
    window.setScrollCallback(onScrollCallback);
    viewport.setViewportSize(1280, 960);
    viewport.setFOV(60);
    viewport.setZNear(0.01);
    viewport.setZFar(500);
    

    rs.init();
    rs.setupLight(0, glm::vec3{0,10, 0},glm::vec3(0.2f), glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1});

    
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
        rs.setViewMatrix(viewport.getCamera().calcViewMatrix());
        rs.setProjMatrix(viewport.calcProjectionMatrix());
        renderScene(rs);
        glfwSwapBuffers(window.getGLFWHandle());
        glfwWaitEvents();      
    }

    return 0;
}