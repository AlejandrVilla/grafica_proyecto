#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_.h"
#include "camera_.h"
#include "laberinto.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#define vi std::vector<int>
#define vvi std::vector<vi>
#define spii std::set<std::pair<int,int>>

// window
GLFWwindow* window;
int iniWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 900;

// camera
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 pos_camera = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 front_camera = glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 up2 = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 pos_camera2 = glm::vec3(0.0f, 25.0f, 0.0f);
glm::vec3 front_camera2 = glm::vec3(0.0f, -1.0f, 0.0f);

Camera camera1(pos_camera);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = 1;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// game
int rows, columns;
vvi laberinto_matrix;
spii puntosComida;
spii puntosFruta;
spii puntosFantasma;
void iniWall();
void renderLab(Shader& wall, Shader& sphere, Shader& cuerpoFantasma, Shader& cabezaFantmas);
void drawWall(Shader& wall);
void iniSphere();
void drawSphere(Shader& sphere);
void iniPacman();
void drawPacman(Shader& pacman);
void game(Shader& wall, Shader& sphere, Shader& pacman, Shader& cuerpoFantasma, Shader& cabezaFantasma);
bool findComida(float x, float y);
bool findFruta(float x, float y);
bool findFantasma(float x, float y);
void iniFantasma();
void drawFantasma(Shader& cuerpoFantasma, Shader& cabezaFantasma);

//glm::mat4 model_wall = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 transf = glm::mat4(1.0f);

// wall
float wall_width = 0.50;
float wall_heigth = 0.75;
float wall_length = 0.50;
float x_wall, y_wall;

// sphere
float pi = 3.1416f;
float incrRad = 180 / 5;
float radio = 0.125;
float tetha, phi;
float vertices[36];

unsigned int VBO[1], VAO[1];

std::vector<glm::vec4> coloresFruta = {
    glm::vec4(1.0f,1.0f,0.0f,0.1f),
    glm::vec4(0.0f,1.0f,1.0f,1.0f),
    glm::vec4(1.0f,0.0f,1.0f,1.0f),
    glm::vec4(1.0f,1.0f,1.0f,1.0f)
};

// Fantasma
float tmp = 0.125;
float radioFantasma = tmp;
float fantasma_width = tmp;
float fantasma_heigth = tmp;
float fantasma_length = tmp;
// desplazamiento
int contMov = 0;
int tiempoMov = 100;
int i_fantasma = 0;
int i_fruta = 0;
bool primeraPosFantasma = 1;
std::vector<glm::vec2> posicionesFantasma;
std::vector<glm::vec4> coloresFantasmas = {
    glm::vec4(1.0f,0.0f,0.0f,0.1f), 
    glm::vec4(0.0f,1.0f,0.0f,1.0f), 
    glm::vec4(0.0f,0.0f,1.0f,1.0f), 
    glm::vec4(1.0f,0.0f,1.0f,1.0f) 
};
vi lr = {1,0,-1,0};
vi ud = {0,1,0,-1};

