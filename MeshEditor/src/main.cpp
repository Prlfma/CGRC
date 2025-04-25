#include "../ThirdParty/glm/glm.hpp"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
#include "../ThirdParty/glm/gtx/transform.hpp"
#include "DynamicLibrary.h"
#include "Viewport.h"

#include "../../Interfaces/IWindow.h"
#include "../../Interfaces/IRenderSystem.h"
#include "HalfEdge.h"
#include "Mesh.h"
#include <iostream>
#include <vector>
#include <GL/gl.h>
#include "../../Parser/STLParser.h"
#include "../ThirdParty/glfw/include/GLFW/glfw3.h"
TriangleSoup triangleSoup;

bool looksLikePath(const std::string& str) {
    return str.find('/') != std::string::npos || str.find('\\') != std::string::npos;
}

void loadModelFromArgs(int argc, char* argv[]) {
    STLParser parser;

    if (argc > 1) {
        std::string input = argv[1];
        std::string path = looksLikePath(input) ? input : "../Assets/" + input;

        std::cout << "Attempting to load: " << path << std::endl;
        triangleSoup = parser.read(path);

        if (triangleSoup.empty()) {
            std::cerr << "Failed to load model: " << path << "\nUsing default cube instead.\n";
        }
    } else {
        std::cout << "No filename provided. Using default cube.\n";
    }

}



IRenderSystem* rs = nullptr;
Mesh* mesh;
Viewport viewport;
std::vector<VertexSimple> loadedModel;
glm::vec3 modelPosition = glm::vec3(0.0f);
glm::vec3 matColor = glm::vec3(0.0f, 0.0f, 0.0f); 
const float colorStep = 0.05f;


glm::vec3 toGlmVec(const Vec& v) {
    return glm::vec3(v.x, v.y, v.z);
}

void renderScene(IRenderSystem& rs)
{
    glm::mat4 modelMatrix = glm::translate(modelPosition);
    rs.setWorldMatrix(modelMatrix);
    rs.renderTriangleSoup(loadedModel, matColor);
}

void moveCube(glm::vec3 offset)
{
    mesh->moveMesh(offset);
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
FaceHandle face;
bool faceIsChosed;

void onKeyCallback(KeyCode key, Action action, Modifier mods)
{
    if (action == Action::Press || action == Action::Repeat) {
        if(!faceIsChosed){
            if (key == KeyCode::UP)
                moveCube( glm::vec3(0.0f, 0.2f, 0.0f));
            if (key == KeyCode::DOWN)
                moveCube(glm::vec3(0.0f, -0.2f, 0.0f));
            if (key == KeyCode::LEFT)
                moveCube(glm::vec3(-0.2f, 0.0f, 0.0f));
            if (key == KeyCode::RIGHT)
                moveCube(glm::vec3(0.2f, 0.0f, 0.0f));
        }
        else{
            if (key == KeyCode::UP)
            mesh->applyTransformation(face, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, 0.0f)));
            if (key == KeyCode::DOWN)
            mesh->applyTransformation(face, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)));
            if (key == KeyCode::LEFT)
            mesh->applyTransformation(face, glm::translate(glm::mat4(1.0f), glm::vec3(-0.2f, 0.0f, 0.0f)));
            if (key == KeyCode::RIGHT)
            mesh->applyTransformation(face, glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.0f, 0.0f)));
            if(key == KeyCode::D){
            mesh->deleteFace(face);
            }
        }

            if (key == KeyCode::R)  matColor.r += colorStep;
            if (key == KeyCode::T)  matColor.r -= colorStep;
    
            if (key == KeyCode::G)  matColor.g += colorStep;
            if (key == KeyCode::H)  matColor.g -= colorStep;
    
            if (key == KeyCode::B)  matColor.b += colorStep;
            if (key == KeyCode::N)  matColor.b -= colorStep;
    
            clampColor(matColor);

            if (key == KeyCode::NUM_1) rs->setRenderMode(RenderMode::Triangles);
            if (key == KeyCode::NUM_2) rs->setRenderMode(RenderMode::Edges);
            if (key == KeyCode::NUM_3) rs->setRenderMode(RenderMode::Vertices);
            if (key == KeyCode::F1)
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

