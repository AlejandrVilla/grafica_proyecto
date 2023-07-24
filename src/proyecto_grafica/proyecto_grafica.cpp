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
#define vi std::vector<int>
#define vvi std::vector<vi>

// window
GLFWwindow* window;
int iniWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 pos_camera = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 front_camera = glm::vec3(0.0f, 0.0f, -1.0f);

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
void iniWall();
void renderWalls(Shader& wall);
void drawWall(Shader& wall);

void renderLimitWalls(Shader& wall);
void renderWall(Shader& wall);
// shader, cuantos muros, dirrecion u/d, direccion l/r
void drawWall(Shader& wall, const int& n, const bool& vert, const bool& hor);

//glm::mat4 model_wall = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 transf = glm::mat4(1.0f);

// wall
float wall_width = 0.50;
float wall_heigth = 0.50;
float wall_length = 1.0;
float x_wall, y_wall;
std::vector<float> aux_x_wall(100);
std::vector<float> aux_y_wall(100);

float dif; // diferencia entre width y height
unsigned int n_walls_ud = 15;
unsigned int n_walls_lf= 18;

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

glm::vec3 vertical[] = {
    glm::vec3(0.0f, 2 * wall_heigth, 0.0f),
    glm::vec3(0.0f, wall_heigth, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, -wall_heigth, 0.0f),
    glm::vec3(4 * wall_width, 0.0f, 0.0f),
    glm::vec3(4 * wall_width, wall_heigth, 0.0f),
    glm::vec3(4 * wall_width, 2 * wall_heigth, 0.0f),
    glm::vec3(8 * wall_width, 0.0f, 0.0f),
    glm::vec3(8 * wall_width, wall_heigth, 0.0f),
    glm::vec3(8 * wall_width, 2 * wall_heigth, 0.0f),
    glm::vec3(8 * wall_width, -wall_heigth, 0.0f)
};

glm::vec3 horizontal[] = {
    glm::vec3(1.0f, -0.5f, 0.0f),
    glm::vec3(1.0f - wall_heigth, -0.5f, 0.0f),
    glm::vec3(1.0f - 2 * wall_heigth, -0.5f, 0.0f)
};

unsigned int VBO_wall, VAO_wall, texture_wall;


