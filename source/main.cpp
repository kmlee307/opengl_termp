/*Spaceship Game
2022-2 final project
P20190608 이경민
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <math.h>
#include <Model.h>
#include <string>
#include <vector>
#include <text.h>
#include <shader.h>
#include <arcball.h>
#include <Sphere.h>
#include <cube.h>
#include <skybox.h>
#include <random>
#include <Windows.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <keyframe.h>
#pragma comment(lib,"winmm.lib")

// Function Prototypes
GLFWwindow* glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void render();
void drawplanets();
void textrender();
void preyinit();
void blackholeinit();
void initKeyframes(float x, float y, float z);
void updateAnimData();
void render_anim(float x, float y, float z);
unsigned int loadTexture(const char*);


// Global variables
GLFWwindow* mainWindow = NULL;
Shader* sphereShader = NULL;
Shader* modelShader = NULL;
Shader* skyboxShader = NULL;
Shader* preycubeShader = NULL;
Shader* animShader = NULL;
Model* rocketModel = NULL;
Model* blackholeModel = NULL;
Model* AsteroidModel = NULL;
Model* StarModel = NULL;
Shader* textShader = NULL;
Text* text = NULL;

using namespace std;
unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 1000;

//prey 
Cube* cube;
glm::vec3 preySize(0.3f, .3f, .3f);
bool preinit = 1;
int eatenprey = 0;
struct Prey {
    bool eaten;
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
};
Prey prey[20];
float RandMax = 1.0f;
float RandMin = -1.0f;
//blackhole
struct BlackHole {
    float x, y, z;
};
BlackHole blackhole[5];
bool InTheHole = 0;
// for game
int Life = 10;
float Speed = 1;
#define MAX_SPEED 20
#define MIN_SPEED 0
void crushtest();
bool collisiontest(float x, float y);
bool Crushed = 0;

//별들의 크기와 위치 조정
float sunsize[3] = { 1.0f, 1.0f, 1.0f };
float sunpos[3] = { 0.0f, 0.0f, 0.0f };
float earthdistance[3] = { 3.0f, 0.0f, 0.0f };
Sphere* Planet;
SkyBox* skybox;


//크기: 0.4, 0.9, 1, 0.5, 11.2 , 9.4, 4, 3.9
//거리: 0.4, 0.6,1,  1.5, 5, 10, 20, 30
float InitialPlanetPos[8] = { 1.5f, 2.0f, 3.0f, 3.5f, 5.0f, 6.0f, 7.0f, 8.0f };
glm::vec3 sunSize(*sunsize);
glm::vec3 sunPos(*sunpos);
glm::vec3 mercurySize(0.73f, 0.73f, 0.73f);
glm::vec3 mercuryPos(InitialPlanetPos[0], 0.0f, 0.0f);
glm::vec3 venusSize(0.96f, 0.96f, 0.96f);
glm::vec3 venusPos(InitialPlanetPos[1], 0.0f, 0.0f);
glm::vec3 earthSize(*sunsize / pow(100.0, 1.0 / 3.0));
glm::vec3 earthPos(InitialPlanetPos[2], 0.0f, 0.0f);
glm::vec3 moonSize(0.2f, 0.2f, 0.2f);
glm::vec3 moonPos(2.4f, 0.0f, 0.0f);
glm::vec3 marsSize(0.5f, 0.5f, 0.5f);
glm::vec3 marsPos(InitialPlanetPos[3], 0.0f, 0.0f);
glm::vec3 jupyterSize(2.25f, 2.25f, 2.25f);
glm::vec3 jupyterPos(InitialPlanetPos[4], 0.0f, 0.0f);
glm::vec3 saturnSize(2.11f, 2.11f, 2.11f);
glm::vec3 saturnPos(InitialPlanetPos[5], 0.0f, 0.0f);
glm::vec3 uranusSize(1.58f, 1.58f, 1.58f);
glm::vec3 uranusPos(InitialPlanetPos[6], 0.0f, 0.0f);
glm::vec3 neptunSize(1.57f, 1.57f, 1.57f);
glm::vec3 neptunPos(InitialPlanetPos[7], 0.0f, 0.0f);

float CurrentMercuryPos[3] = { InitialPlanetPos[0], 0.0f, 0.0f };
float CurrentVenusPos[3] = { InitialPlanetPos[1], 0.0f, 0.0f };
float CurrentEarthPos[3] = { InitialPlanetPos[2], 0.0f, 0.0f };
float CurrentMarsPos[3] = { InitialPlanetPos[3], 0.0f, 0.0f };
float CurrentJupyterPos[3] = { InitialPlanetPos[4], 0.0f, 0.0f };
float CurrentSaturnPos[3] = { InitialPlanetPos[5], 0.0f, 0.0f };
float CurrentUranusPos[3] = { InitialPlanetPos[6], 0.0f, 0.0f };
float CurrentNeptunPos[3] = { InitialPlanetPos[7], 0.0f, 0.0f };

// for texture
static unsigned int texture_sun;
static unsigned int texture_mercury;
static unsigned int texture_venus;
static unsigned int texture_earth;
static unsigned int texture_moon;
static unsigned int texture_mars;
static unsigned int texture_jupyter;
static unsigned int texture_saturn;
static unsigned int texture_neptun;
static unsigned int texture_uranus;
static unsigned int texture_rocket;
static unsigned int texture_blakhole;
static unsigned int texture_background;

float* CurrentPlanetPos[9] = { sunpos, CurrentMercuryPos,CurrentVenusPos, CurrentEarthPos,CurrentMarsPos,CurrentJupyterPos, CurrentSaturnPos,CurrentUranusPos,CurrentNeptunPos };
float PlanetRadius[9] = { 1.0f, 0.01f,0.01f, 0.01f,0.02f,0.04f, 0.1f,0.2f,0.3f};

glm::mat4 projection, view, model, backgroundview, animview;
// for arcball
float arcballSpeed = 0.2f;
static Arcball camArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
static Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;

// for camera
glm::vec3 cameraPos(0.0f, 0.0f, 15.0f);
glm::vec3 ModelPos(-3.0f, -3.0f, 0.0f);
glm::vec3 ModelBackPoint = ModelPos - glm::vec3(0.0f, 2.0f, 0.0f);
glm::vec3 ModelDirVec = ModelPos - ModelBackPoint;

float ModelRotateCoefficientX = 0.0f;

// for rotation
float lastTime = 0.0f;
glm::vec3 axis(0.0f, 0.0f, 1.0f);
glm::vec3 earthaxis(earthPos);

// for lighting
glm::vec3 lightSize(0.1f, 0.1f, 0.1f);
glm::vec3 lightPos = ModelPos + glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 1.0f, 0.0f);
float ambientStrenth = 0.1f;
float specularStrength = 0.5f;
float specularPower = 64.0f;

// for animation
enum RenderMode { INIT, ANIM, STOP };
RenderMode renderMode;                  // current rendering mode
float beginT;                           // animation beginning time (in sec)
float timeT;                            // current time (in sec)
float animEndTime = 5.0f;               // ending time of animation (in sec)
float xTrans, yTrans, zTrans;           // current translation factors
float xAngle, yAngle, zAngle;           // current rotation factors
KeyFraming xTKF(4), yTKF(4), zTKF(4);   // translation keyframes
KeyFraming xRKF(4), yRKF(4), zRKF(4);   // rotation keyframes
float renderanim_x, renderanim_y, renderanim_z;
int main()
{
    mainWindow = glAllInit();

    //shader loading and compile (by calling the constructor)
    sphereShader = new Shader("shader/sphereShader.vert", "shader/sphereShader.frag");
    modelShader = new Shader("shader/modelLoading.vs", "shader/modelLoading.frag");
    skyboxShader = new Shader("shader/global.vs", "shader/global.fs");
    preycubeShader = new Shader("shader/lamp.vs", "shader/lamp.fs");
    textShader = new Shader("shader/text.vs", "shader/text.frag");
    animShader = new Shader("shader/basic_lighting.vs", "shader/basic_lighting.fs");
    text = new Text((char*)"fonts/arial.ttf", textShader, SCR_WIDTH, SCR_HEIGHT);

    rocketModel = new Model((GLchar*)"3d/Intergalactic_Spaceships_Version_2.obj");
    blackholeModel = new Model((GLchar*)"3d/blackhole.obj");
    AsteroidModel = new Model((GLchar*)"3d/Asteroid.obj");
    StarModel = new Model((GLchar*)"3d/star.obj");

    projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    skyboxShader->use();
    skyboxShader->setMat4("projection", projection);

    sphereShader->use();
    sphereShader->setMat4("projection", projection);

    texture_sun = loadTexture("image/sun.jpg");
    texture_mercury = loadTexture("image/mercury.jpg");
    texture_venus = loadTexture("image/venus.jpg");
    texture_earth = loadTexture("image/earth.bmp");
    texture_moon = loadTexture("image/moon.jpg");
    texture_mars = loadTexture("image/mars.jpg");
    texture_jupyter = loadTexture("image/jupyter.jpg");
    texture_saturn = loadTexture("image/saturn.jpg");
    texture_uranus = loadTexture("image/uranus.jpg");
    texture_neptun = loadTexture("image/neptune.jpg");
    texture_rocket = loadTexture("image/rocket_texture.jpg");
    texture_blakhole = loadTexture("image/ring_opa.jpg");
    texture_background = loadTexture("image/space.jpg");

    modelShader->use();
    modelShader->setMat4("projection", projection);

    Planet = new Sphere();

    skybox = new SkyBox();

    cube = new Cube();
    preycubeShader->use();
    preycubeShader->setMat4("projection", projection);

    preyinit();
    blackholeinit();

    axis = glm::normalize(axis);


    // lighting parameters to fragment shader
    animShader->use();
    animShader->setVec3("objectColor", objectColor);
    animShader->setVec3("lightColor", lightColor);
    animShader->setVec3("lightPos", lightPos);
    animShader->setFloat("ambientStrenth", ambientStrenth);

    // projection matrix
    projection = glm::perspective(glm::radians(45.0f),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    animShader->setMat4("projection", projection);

    // cube initialization
    cube = new Cube();

    // initialize animation data
    initKeyframes(-4.0f, 10.0f, 10.0f);
    timeT = 0.0f;
    updateAnimData();
    renderMode = INIT;



    cout << "ARCBALL: camera rotation mode" << endl;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(mainWindow)) {
        render();
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

GLFWwindow* glAllInit()
{
    GLFWwindow* window;

    // glfw: initialize and configure
    if (!glfwInit()) {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SpaceShip Game", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // OpenGL states
    glEnable(GL_DEPTH_TEST);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    PlaySound("sound/spacesound.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);

    return window;

}

unsigned int loadTexture(const char* texFileName) {
    unsigned int texture;

    // Create texture ids.
    glGenTextures(1, &texture);

    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters for wrapping.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);   // vertical flip the texture
    unsigned char* image = stbi_load(texFileName, &width, &height, &nrChannels, 0);
    if (!image) {
        printf("texture %s loading error ... \n", texFileName);
    }
    else printf("texture %s loaded\n", texFileName);

    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

unsigned int loadCubemap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(3, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            cout << faces[i].c_str() << endl;
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void render() {

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;  // in seconds (in glfw)
    float angleStepDeg = (float)30.0f * deltaTime;
    float angleStepRad = glm::radians(angleStepDeg);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //camerapos replace
    cameraPos = ModelPos + glm::vec3(0.0f, -2.0f, 0.0f);
    ModelDirVec = ModelPos - ModelBackPoint;
    lightPos = ModelPos;
    view = glm::lookAt(ModelBackPoint, ModelPos, glm::vec3(0.0f, 0.0f, .1f));
    backgroundview = glm::lookAt(glm::vec3(0.0f, -20.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glDepthMask(GL_FALSE);
    skyboxShader->use();
    skyboxShader->setMat4("view", backgroundview);
    glBindTexture(GL_TEXTURE_2D, texture_background);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 00.0f, 0.0f));
    model = glm::scale(model, glm::vec3(20.0f, 20.0f,20.0f));
    skyboxShader->setMat4("model", model);
    cube->draw(skyboxShader);
    glDepthMask(GL_TRUE);

    textrender();
    drawplanets();

    preycubeShader->use();
    preycubeShader->setMat4("view", view);
    for (int i = 0; i < 20; i++)
    {
        // prey 20 개 무작위 위치에 무작위 색깔로 생성
        if (prey[i].eaten == 0) {
            preycubeShader->setVec3("toColor", glm::vec3(prey[i].r, prey[i].g, prey[i].b));
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(prey[i].x, prey[i].y, prey[i].z));
            model = glm::scale(model, preySize);
            preycubeShader->setMat4("model", model);
            cube->draw(preycubeShader);
        }
    }

    modelShader->use();
    modelShader->setMat4("view", view);
    glBindTexture(GL_TEXTURE_2D, texture_blakhole);
    for (int i = 0; i <5; i++)
    {
        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(blackhole[i].x, blackhole[i].y, blackhole[i].z));
        model = glm::rotate(model, angleStepRad * (1.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(.01f, .01f, .01f));
        modelShader->setMat4("model", model);
        blackholeModel->Draw(modelShader);
    }

    // Draw the loaded model
    modelShader->use();
    modelShader->setMat4("view", view);
    glBindTexture(GL_TEXTURE_2D, texture_rocket);
    glm::mat4 model(1.0);
    model = glm::translate(model, ModelPos);
    model = glm::rotate(model, -1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, ModelRotateCoefficientX, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
    modelShader->setMat4("model", model);
    rocketModel->Draw(modelShader);

    crushtest();
    
    if (renderMode == INIT) {
        renderanim_x = ((float)rand() / (RAND_MAX)-1.0) * 20;
        renderanim_y = ((float)rand() / (RAND_MAX)+0.5) * 10;
        renderanim_z = 50.0f;
        renderMode = ANIM;
    }

    render_anim(renderanim_x, renderanim_y, renderanim_z);

    glfwSwapBuffers(mainWindow);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cube->deletebuffer();
        Planet->deletebuffer();
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
    }
    if (key == GLFW_KEY_UP) {
        if (!collisiontest(0.0f, 0.1f)) {
            ModelPos[1] += 0.1 * Speed;
            ModelBackPoint[1] += 0.1 * Speed;
        }
        if (ModelPos[1] > 10) ModelPos[1] = 10;
    }
    if (key == GLFW_KEY_DOWN) {
        if (!collisiontest(0.0f, -0.1f)) {
            ModelPos[1] -= 0.1 * Speed;
            ModelBackPoint[1] -= 0.1 * Speed;
        }

        if (ModelPos[1] < -10) ModelPos[1] = -10;
    }
    if (key == GLFW_KEY_LEFT) {
        if (!collisiontest(-0.1f, 0.0f)) {
            ModelPos[0] -= 0.1 * Speed;
            ModelBackPoint[0] -= 0.1 * Speed;
        }

        if (ModelPos[0] < -10) ModelPos[0] = -10;
  
    }if (key == GLFW_KEY_RIGHT) {
        if (!collisiontest(0.1f, 0.0f)){
            ModelPos[0] += 0.1 * Speed;
            ModelBackPoint[0] += 0.1 * Speed;
        }
        if (ModelPos[0] > 10) ModelPos[0] = 10;
    }
    if (key == GLFW_KEY_W) {
        if (!collisiontest(0.0f, 0.0f)) {
            ModelPos[2] += 0.1 * Speed;
            ModelBackPoint[2] += 0.1 * Speed;
        }
        if (ModelPos[2] > 10) ModelPos[2] = 10;
    }
    if (key == GLFW_KEY_S) {
        if (!collisiontest(0.0f, 0.0f)){
            ModelPos[2] -= 0.1 * Speed;
            ModelBackPoint[2] -= 0.1 * Speed;
        }
        if (ModelPos[2] < -10) ModelPos[2] = -10;
    }
    if (key == GLFW_KEY_A) {
        double pi = 3.14159;
        ModelRotateCoefficientX += 0.1f;
        ModelDirVec = ModelBackPoint - ModelPos;
        float x, y, lenth;
        lenth = sqrt(pow(ModelDirVec[0], 2) + pow(ModelDirVec[1], 2)) * sin(0.1/2.0);
        x = lenth * cos(0.1 / 2.0);
        y = lenth * sin(0.1 / 2.0);
        ModelBackPoint[0] -= x*2;
        ModelBackPoint[1] += y*2;
    }
    if (key == GLFW_KEY_D) {
        ModelRotateCoefficientX -= 0.1f;
        ModelDirVec = ModelBackPoint - ModelPos;
        float x, y, lenth;
        lenth = sqrt(pow(ModelDirVec[0], 2) + pow(ModelDirVec[1], 2)) * sin(0.1 / 2.0);
        x = lenth * cos(0.1 / 2.0);
        y = lenth * sin(0.1 / 2.0);
        ModelBackPoint[0] += x * 2;
        ModelBackPoint[1] += y * 2;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (renderMode == INIT) {
            renderMode = ANIM;
            beginT = glfwGetTime();
        }
        else if (renderMode == STOP) {
            if (timeT == animEndTime) renderMode = INIT;
            else {
                renderMode = ANIM;
            }
        }
        else if (renderMode == ANIM) renderMode = STOP;

        if (renderMode == INIT) {
            timeT = 0.0;
            updateAnimData();
        }
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        ModelPos[0] += ModelDirVec[0] * Speed;
        ModelPos[1] += ModelDirVec[1] * Speed;
        ModelPos[2] += ModelDirVec[2] * Speed;
        if (ModelPos[0] > 10) ModelPos[0] = 10;
        if (ModelPos[1] > 10) ModelPos[1] = 10;
        if (ModelPos[2] > 10) ModelPos[2] = 10;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        ModelPos[0] -= ModelDirVec[0] * Speed;
        ModelPos[1] -= ModelDirVec[1] * Speed;
        ModelPos[2] -= ModelDirVec[2] * Speed;
        if (ModelPos[0] < -10) ModelPos[0] = -10;
        if (ModelPos[1] < -10) ModelPos[1] = -10;
        if (ModelPos[2] < -10) ModelPos[2] = -10;

    }
}

void cursor_position_callback(GLFWwindow* window, double x, double y) {
    if (arcballCamRot)
        camArcBall.cursorCallback(window, x, y);
    else
        modelArcBall.cursorCallback(window, x, y);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Speed += yoffset * 0.1;
    if (Speed < 1) Speed = 1;
    if (Speed > 5) Speed = 5;
}
void drawplanets() {

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;  // in seconds (in glfw)
    float angleStepDeg = (float)30.0f * deltaTime;
    float angleStepRad = glm::radians(angleStepDeg);

    sphereShader->use();
    sphereShader->setMat4("view", view);
    sphereShader->setVec3("light.position", lightPos);
    sphereShader->setVec3("viewPos", cameraPos);
    sphereShader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
    sphereShader->setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
    sphereShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    sphereShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    sphereShader->setFloat("material.shininess", 128.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_sun);
    model = glm::mat4(1.0f);
    model = glm::translate(model, sunPos);
    model = glm::scale(model, sunSize);
    model = glm::rotate(model, angleStepRad * (0.3f), axis);
    model = model * modelArcBall.createRotationMatrix();
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);

    glBindTexture(GL_TEXTURE_2D, texture_mercury);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad * (1.5f), axis);
    model = glm::translate(model, mercuryPos);
    model = glm::rotate(model, angleStepRad * (1.5f), axis);
    model = glm::scale(model, mercurySize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentMercuryPos[0] = InitialPlanetPos[0] * cos(angleStepRad * (1.5f));
    CurrentMercuryPos[1] = InitialPlanetPos[0] * sin(angleStepRad * (1.5f));

    glBindTexture(GL_TEXTURE_2D, texture_venus);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad * (1.1f), axis);
    model = glm::translate(model, venusPos);
    model = glm::rotate(model, angleStepRad * (1.1f), axis);
    model = glm::scale(model, venusSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentVenusPos[0] = InitialPlanetPos[1] * cos(angleStepRad * (1.1f));
    CurrentVenusPos[1] = InitialPlanetPos[1] * sin(angleStepRad * (1.1f));

    glBindTexture(GL_TEXTURE_2D, texture_earth);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad, axis);
    model = glm::rotate(model, -10.f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, earthPos);
    model = glm::rotate(model, angleStepRad, axis);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentEarthPos[0] = InitialPlanetPos[2] * cos(angleStepRad);
    CurrentEarthPos[1] = InitialPlanetPos[2] * sin(angleStepRad);

    glBindTexture(GL_TEXTURE_2D, texture_moon);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad, axis);
    model = glm::rotate(model, -10.f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, angleStepRad * (0.1f), earthaxis);
    model = glm::translate(model, moonPos);
    model = glm::rotate(model, angleStepRad, axis);
    model = glm::scale(model, moonSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);


    glBindTexture(GL_TEXTURE_2D, texture_mars);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad * (0.8f), axis);
    model = glm::rotate(model, 15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, marsPos);
    model = glm::rotate(model, angleStepRad * (0.8f), axis);
    model = glm::scale(model, marsSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentMarsPos[0] = InitialPlanetPos[3] * cos(angleStepRad * (.8f));
    CurrentMarsPos[1] = InitialPlanetPos[3] * sin(angleStepRad * (.8f));

    glBindTexture(GL_TEXTURE_2D, texture_jupyter);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad * (0.4f), axis);
    model = glm::rotate(model, 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, jupyterPos);
    model = glm::rotate(model, angleStepRad * (0.4f), axis);
    model = glm::scale(model, jupyterSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader); glBindTexture(GL_TEXTURE_2D, texture_saturn);
    CurrentJupyterPos[0] = InitialPlanetPos[4] * cos(angleStepRad * (.4f));
    CurrentJupyterPos[1] = InitialPlanetPos[4] * sin(angleStepRad * (.4f));

    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad * (0.3f), axis);
    model = glm::rotate(model, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, saturnPos);
    model = glm::rotate(model, angleStepRad * (0.3f), axis);
    model = glm::scale(model, saturnSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentSaturnPos[0] = InitialPlanetPos[5] * cos(angleStepRad * (.3f));
    CurrentSaturnPos[1] = InitialPlanetPos[5] * sin(angleStepRad * (.3f));

    glBindTexture(GL_TEXTURE_2D, texture_uranus);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, angleStepRad * (0.2f), axis);
    model = glm::rotate(model, 15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, uranusPos);
    model = glm::rotate(model, angleStepRad * (0.2f), axis);
    model = glm::scale(model, uranusSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentUranusPos[0] = InitialPlanetPos[6] * cos(angleStepRad * (.2f));
    CurrentUranusPos[1] = InitialPlanetPos[6] * sin(angleStepRad * (.2f));

    glBindTexture(GL_TEXTURE_2D, texture_neptun);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, -15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, angleStepRad * (0.15f), axis);
    model = glm::translate(model, neptunPos);
    model = glm::rotate(model, angleStepRad * (0.15f), axis);
    model = glm::scale(model, neptunSize);
    model = glm::scale(model, earthSize);
    sphereShader->setMat4("model", model);
    Planet->draw(sphereShader);
    CurrentNeptunPos[0] = InitialPlanetPos[7] * cos(angleStepRad * (.15f));
    CurrentNeptunPos[1] = InitialPlanetPos[7] * sin(angleStepRad * (.15f));

}

void textrender() {
    string position = "Current Position: ";
    float positionz = 0.0f;
    text->RenderText("------------------------------------------------------------", 670.0f, 980.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Current Speed: " + to_string(Speed), 670.0f, 960.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText(position + to_string(ModelPos[0]) + ", " + to_string(ModelPos[1]) + ", " + to_string(ModelPos[2]), 670.0f, 930.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Eaten Pray : " + to_string(eatenprey) + " / 20", 670.0f, 900.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Life: " + to_string(Life), 670.0f, 870.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("------------------------------------------------------------", 670.0f, 850.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Sunpos: " + to_string(sunpos[0]) + ", " + to_string(sunpos[1]) + ", " + to_string(sunpos[2]), 670.0f, 830.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Mercurypos: " + to_string(CurrentMercuryPos[0]) + ", " + to_string(CurrentMercuryPos[1]) + ", " + to_string(positionz), 670.0f, 810.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Venuspos: " + to_string(CurrentVenusPos[0]) + ", " + to_string(CurrentVenusPos[1]) + ", " + to_string(positionz), 670.0f, 790.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Earthpos: " + to_string(CurrentEarthPos[0]) + ", " + to_string(CurrentEarthPos[1]) + ", " + to_string(positionz), 670.0f, 770.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Marspos: " + to_string(CurrentMarsPos[0]) + ", " + to_string(CurrentMarsPos[1]) + ", " + to_string(positionz), 670.0f, 750.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Jupyterpos: " + to_string(CurrentJupyterPos[0]) + ", " + to_string(CurrentJupyterPos[1]) + ", " + to_string(positionz), 670.0f, 730.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Saturnpos: " + to_string(CurrentSaturnPos[0]) + ", " + to_string(CurrentSaturnPos[1]) + ", " + to_string(positionz), 670.0f, 710.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Uranuspos: " + to_string(CurrentUranusPos[0]) + ", " + to_string(CurrentUranusPos[1]) + ", " + to_string(positionz), 670.0f, 690.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("Neptunpos: " + to_string(CurrentNeptunPos[0]) + ", " + to_string(CurrentNeptunPos[1]) + ", " + to_string(positionz), 670.0f, 670.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText("------------------------------------------------------------", 670.0f, 650.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
    text->RenderText(to_string(Crushed) + "-------------------------------" + to_string(InTheHole), 670.0f, 630.0f, 0.3f, glm::vec3(0.0f, 0.7f, 0.0f));
}

void preyinit() {

    for (int i = 0; i < 20; i++) {
        prey[i].eaten = 0;
        prey[i].x = ((float)rand() / (RAND_MAX)-0.5) * 20;
        prey[i].y = ((float)rand() / (RAND_MAX)-0.5) * 20;
        prey[i].z = ((float)rand() / (RAND_MAX)-0.5) * 20;
        prey[i].r = ((float)rand() / (RAND_MAX));
        prey[i].g = ((float)rand() / (RAND_MAX));
        prey[i].b = ((float)rand() / (RAND_MAX));
    }
}

void crushtest() {
    float distance;
    for (int i = 0; i < 20; i++) {
        distance = (float)sqrt(pow(ModelPos[0] - prey[i].x, 2) + pow(ModelPos[1] - prey[i].y, 2));
        if (prey[i].eaten == 0 && distance < 0.5) {
            prey[i].eaten = 1;
            eatenprey += 1;
            PlaySound("sound/eatingsound.wav", 0, SND_FILENAME | SND_ASYNC);
        }
    }
    for (int i = 0; i < 5; i++) {
        distance = (float)sqrt(pow(ModelPos[0] - blackhole[i].x, 2) + pow(ModelPos[1] - blackhole[i].y, 2));
        if (InTheHole == 0 && distance < 0.5) {
            InTheHole = 1;
            if (Life - 1 > 0) Life -= 1;
            PlaySound("sound/beepsound.wav", 0, SND_FILENAME | SND_ASYNC);
        }
        //else if (distance > 1) InTheHole =0;
    }
    for (int i = 0; i < 9; i++) {
        distance = sqrt(pow(ModelPos[0] - CurrentPlanetPos[i][0], 2) + pow(ModelPos[1] - CurrentPlanetPos[i][1], 2) + pow(ModelPos[2] - CurrentPlanetPos[i][2], 2));
        if (Crushed == 0 && distance < 0.5) {
            Crushed = 1;
            if (Life - 1 >= 0) Life -= 1;

            PlaySound("sound/beepsound.wav", 0, SND_FILENAME | SND_ASYNC);
        }
        if (Crushed == 1 && distance > 10) {
            Crushed = 0;
            //cout << "hierror" << endl;
        }
    }
}

void blackholeinit() {
    for (int i = 0; i < 5; i++) {
        blackhole[i].x = ((float)rand() / (RAND_MAX)-0.5) * 20;
        blackhole[i].y = ((float)rand() / (RAND_MAX)-0.5) * 20;
        blackhole[i].z = ((float)rand() / (RAND_MAX)-0.5) * 20;
    }
}

bool collisiontest(float x, float y) {
    float distance;
    bool collision=0;
    for (int i = 0; i < 9; i++) {
        distance = sqrt(pow(ModelPos[0]+x - CurrentPlanetPos[i][0], 2) + pow(ModelPos[1]+y - CurrentPlanetPos[i][1], 2) + pow(ModelPos[2] - CurrentPlanetPos[i][2], 2));
        if (distance < PlanetRadius[i])
            collision = 1;
    }
    return collision;
}

void initKeyframes(float renderanim_x, float renderanim_y, float renderanim_z) {

    // x-translation keyframes
    xTKF.setKey(0, 0, renderanim_x-5.0);
    xTKF.setKey(1, 1.5, renderanim_x+0.0);
    xTKF.setKey(2, 3.0, renderanim_x+5.0);
    xTKF.setKey(3, animEndTime, renderanim_x+9.0);

    // y-translation keyframes
    yTKF.setKey(0, 0, renderanim_y+10.0);
    yTKF.setKey(1, 1.5, renderanim_y+20.0f);
    yTKF.setKey(2, 3.0, renderanim_y+30.0f);
    yTKF.setKey(3, animEndTime, renderanim_y+40.0);

    // z-translation keyframes
    zTKF.setKey(0, 0, renderanim_z);
    zTKF.setKey(1, 1.5, renderanim_z-10.0);
    zTKF.setKey(2, 3.0, renderanim_z-20.0);
    zTKF.setKey(3, animEndTime, renderanim_z-30.0);

    // x-rotation keyframes
    xRKF.setKey(0, 0, 0.0);
    xRKF.setKey(1, 1.5, 20.0);
    xRKF.setKey(2, 3.0, 80.0);
    xRKF.setKey(3, animEndTime, 0.0);

    // y-rotation keyframes
    yRKF.setKey(0, 0, 0.0);
    yRKF.setKey(1, 1.5, -30.0);
    yRKF.setKey(2, 3.0, 50.0);
    yRKF.setKey(3, animEndTime, 0.0);

    // z-rotation keyframes
    zRKF.setKey(0, 0, 0.0);
    zRKF.setKey(1, 1.5, 90.0);
    zRKF.setKey(2, 3.0, 180.0);
    zRKF.setKey(3, animEndTime, 200.0);
}

void updateAnimData() {
    if (timeT > animEndTime) {
        renderMode = STOP;
        timeT = animEndTime;
    }
    xTrans = xTKF.getValLinear(timeT);
    yTrans = yTKF.getValLinear(timeT);
    zTrans = zTKF.getValLinear(timeT);
    xAngle = xRKF.getValLinear(timeT);
    yAngle = yRKF.getValLinear(timeT);
    zAngle = zRKF.getValLinear(timeT);
}

void render_anim(float x, float y, float z) {
    glm::vec3 cameraAt(0.0f, 0.0f, 0.0f);

    animview = view * camArcBall.createRotationMatrix();
    if (renderMode == ANIM) {
        float cTime = (float)glfwGetTime(); // current time
        timeT = cTime - beginT;
        updateAnimData();
    }
    if (renderMode == INIT) {
        initKeyframes(x, y, z);
    }
    if (renderMode == STOP) {
        renderMode = INIT;
    }

    // view matrix to fragment shader
    animShader->use();
    animShader->setMat4("view", animview);
    // cube object
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(xTrans, yTrans, zTrans));
    glm::vec3 eulerAngles(glm::radians(xAngle), glm::radians(yAngle), glm::radians(zAngle));
    glm::quat q(eulerAngles);
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 rotMatrix = q.operator glm::mat4x4();
    model = model * rotMatrix;
    animShader->setMat4("model", model);
    StarModel->Draw(animShader);
}