unsigned int VBO_cuerpo_fantasma, VAO_cuerpo_fantasma, texture_verticesCuerpoFantasma;
float verticesCuerpoFantasma[] = {
        -fantasma_width, -fantasma_heigth, -fantasma_length,
         fantasma_width, -fantasma_heigth, -fantasma_length,
         fantasma_width,  fantasma_heigth, -fantasma_length,
         fantasma_width,  fantasma_heigth, -fantasma_length,
        -fantasma_width,  fantasma_heigth, -fantasma_length,
        -fantasma_width, -fantasma_heigth, -fantasma_length,

        -fantasma_width, -fantasma_heigth,  fantasma_length,
         fantasma_width, -fantasma_heigth,  fantasma_length,
         fantasma_width,  fantasma_heigth,  fantasma_length,
         fantasma_width,  fantasma_heigth,  fantasma_length,
        -fantasma_width,  fantasma_heigth,  fantasma_length,
        -fantasma_width, -fantasma_heigth,  fantasma_length,

        -fantasma_width,  fantasma_heigth,  fantasma_length,
        -fantasma_width,  fantasma_heigth, -fantasma_length,
        -fantasma_width, -fantasma_heigth, -fantasma_length,
        -fantasma_width, -fantasma_heigth, -fantasma_length,
        -fantasma_width, -fantasma_heigth,  fantasma_length,
        -fantasma_width,  fantasma_heigth,  fantasma_length,

         fantasma_width,  fantasma_heigth,  fantasma_length,
         fantasma_width,  fantasma_heigth, -fantasma_length,
         fantasma_width, -fantasma_heigth, -fantasma_length,
         fantasma_width, -fantasma_heigth, -fantasma_length,
         fantasma_width, -fantasma_heigth,  fantasma_length,
         fantasma_width,  fantasma_heigth,  fantasma_length,

        -fantasma_width, -fantasma_heigth, -fantasma_length,
         fantasma_width, -fantasma_heigth, -fantasma_length,
         fantasma_width, -fantasma_heigth,  fantasma_length,
         fantasma_width, -fantasma_heigth,  fantasma_length,
        -fantasma_width, -fantasma_heigth,  fantasma_length,
        -fantasma_width, -fantasma_heigth, -fantasma_length,

        -fantasma_width,  fantasma_heigth, -fantasma_length,
         fantasma_width,  fantasma_heigth, -fantasma_length,
         fantasma_width,  fantasma_heigth,  fantasma_length,
         fantasma_width,  fantasma_heigth,  fantasma_length,
        -fantasma_width,  fantasma_heigth,  fantasma_length,
        -fantasma_width,  fantasma_heigth, -fantasma_length
};

float verticesCabezaFantasma[18];
unsigned int VBO_cabeza_fantasma[1], VAO_cabeza_fantasma[1];

// Pacman
int sX, eX;
float incrRadPacman = 180 / 10;
float radioPacman = 0.25;
float tethaPacman, phiPacman;
float verticesPacman[36];
// escala
bool boolTime=0;
int tiempo = 500;
int acTime = 0;
int countScala = 0;
float scale = 1.3;
float acScala = 1;
// rotacion
float angulo = 2.5f;
float _angulo = angulo;
float limit_angulo = 45.0f;
float ac_angulo = 0.0f;

unsigned int VBOPacman[1], VAOPacman[1];

float wall[] = {
        -wall_width, -wall_heigth, -wall_length,  0.0f, 0.0f,
         wall_width, -wall_heigth, -wall_length,  1.0f, 0.0f,
         wall_width,  wall_heigth, -wall_length,  1.0f, 1.0f,
         wall_width,  wall_heigth, -wall_length,  1.0f, 1.0f,
        -wall_width,  wall_heigth, -wall_length,  0.0f, 1.0f,
        -wall_width, -wall_heigth, -wall_length,  0.0f, 0.0f,

        -wall_width, -wall_heigth,  wall_length,  0.0f, 0.0f,
         wall_width, -wall_heigth,  wall_length,  1.0f, 0.0f,
         wall_width,  wall_heigth,  wall_length,  1.0f, 1.0f,
         wall_width,  wall_heigth,  wall_length,  1.0f, 1.0f,
        -wall_width,  wall_heigth,  wall_length,  0.0f, 1.0f,
        -wall_width, -wall_heigth,  wall_length,  0.0f, 0.0f,

        -wall_width,  wall_heigth,  wall_length,  1.0f, 0.0f,
        -wall_width,  wall_heigth, -wall_length,  1.0f, 1.0f,
        -wall_width, -wall_heigth, -wall_length,  0.0f, 1.0f,
        -wall_width, -wall_heigth, -wall_length,  0.0f, 1.0f,
        -wall_width, -wall_heigth,  wall_length,  0.0f, 0.0f,
        -wall_width,  wall_heigth,  wall_length,  1.0f, 0.0f,

         wall_width,  wall_heigth,  wall_length,  1.0f, 0.0f,
         wall_width,  wall_heigth, -wall_length,  1.0f, 1.0f,
         wall_width, -wall_heigth, -wall_length,  0.0f, 1.0f,
         wall_width, -wall_heigth, -wall_length,  0.0f, 1.0f,
         wall_width, -wall_heigth,  wall_length,  0.0f, 0.0f,
         wall_width,  wall_heigth,  wall_length,  1.0f, 0.0f,

        -wall_width, -wall_heigth, -wall_length,  0.0f, 1.0f,
         wall_width, -wall_heigth, -wall_length,  1.0f, 1.0f,
         wall_width, -wall_heigth,  wall_length,  1.0f, 0.0f,
         wall_width, -wall_heigth,  wall_length,  1.0f, 0.0f,
        -wall_width, -wall_heigth,  wall_length,  0.0f, 0.0f,
        -wall_width, -wall_heigth, -wall_length,  0.0f, 1.0f,

        -wall_width,  wall_heigth, -wall_length,  0.0f, 1.0f,
         wall_width,  wall_heigth, -wall_length,  1.0f, 1.0f,
         wall_width,  wall_heigth,  wall_length,  1.0f, 0.0f,
         wall_width,  wall_heigth,  wall_length,  1.0f, 0.0f,
        -wall_width,  wall_heigth,  wall_length,  0.0f, 0.0f,
        -wall_width,  wall_heigth, -wall_length,  0.0f, 1.0f
};