int main()
{
    rows=50, columns=50;
    Laberinto laberinto(rows,columns);
    laberinto.showMatrix();
    laberinto.fixed();
    laberinto.transp();
    std::swap(rows, columns);
    // laberinto.showMatrix();
    laberinto_matrix = laberinto.getLaberinto();

    int state = iniWindow();
    if (state) return -1;
    
    Shader wall("proyecto.vs", "proyecto.fs");
    iniWall();

    //view_wall = glm::lookAt(pos_camera, glm::vec3(0.0, 0.0, 0.0), up);
    dif = abs(wall_heigth - wall_width);
    

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

        // textura
        glBindTexture(GL_TEXTURE_2D, texture_wall);

        // proyeccion
        wall.use();
        projection = glm::perspective(glm::radians(camera1.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 100.0f);
        wall.setMat4("projection", projection);

        // vista (lookat)
        view = camera1.GetViewMatrix();
        wall.setMat4("view", view);

        // figura
        glBindVertexArray(VAO_wall);

        /*
        // prueba
        transf = glm::mat4(1.0f);
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        transf = glm::mat4(1.0f);
        //transf = glm::translate(transf, glm::vec3(0.0f, 0.2f, 0.0f));
        transf = glm::translate(transf, glm::vec3(wall_width,wall_heigth,0.0f));
        //transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        //transf = glm::rotate(transf, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */

        // laberinto
        //x_wall = 0;
        //y_wall = 0;
        renderWalls(wall);
        //renderLimitWalls(wall);
        //renderWall(wall);

        /*
        for (int k = 1; k <= 5; ++k)
        {
            y_wall += wall_heigth;
            transf = glm::mat4(1.0f);
            glm::vec3 limit = glm::vec3(x_wall, y_wall, 0.0f);
            // glm::vec3 limit = glm::vec3(n_walls_lf * wall_width - 4 * wall_heigth, k * wall_heigth + dif, 0.0f);
            transf = glm::translate(transf, limit);
            //transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
            wall.setMat4("transf", transf);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        
        /*
        for (unsigned int i = 0; i < 3; ++i)
        {
            transf = glm::mat4(1.0f);
            //transf = glm::translate(transf, glm::vec3(0.0f, 0.2f, 0.0f));
            transf = glm::translate(transf, horizontal[i]);
            transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
            wall.setMat4("transf", transf);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_wall);
    glDeleteBuffers(1, &VBO_wall);

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
    unsigned char* data = stbi_load("img/galaxia.jpg", &width, &height, &nrChannels, 0);
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

}

void renderLimitWalls(Shader& wall)
{
    // limites vertical derecha superior
    for (unsigned int k = 1; k < n_walls_ud; ++k)
    {
        transf = glm::mat4(1.0f);
        //transf = glm::translate(transf, glm::vec3(0.0f, 0.2f, 0.0f));
        glm::vec3 limit = glm::vec3(n_walls_lf * wall_width, k * wall_heigth, 0.0f);
        transf = glm::translate(transf, limit);
        //transf = glm::rotate(transf, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // limites vertical derecha inferior
    for (unsigned int k = 1; k < n_walls_ud; ++k)
    {
        transf = glm::mat4(1.0f);
        //transf = glm::translate(transf, glm::vec3(0.0f, 0.2f, 0.0f));
        glm::vec3 limit = glm::vec3(n_walls_lf * wall_width, k * -wall_heigth, 0.0f);
        transf = glm::translate(transf, limit);
        //transf = glm::rotate(transf, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // limites vertical izquierda superior
    for (unsigned int k = 1; k < n_walls_ud; ++k)
    {
        transf = glm::mat4(1.0f);
        //transf = glm::translate(transf, glm::vec3(0.0f, 0.2f, 0.0f));
        glm::vec3 limit = glm::vec3(n_walls_lf * -wall_width, k * wall_heigth, 0.0f);
        transf = glm::translate(transf, limit);
        //transf = glm::rotate(transf, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // limites vertical izquierda inferior
    for (unsigned int k = 1; k < n_walls_ud; ++k)
    {
        transf = glm::mat4(1.0f);
        //transf = glm::translate(transf, glm::vec3(0.0f, 0.2f, 0.0f));
        glm::vec3 limit = glm::vec3(n_walls_lf * -wall_width, k * -wall_heigth, 0.0f);
        transf = glm::translate(transf, limit);
        //transf = glm::rotate(transf, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    /*
    // limites horizontal ezquierda inferior
    for (unsigned int k = 1; k < n_walls_lf/ (6*wall_width)-1; ++k)
    {
        transf = glm::mat4(1.0f);
        float dif = abs(wall_heigth - wall_width);
        glm::vec3 limit = glm::vec3(k * -wall_width, n_walls_ud * -wall_heigth, 0.0f);
        //glm::vec3 limit = glm::vec3(k*-wall_heigth + dif / 2.0f, n_walls_ud * -wall_heigth + dif / 2.0f, 0.0f);
        transf = glm::translate(transf, limit);
        //transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }*/

    // limites horizontal izquierda inferior
    for (unsigned int k = 1; k < n_walls_lf / (6 * wall_heigth); ++k)
    {
        transf = glm::mat4(1.0f);
        float dif = abs(wall_heigth - wall_width);
        //glm::vec3 limit = glm::vec3(k * wall_width, n_walls_ud * -wall_heigth, 0.0f);
        glm::vec3 limit = glm::vec3(k * -wall_heigth + dif / 2.0f, n_walls_ud * -wall_heigth + dif / 2.0f, 0.0f);
        transf = glm::translate(transf, limit);
        transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // limites horizontal derecha inferior
    for (unsigned int k = 1; k < n_walls_lf / (6 * wall_heigth); ++k)
    {
        transf = glm::mat4(1.0f);
        float dif = abs(wall_heigth - wall_width);
        //glm::vec3 limit = glm::vec3(k * wall_width, n_walls_ud * -wall_heigth, 0.0f);
        glm::vec3 limit = glm::vec3(k * wall_heigth - dif / 2.0f, n_walls_ud * -wall_heigth + dif / 2.0f, 0.0f);
        transf = glm::translate(transf, limit);
        transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // limites horizontal izquierda superior
    for (unsigned int k = 1; k < n_walls_lf / (6 * wall_heigth); ++k)
    {
        transf = glm::mat4(1.0f);
        float dif = abs(wall_heigth - wall_width);
        //glm::vec3 limit = glm::vec3(k * wall_width, n_walls_ud * -wall_heigth, 0.0f);
        glm::vec3 limit = glm::vec3(k * -wall_heigth + dif / 2.0f, n_walls_ud * wall_heigth - dif / 2.0f, 0.0f);
        transf = glm::translate(transf, limit);
        transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // limites horizontal derecha superior
    for (unsigned int k = 1; k < n_walls_lf / (6 * wall_heigth); ++k)
    {
        transf = glm::mat4(1.0f);
        float dif = abs(wall_heigth - wall_width);
        //glm::vec3 limit = glm::vec3(k * wall_width, n_walls_ud * -wall_heigth, 0.0f);
        glm::vec3 limit = glm::vec3(k * wall_heigth - dif / 2.0f, n_walls_ud * wall_heigth - dif / 2.0f, 0.0f);
        transf = glm::translate(transf, limit);
        transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void renderWall(Shader& wall)
{
    // primera parte
    drawWall(wall, 4, true, false);
    x_wall -= dif / 2;
    y_wall -= dif / 2;
    drawWall(wall, 5, true, true);
    x_wall += -dif / 2;
    y_wall += dif / 2;
    drawWall(wall, 3, false, true);
    // guardado 0
    aux_x_wall[0] = x_wall;
    aux_y_wall[0] = y_wall;
    x_wall += dif / 2;
    y_wall += dif / 2;
    drawWall(wall, 3, false, false);
    x_wall += dif / 2;
    y_wall += -dif / 2;
    drawWall(wall, 1, true, false);
    x_wall += -3 * dif / 2;
    y_wall += -3 * dif / 2;
    drawWall(wall, 2, true, true);
    // segunda parte
    x_wall = aux_x_wall[0];
    y_wall = aux_y_wall[0];
    x_wall += dif / 2;
    y_wall += wall_width + dif / 2;
    drawWall(wall, 4, true, true);
    x_wall += 3 * dif / 2;
    y_wall += 3 * dif / 2;
    drawWall(wall, 8, true, false);
    x_wall += -3 * dif / 2;
    y_wall += -3 * dif / 2;
    drawWall(wall, 4, true, true);
    x_wall += wall_heigth - dif / 2;
    y_wall += -2 * wall_heigth + dif / 2;
    drawWall(wall, 7, false, true);
    x_wall += dif / 2;
    y_wall += dif / 2;
    drawWall(wall, 1, false, false);
    // tercera parte
    x_wall = aux_x_wall[0];
    y_wall = aux_y_wall[0];
    x_wall += dif;
    y_wall += wall_heigth + dif;
    drawWall(wall, 5, true, false);
    // guardado 1
    aux_x_wall[1] = x_wall;
    aux_y_wall[1] = y_wall;
    x_wall += -wall_heigth;
    y_wall += 2 * wall_heigth;
    drawWall(wall, 2, false, true);
    x_wall += wall_heigth;
    drawWall(wall, 1, false, true);
    // cuarta parte
    x_wall = aux_x_wall[1];
    y_wall = aux_y_wall[1];
    x_wall += -2 * wall_heigth;
    y_wall += -dif / 2;
    drawWall(wall, 2, true, true);
    x_wall += dif / 2;
    y_wall += dif / 2;
    drawWall(wall, 5, true, false);
    x_wall += -dif / 2;
    y_wall += +dif / 2;
    drawWall(wall, 4, false, false);
    y_wall += -wall_heigth;
    drawWall(wall, 3, false, false);
    x_wall += dif / 2;
    y_wall += -dif / 2;
    drawWall(wall, 3, true, false);
    x_wall += -dif / 2;
    y_wall += -dif / 2;
    drawWall(wall, 2, true, true);
    x_wall += -3 * dif / 2;
    y_wall += 3 * dif / 2;
    drawWall(wall, 3, false, true);
    x_wall += dif / 2;
    y_wall += wall_heigth;
    drawWall(wall, 4, true, false);
    y_wall += 2 * wall_heigth - dif / 2;
    drawWall(wall, 2, false, true);
    x_wall += dif / 2;
    y_wall += -dif / 2;
    drawWall(wall, 7, true, true);
    x_wall += -dif / 2;
    y_wall += dif / 2;
    drawWall(wall, 3, false, true);
    x_wall += dif / 2;
    y_wall += dif / 2;
    drawWall(wall, 2, false, false);
    x_wall += 2*wall_heigth;
    y_wall += -wall_heigth + dif / 2;
    drawWall(wall, 3, true, false);
    // quinta parte
    x_wall = aux_x_wall[1];
    y_wall = aux_x_wall[1];
}

void renderWalls(Shader& wall)
{
    for (x_wall = 0; x_wall < rows; ++x_wall)
    {
        for (y_wall = 0; y_wall < columns; ++y_wall)
        {
            if(laberinto_matrix[x_wall][y_wall])
                drawWall(wall);
        }
    }
}

void drawWall(Shader& wall)
{
    transf = glm::mat4(1.0f);
    glm::vec3 limit = glm::vec3(x_wall-rows/2, y_wall-columns/2, 0.0f);
    //glm::vec3 limit = glm::vec3(n_walls_lf*wall_width - k * wall_heigth + dif/2.0f, 3*dif/2, 0.0f);
    transf = glm::translate(transf, limit);
    //transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
    wall.setMat4("transf", transf);


    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawWall(Shader& wall, const int& n, const bool& vert, const bool& hor)
{
    for (int k = 1; k <= n; ++k)
    {
        if(!vert && hor)        // 0 && 1 -> derecha
            x_wall += wall_heigth;
        else if(vert && !hor)  // 1 && 0 -> izquierda
            x_wall -= wall_heigth;
        else if(vert && hor)   // 1 && 1 -> arriba
            y_wall += wall_heigth;
        else if(!vert && !hor) // 0 && 0 -> abajo
            y_wall -= wall_heigth;
        transf = glm::mat4(1.0f);
        glm::vec3 limit = glm::vec3(x_wall, y_wall, 0.0f);
        //glm::vec3 limit = glm::vec3(n_walls_lf*wall_width - k * wall_heigth + dif/2.0f, 3*dif/2, 0.0f);
        transf = glm::translate(transf, limit);
        if(vert != hor)
            transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);


        glDrawArrays(GL_TRIANGLES, 0, 36);
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera1.ProcessMouseScroll(static_cast<float>(yoffset));
}