HalfEdgeTable createCube()
{
    HalfEdgeTable table;

    // Додаємо 8 вершин куба
    glm::vec3 verts[8] = {
        {-1, -1, -1}, // 0
        { 1, -1, -1}, // 1
        { 1,  1, -1}, // 2
        {-1,  1, -1}, // 3
        {-1, -1,  1}, // 4
        { 1, -1,  1}, // 5
        { 1,  1,  1}, // 6
        {-1,  1,  1}  // 7
    };

    std::vector<VertexHandle> vhs;
    for (int i = 0; i < 8; ++i)
        vhs.push_back(table.addVertex(verts[i]));

    // Індекси вершин для кожної грані (по два трикутники на грань)
    int faceIndices[12][3] = {
        {0, 1, 2}, {0, 2, 3}, // Задня
        {1, 5, 6}, {1, 6, 2}, // Права
        {5, 4, 7}, {5, 7, 6}, // Передня
        {4, 0, 3}, {4, 3, 7}, // Ліва
        {3, 2, 6}, {3, 6, 7}, // Верхня
        {4, 5, 1}, {4, 1, 0}  // Нижня
    };

    for (int i = 0; i < 12; ++i)
    {
        table.addFace(
            vhs[faceIndices[i][0]],
            vhs[faceIndices[i][1]],
            vhs[faceIndices[i][2]]
        );
    }

    return table;
}


HalfEdgeTable createFromTriangleSoup(const TriangleSoup& soup)
{
    HalfEdgeTable table;

    if (soup.size() % 3 != 0) {
        throw std::runtime_error("TriangleSoup must contain a multiple of 3 vertices!");
    }

    // Зберігаємо вже додані вершини
    std::vector<VertexHandle> vertexHandles;
    vertexHandles.reserve(soup.size());

    // Додаємо вершини
    for (const auto& vertex : soup)
    {
        glm::vec3 pos(static_cast<float>(vertex.pos.x),
                      static_cast<float>(vertex.pos.y),
                      static_cast<float>(vertex.pos.z));
        vertexHandles.push_back(table.addVertex(pos));
    }

    // Додаємо грані
    for (size_t i = 0; i < soup.size(); i += 3)
    {
        table.addFace(
            vertexHandles[i],
            vertexHandles[i + 1],
            vertexHandles[i + 2]
        );
    }

    return table;
}

bool leftMousePressed = false;
bool rightMousePressed = false;
double lastX = 0.0, lastY = 0.0;

void onMouseButtonCallback(ButtonCode button, Action action, Modifier mods,double x, double y) {
    if (button == ButtonCode::Left) {
        leftMousePressed = (action == Action::Press);

        if (leftMousePressed && (mods == Modifier::Control || (static_cast<int>(mods) & static_cast<int>(Modifier::Control)))) {
            auto r = viewport.calcCursorRay(x,y);
            face = mesh->intersectRay(r);
            faceIsChosed = face.index > 0;
        }
    }
    if (button == ButtonCode::Right) {
        rightMousePressed = (action == Action::Press);
        if (rightMousePressed && (mods == Modifier::Control || (static_cast<int>(mods) & static_cast<int>(Modifier::Control))))
            faceIsChosed = false;
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
    DynamicLibrary dll("GLRenderSystem/libGLRenderSystem.dll");
    auto createRenderSystem = dll.getSymbol<IRenderSystem* (*)()>("createRenderSystem");
    auto createWindow = dll.getSymbol<IWindow* (*)(const std::string& title, 
    uint32_t width, uint32_t height)>("createWindow");
    auto waitEvents = dll.getSymbol<void(*)()>("waitEvents");
    auto swapDisplayBuffers = dll.getSymbol<void(*)(IWindow* window)>("swapDisplayBuffers");
    auto windowShouldClose = dll.getSymbol<bool(*)(IWindow* window)>("windowShouldClose");
    rs = createRenderSystem();
    IWindow* window = createWindow("myWindow", 1280, 960);

    window->setKeyCallback(onKeyCallback);
    window->setMouseCallback(onMouseButtonCallback);
    window->setCursorPosCallback(onMouseMoveCallback);
    window->setScrollCallback(onScrollCallback);
    viewport.setViewportSize(1280, 960);
    viewport.setFOV(60);
    viewport.setZNear(0.01);
    viewport.setZFar(500);
    

    rs->init();
    rs->setupLight(0, glm::vec3{0,10, 0},glm::vec3(0.2f), glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1});

    HalfEdgeTable halfEdgeTable ;
    loadModelFromArgs(argc,argv);
    auto soup = triangleSoup;
    if(soup.empty()){
        halfEdgeTable = createCube();
    }
    else{
        halfEdgeTable = createFromTriangleSoup(soup);
    }
    Mesh m(halfEdgeTable);
    mesh = &m;
    while (!windowShouldClose(window))
    {
        rs->setViewport(0, 0, window->getWidth(), window->getHeight());
        rs->clearDisplay(0.5f, 0.5f, 0.5f);
        rs->setViewMatrix(viewport.getCamera().calcViewMatrix());
        rs->setProjMatrix(viewport.calcProjectionMatrix());
        //renderScene(*rs);
        mesh->render(*rs,matColor);
        swapDisplayBuffers(window);
        waitEvents();      
    }
    delete window;
    delete rs;
    return 0;
}