unsigned int VBO_wall, VAO_wall, texture_wall;

int main()
{
    rows=20, columns=20;
    Laberinto laberinto(rows,columns);
    laberinto.showMatrix();

    laberinto.fixed();
    laberinto.transp();
    std::swap(rows, columns);

    laberinto.showMatrix();
    laberinto_matrix = laberinto.getLaberinto();
    puntosComida = laberinto.getPuntosComida();
    puntosFruta = laberinto.getPuntosFruta();
    puntosFantasma = laberinto.getPuntosFantasma();

    posicionesFantasma.resize(laberinto.getNFantasmas());
    for (auto it = puntosComida.begin(); it != puntosComida.end(); ++it)
        std::cout << it->first << " " << it->second << '\n';

    sX = laberinto.getStartx()+1;
    eX = laberinto.getEndx()+1;
    // std::cout << sX << " " << eX << '\n';

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 pos_camera = glm::vec3(-rows / 2 -1, 0.0f, - columns / 2 + sX);
    // falta corregir direccion de la camara
    glm::vec3 front_camera = glm::vec3(0.0f, 0.0f, 0.0f);
    camera1.setCamera(pos_camera, up, front_camera);

    int state = iniWindow();
    if (state) return -1;
    
    Shader wall("proyecto.vs", "proyecto.fs");
    iniWall();
    Shader sphere("fantasma.vs", "fantasma.fs");
    iniSphere();
    Shader pacman("color.vs", "color.fs");
    iniPacman();
    Shader cuerpoFantasma("fantasma.vs", "fantasma.fs");
    Shader cabezaFantasma("fantasma.vs", "fantasma.fs");
    iniFantasma();

    //view_wall = glm::lookAt(pos_camera, glm::vec3(0.0, 0.0, 0.0), up);    

    while (!glfwWindowShouldClose(window))
    {
        // mouse
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
         
        // ventana
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // escala
        acScala = 1.0f;
        for (int i = 0; i < countScala; ++i)
            acScala *= scale;
        if (boolTime)
        {
            if (acTime % tiempo == 0)
                countScala--;
            acTime--;
        }
        if (acTime <= 0)
        {
            countScala = 0;
            acTime = 0;
            boolTime = 0;
        }

        // rotacion
        if (ac_angulo > limit_angulo)
        {
            ac_angulo = limit_angulo;
            _angulo = -angulo;
        }
        else if (ac_angulo < angulo)
        {
            ac_angulo = angulo;
            _angulo = angulo;
        }
        ac_angulo += _angulo;

        // primera camaraa
        glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT);

        // proyeccion
        projection = glm::perspective(glm::radians(camera1.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), acScala*radioPacman+0.1f, 100.0f);
        view = camera1.GetViewMatrix();     // vista (lookat)

        i_fantasma = 0;
        i_fruta = 0;
        game(wall, sphere, pacman, cuerpoFantasma, cabezaFantasma);

        // segunda camara
        glViewport(SCR_WIDTH / 2, 0, SCR_WIDTH / 2, SCR_HEIGHT);

        // proyeccion
        projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 100.0f);
        view = glm::lookAt(pos_camera2, pos_camera2 + front_camera2, up2);     

        i_fantasma = 0;
        i_fruta = 0;
        game(wall, sphere, pacman, cuerpoFantasma, cabezaFantasma);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_wall);
    glDeleteBuffers(1, &VBO_wall);

    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);

    glDeleteVertexArrays(1, VAOPacman);
    glDeleteBuffers(1, VBOPacman);

    glfwTerminate();
    return 0;
}

int iniWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto final Computacion Grafica", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    return 0;
}

void game(Shader& wall, Shader& sphere, Shader& pacman, Shader& cuerpoFantasma, Shader& cabezaFantasma)
{
    // muro
    wall.use();
    wall.setMat4("projection", projection);
    wall.setMat4("view", view);

    // esferas
    sphere.use();
    sphere.setMat4("projection", projection);
    sphere.setMat4("view", view);

    // fantasmas
    cuerpoFantasma.use();
    cuerpoFantasma.setMat4("projection", projection);
    cuerpoFantasma.setMat4("view", view);

    cabezaFantasma.use();
    cabezaFantasma.setMat4("projection", projection);
    cabezaFantasma.setMat4("view", view);

    // laberinto
    renderLab(wall, sphere, cuerpoFantasma, cabezaFantasma);

    // pacman
    glBindBuffer(GL_ARRAY_BUFFER, VBOPacman[0]);
    drawPacman(pacman);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void iniWall()
{
    glGenVertexArrays(1, &VAO_wall);
    glGenBuffers(1, &VBO_wall);
    glGenTextures(1, &texture_wall);

    glBindVertexArray(VAO_wall);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_wall);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall), wall, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, texture_wall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("pared_piedra_negra.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void iniSphere()
{
    // colores de la esfera

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    /*
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void iniPacman()
{
    // colo amarillo de pacman
    verticesPacman[3] = 1.0f; verticesPacman[4] = 1.0f; verticesPacman[5] = 0.0f;
    verticesPacman[9] = 1.0f; verticesPacman[10] = 1.0f; verticesPacman[11] = 0.0f;
    verticesPacman[15] = 1.0f; verticesPacman[16] = 1.0f; verticesPacman[17] = 0.0f;
    verticesPacman[21] = 1.0f; verticesPacman[22] = 1.0f; verticesPacman[23] = 0.0f;
    verticesPacman[27] = 1.0f; verticesPacman[28] = 1.0f; verticesPacman[29] = 0.0f;
    verticesPacman[33] = 1.0f; verticesPacman[34] = 1.0f; verticesPacman[35] = 0.0f;

    glGenVertexArrays(1, VAOPacman);
    glGenBuffers(1, VBOPacman);

    glBindVertexArray(VAOPacman[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOPacman[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPacman), verticesPacman, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void iniFantasma()
{
    // cuerpo
    glGenVertexArrays(1, &VAO_cuerpo_fantasma);
    glGenBuffers(1, &VBO_cuerpo_fantasma);

    glBindVertexArray(VAO_cuerpo_fantasma);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cuerpo_fantasma);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCuerpoFantasma), verticesCuerpoFantasma, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // cabeza
    glGenVertexArrays(1, VAO_cabeza_fantasma);
    glGenBuffers(1, VBO_cabeza_fantasma);

    glBindVertexArray(VAO_cabeza_fantasma[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cabeza_fantasma[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCabezaFantasma), verticesCabezaFantasma, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderLab(Shader& wall, Shader& sphere, Shader& cuerpoFantasma, Shader& cabezaFantasma)
{
    for (x_wall = 0; x_wall < rows; ++x_wall)
    {
        for (y_wall = 0; y_wall < columns; ++y_wall)
        {
            if (laberinto_matrix[x_wall][y_wall] == 1)
            {
                // muro
                wall.use();
                glBindVertexArray(VAO_wall);
                glBindBuffer(GL_ARRAY_BUFFER, VBO_wall);
                drawWall(wall);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            else if (laberinto_matrix[x_wall][y_wall] == 4)
            {
                // comida
                sphere.use();
                glBindVertexArray(VAO[0]);                
                glBindBuffer(GL_ARRAY_BUFFER, VAO[0]);
                sphere.setVec4("ourColor2", glm::vec4(1.0f, 0.5f, 0.5f,1.0f));
                drawSphere(sphere);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            else if (laberinto_matrix[x_wall][y_wall] == 5)
            {
                // fantasma
                drawFantasma(cuerpoFantasma, cabezaFantasma);
                i_fantasma++;
            }
            else if (laberinto_matrix[x_wall][y_wall] == 6)
            {
                // fruta
                sphere.use();
                glBindVertexArray(VAO[0]);
                glBindBuffer(GL_ARRAY_BUFFER, VAO[0]);
                sphere.setVec4("ourColor2", coloresFruta[i_fruta]);
                drawSphere(sphere);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                i_fruta++;
            }
        }
    }
}

void drawWall(Shader& wall)
{
    transf = glm::mat4(1.0f);
    glm::vec3 limit = glm::vec3(x_wall-rows/2, 0.0f, - y_wall + columns / 2);
    //glm::vec3 limit = glm::vec3(n_walls_lf*wall_width - k * wall_heigth + dif/2.0f, 3*dif/2, 0.0f);
    transf = glm::translate(transf, limit);
    //transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
    wall.setMat4("transf", transf);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSphere(Shader& sphere)
{
    transf = glm::mat4(1.0f);
    glm::vec3 limit = glm::vec3(x_wall - rows / 2, 0.0f, - y_wall + columns / 2);
    //glm::vec3 limit = glm::vec3(n_walls_lf*wall_width - k * wall_heigth + dif/2.0f, 3*dif/2, 0.0f);
    transf = glm::translate(transf, limit);
    if (laberinto_matrix[x_wall][y_wall] == 6)      // es fruta
        transf = glm::scale(transf, glm::vec3(1.5f, 2.5f, 1.5f));
    sphere.setMat4("transf", transf);
    for (tetha = 0; tetha < 180; tetha += incrRad) {
        for (phi = 0; phi < 360; phi += incrRad) {
            vertices[0] = radio * sin(glm::radians(tetha)) * sin(glm::radians(phi));
            vertices[1] = radio * cos(glm::radians(tetha));
            vertices[2] = radio * sin(glm::radians(tetha)) * cos(glm::radians(phi));

            vertices[3] = radio * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi));
            vertices[4] = radio * cos(glm::radians(tetha + incrRad));
            vertices[5] = radio * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi));

            vertices[6] = radio * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi + incrRad));
            vertices[7] = radio * cos(glm::radians(tetha + incrRad));
            vertices[8] = radio * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi + incrRad));

            vertices[9] = radio * sin(glm::radians(tetha)) * sin(glm::radians(phi));
            vertices[10] = radio * cos(glm::radians(tetha));
            vertices[11] = radio * sin(glm::radians(tetha)) * cos(glm::radians(phi));

            vertices[12] = radio * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi + incrRad));
            vertices[13] = radio * cos(glm::radians(tetha + incrRad));
            vertices[14] = radio * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi + incrRad));

            vertices[15] = radio * sin(glm::radians(tetha)) * sin(glm::radians(phi + incrRad));
            vertices[16] = radio * cos(glm::radians(tetha));
            vertices[17] = radio * sin(glm::radians(tetha)) * cos(glm::radians(phi + incrRad));
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

void drawFantasma(Shader& cuerpoFantasma, Shader& cabezaFantasma)
{
    transf = glm::mat4(1.0f);
    glm::vec3 limit;
    limit = glm::vec3(x_wall - rows / 2, 0.0, -y_wall + columns / 2);
    /*
    if (primeraPosFantasma)
    {
        posicionesFantasma[i_fantasma] = glm::vec2(x_wall - rows / 2, -y_wall + columns / 2);
        limit = glm::vec3(x_wall - rows / 2, 0.0, -y_wall + columns / 2);
        primeraPosFantasma = 0;
    }
    else
    {
        if (contMov % tiempoMov == 0)
        {
            // num = rand() % (max - min) + min;
            int i = rand() % (4);
            while (laberinto_matrix[posicionesFantasma[i_fantasma][0] + ud[i]][posicionesFantasma[i_fantasma][1] + lr[i]] != 0)
            {
                i = rand() % (4);
            }
            limit = glm::vec3(posicionesFantasma[i_fantasma][0] + ud[i]/2, 0.0f, posicionesFantasma[i_fantasma][1] + lr[i]/2);
        }
    }*/
    contMov++;
    // dibuja cuerpo
    glBindVertexArray(VAO_cuerpo_fantasma);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cuerpo_fantasma);
    cuerpoFantasma.use();
    transf = glm::translate(transf, limit);
    cuerpoFantasma.setVec4("ourColor2", coloresFantasmas[i_fantasma]);
    cuerpoFantasma.setMat4("transf", transf);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // dibuja cabeza
    glBindVertexArray(VAO_cabeza_fantasma[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cabeza_fantasma[0]);
    cabezaFantasma.use();
    transf = glm::translate(transf, glm::vec3(0.0f, fantasma_heigth, 0.0f));
    cabezaFantasma.setVec4("ourColor2", coloresFantasmas[i_fantasma]);
    cabezaFantasma.setMat4("transf", transf);
    for (tetha = 0; tetha < 90; tetha += incrRad) {
        for (phi = 0; phi < 360; phi += incrRad) {
            verticesCabezaFantasma[0] = radioFantasma * sin(glm::radians(tetha)) * sin(glm::radians(phi));
            verticesCabezaFantasma[1] = radioFantasma * cos(glm::radians(tetha));
            verticesCabezaFantasma[2] = radioFantasma * sin(glm::radians(tetha)) * cos(glm::radians(phi));

            verticesCabezaFantasma[3] = radioFantasma * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi));
            verticesCabezaFantasma[4] = radioFantasma * cos(glm::radians(tetha + incrRad));
            verticesCabezaFantasma[5] = radioFantasma * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi));

            verticesCabezaFantasma[6] = radioFantasma * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi + incrRad));
            verticesCabezaFantasma[7] = radioFantasma * cos(glm::radians(tetha + incrRad));
            verticesCabezaFantasma[8] = radioFantasma * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi + incrRad));

            verticesCabezaFantasma[9] = radioFantasma * sin(glm::radians(tetha)) * sin(glm::radians(phi));
            verticesCabezaFantasma[10] = radioFantasma * cos(glm::radians(tetha));
            verticesCabezaFantasma[11] = radioFantasma * sin(glm::radians(tetha)) * cos(glm::radians(phi));

            verticesCabezaFantasma[12] = radioFantasma * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi + incrRad));
            verticesCabezaFantasma[13] = radioFantasma * cos(glm::radians(tetha + incrRad));
            verticesCabezaFantasma[14] = radioFantasma * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi + incrRad));

            verticesCabezaFantasma[15] = radioFantasma * sin(glm::radians(tetha)) * sin(glm::radians(phi + incrRad));
            verticesCabezaFantasma[16] = radioFantasma * cos(glm::radians(tetha));
            verticesCabezaFantasma[17] = radioFantasma * sin(glm::radians(tetha)) * cos(glm::radians(phi + incrRad));
            //glBindVertexArray(VAO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCabezaFantasma), verticesCabezaFantasma, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool findComida(float x, float y)
{
    if (find(puntosComida.begin(), puntosComida.end(), std::make_pair(int(y), int(x))) == puntosComida.end())
        return false;
    return true;
}

bool findFruta(float x, float y)
{
    if (find(puntosFruta.begin(), puntosFruta.end(), std::make_pair(int(y), int(x))) == puntosFruta.end())
        return false;
    return true;
}

bool findFantasma(float x, float y)
{
    if (find(puntosFantasma.begin(), puntosFantasma.end(), std::make_pair(int(y), int(x))) == puntosFantasma.end())
        return false;
    return true;
}

void drawPacman(Shader& pacman)
{
    pacman.use();
    transf = glm::mat4(1.0f);
    glm::vec3 pos = camera1.getPosition();      // extrae coordenadas actuales de la camara para desplazar el pacman
    //glm::vec3 pos = glm::vec3(-rows / 2 - 1, 0.0f, -columns / 2 + sX);
    float x = pos[0] + float(rows / 2);
    float z = pos[2] + float(columns / 2) - 1;
    
    //std::cout << acTime <<" "<<countScala << " " << acScala<< '\n';
    transf = glm::translate(transf, pos);
    transf = glm::rotate(transf, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
    transf = glm::rotate(transf, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    transf = glm::rotate(transf, glm::radians(ac_angulo), glm::vec3(0.0, 1.0, 0.0));
    
    if (findFruta(x, z))                       // si encuentra fruta
    {
        if (laberinto_matrix[int(x)][columns - 1 - int(z)] == 6)
        {
            laberinto_matrix[int(x)][columns - 1 - int(z)] = 0;     // borra fruta
            countScala++;           // acumula escala
            boolTime = 1;
            acTime += tiempo - 1;     // acumula tiempo en el que escala
        }
    }
    else if (findFantasma(x, z))
    {
        if (laberinto_matrix[int(x)][columns - 1 - int(z)] == 5 && acScala >= 1.5f)
            laberinto_matrix[int(x)][columns - 1 - int(z)] = 0;     // borra fantasma
    }
    else if (findComida(x, z))                       // si encuentra comida
    {
        if (laberinto_matrix[int(x)][columns - 1 - int(z)] == 4)
            laberinto_matrix[int(x)][columns - 1 - int(z)] = 0;     // borra comida
    }
    //acScala *= 2;
    transf = glm::scale(transf, glm::vec3(acScala, acScala, acScala));
    pacman.setMat4("transf", transf);
    pacman.setMat4("projection", projection);
    pacman.setMat4("view", view);

    // figura
    glBindVertexArray(VAOPacman[0]);
    for (tethaPacman = 0; tethaPacman < 180; tethaPacman += incrRadPacman) {
        for (phiPacman = 0; phiPacman < 180; phiPacman += incrRadPacman) {
            verticesPacman[0] = radioPacman * sin(glm::radians(tethaPacman)) * sin(glm::radians(phiPacman));
            verticesPacman[1] = radioPacman * cos(glm::radians(tethaPacman));
            verticesPacman[2] = radioPacman * sin(glm::radians(tethaPacman)) * cos(glm::radians(phiPacman));

            verticesPacman[6] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * sin(glm::radians(phiPacman));
            verticesPacman[7] = radioPacman * cos(glm::radians(tethaPacman + incrRadPacman));
            verticesPacman[8] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * cos(glm::radians(phiPacman));

            verticesPacman[12] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * sin(glm::radians(phiPacman + incrRadPacman));
            verticesPacman[13] = radioPacman * cos(glm::radians(tethaPacman + incrRadPacman));
            verticesPacman[14] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * cos(glm::radians(phiPacman + incrRadPacman));

            verticesPacman[18] = radioPacman * sin(glm::radians(tethaPacman)) * sin(glm::radians(phiPacman));
            verticesPacman[19] = radioPacman * cos(glm::radians(tethaPacman));
            verticesPacman[20] = radioPacman * sin(glm::radians(tethaPacman)) * cos(glm::radians(phiPacman));

            verticesPacman[24] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * sin(glm::radians(phiPacman + incrRadPacman));
            verticesPacman[25] = radioPacman * cos(glm::radians(tethaPacman + incrRadPacman));
            verticesPacman[26] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * cos(glm::radians(phiPacman + incrRadPacman));

            verticesPacman[30] = radioPacman * sin(glm::radians(tethaPacman)) * sin(glm::radians(phiPacman + incrRadPacman));
            verticesPacman[31] = radioPacman * cos(glm::radians(tethaPacman));
            verticesPacman[32] = radioPacman * sin(glm::radians(tethaPacman)) * cos(glm::radians(phiPacman + incrRadPacman));
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPacman), verticesPacman, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    pacman.use();
    transf = glm::mat4(1.0f);
    transf = glm::translate(transf, pos);
    transf = glm::rotate(transf, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
    transf = glm::rotate(transf, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
    transf = glm::rotate(transf, glm::radians(-ac_angulo), glm::vec3(0.0, 1.0, 0.0));
    transf = glm::scale(transf, glm::vec3(acScala, acScala, acScala));
    
    pacman.setMat4("transf", transf);
    glBindVertexArray(VAOPacman[0]);
    for (tethaPacman = 0; tethaPacman < 180; tethaPacman += incrRadPacman) {
        for (phiPacman = 0; phiPacman < 180; phiPacman += incrRadPacman) {
            verticesPacman[0] = radioPacman * sin(glm::radians(tethaPacman)) * sin(glm::radians(phiPacman));
            verticesPacman[1] = radioPacman * cos(glm::radians(tethaPacman));
            verticesPacman[2] = radioPacman * sin(glm::radians(tethaPacman)) * cos(glm::radians(phiPacman));

            verticesPacman[6] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * sin(glm::radians(phiPacman));
            verticesPacman[7] = radioPacman * cos(glm::radians(tethaPacman + incrRadPacman));
            verticesPacman[8] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * cos(glm::radians(phiPacman));

            verticesPacman[12] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * sin(glm::radians(phiPacman + incrRadPacman));
            verticesPacman[13] = radioPacman * cos(glm::radians(tethaPacman + incrRadPacman));
            verticesPacman[14] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * cos(glm::radians(phiPacman + incrRadPacman));

            verticesPacman[18] = radioPacman * sin(glm::radians(tethaPacman)) * sin(glm::radians(phiPacman));
            verticesPacman[19] = radioPacman * cos(glm::radians(tethaPacman));
            verticesPacman[20] = radioPacman * sin(glm::radians(tethaPacman)) * cos(glm::radians(phiPacman));

            verticesPacman[24] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * sin(glm::radians(phiPacman + incrRadPacman));
            verticesPacman[25] = radioPacman * cos(glm::radians(tethaPacman + incrRadPacman));
            verticesPacman[26] = radioPacman * sin(glm::radians(tethaPacman + incrRadPacman)) * cos(glm::radians(phiPacman + incrRadPacman));

            verticesPacman[30] = radioPacman * sin(glm::radians(tethaPacman)) * sin(glm::radians(phiPacman + incrRadPacman));
            verticesPacman[31] = radioPacman * cos(glm::radians(tethaPacman));
            verticesPacman[32] = radioPacman * sin(glm::radians(tethaPacman)) * cos(glm::radians(phiPacman + incrRadPacman));
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPacman), verticesPacman, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera1.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera1.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera1.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera1.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera1.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera1.ProcessMouseScroll(static_cast<float>(yoffset));
}