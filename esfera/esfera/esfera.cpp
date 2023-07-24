#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_.h"
#include "camera_.h"

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
void iniSphere();
void iniWall();

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
unsigned int VBO_wall, VAO_wall, texture_wall;


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



glm::vec3 despl[] = {
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(-1.0f, -1.0f, 0.0f),
    glm::vec3(1.0f, -1.0f, 0.0f),
    glm::vec3(-1.0f, 1.0f, 0.0f)
};

// sphere
float pi = 3.1416f;
float incrRad = 180 / 10;
float radio = 0.5;
float tetha, phi;
float vertices[36];

unsigned int VBO[1], VAO[1];


int main()
{
    //tetha = 180;
    //phi = 180;
    // colores
    vertices[3] = 0.0f; vertices[4] = 0.0f; vertices[5] = 1.0f;
    vertices[9] = 0.0f; vertices[10] = 0.0f; vertices[11] = 1.0f;
    vertices[15] = 0.0f; vertices[16] = 0.0f; vertices[17] = 1.0f;
    vertices[21] = 0.0f; vertices[22] = 0.0f; vertices[23] = 1.0f;
    vertices[27] = 0.0f; vertices[28] = 0.0f; vertices[29] = 1.0f;
    vertices[33] = 0.0f; vertices[34] = 0.0f; vertices[35] = 1.0f;

    int state = iniWindow();
    if (state) return -1;

    Shader wall("proyecto.vs", "proyecto.fs");
    iniWall();
    Shader sphere("color.vs", "color.fs");
    iniSphere();
    
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

        // textura
        glBindTexture(GL_TEXTURE_2D, texture_wall);

        // proyeccion
        wall.use();
        projection = glm::perspective(glm::radians(camera1.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 100.0f);
        wall.setMat4("projection", projection);

        // vista (lookat)
        view = camera1.GetViewMatrix();
        wall.setMat4("view", view);

        glBindVertexArray(VAO_wall);
        transf = glm::mat4(1.0f);
        glm::vec3 limit = glm::vec3(0, 0, 0.0f);
        //glm::vec3 limit = glm::vec3(n_walls_lf*wall_width - k * wall_heigth + dif/2.0f, 3*dif/2, 0.0f);
        transf = glm::translate(transf, limit);
        //transf_wall = glm::scale(transf_wall, glm::vec3(0.5f, 0.5f, 0.5f));
        wall.setMat4("transf", transf);

        glDrawArrays(GL_TRIANGLES, 0, 36);


        sphere.use();
        sphere.setMat4("projection", projection);
        sphere.setMat4("view", view);
        //transf = glm::translate(transf, glm::vec3(0,0,0));
        //sphere.setMat4("transf", transf);
        // figura
        glBindVertexArray(VAO[0]);
        for (int i = 0; i < 4; ++i)
        {
            transf= glm::mat4(1.0f);
            transf = glm::translate(transf, despl[i]);
            sphere.setMat4("transf", transf);
            for (tetha = 0; tetha < 180; tetha += incrRad) {
                for (phi = 0; phi < 360; phi += incrRad) {
                    vertices[0] = radio * sin(glm::radians(tetha)) * sin(glm::radians(phi));
                    vertices[1] = radio * cos(glm::radians(tetha));
                    vertices[2] = radio * sin(glm::radians(tetha)) * cos(glm::radians(phi));

                    vertices[6] = radio * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi));
                    vertices[7] = radio * cos(glm::radians(tetha + incrRad));
                    vertices[8] = radio * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi));

                    vertices[12] = radio * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi + incrRad));
                    vertices[13] = radio * cos(glm::radians(tetha + incrRad));
                    vertices[14] = radio * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi + incrRad));

                    vertices[18] = radio * sin(glm::radians(tetha)) * sin(glm::radians(phi));
                    vertices[19] = radio * cos(glm::radians(tetha));
                    vertices[20] = radio * sin(glm::radians(tetha)) * cos(glm::radians(phi));

                    vertices[24] = radio * sin(glm::radians(tetha + incrRad)) * sin(glm::radians(phi + incrRad));
                    vertices[25] = radio * cos(glm::radians(tetha + incrRad));
                    vertices[26] = radio * sin(glm::radians(tetha + incrRad)) * cos(glm::radians(phi + incrRad));

                    vertices[30] = radio * sin(glm::radians(tetha)) * sin(glm::radians(phi + incrRad));
                    vertices[31] = radio * cos(glm::radians(tetha));
                    vertices[32] = radio * sin(glm::radians(tetha)) * cos(glm::radians(phi + incrRad));
                    //glBindVertexArray(VAO[0]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
            }
        }
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);

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

void iniSphere()
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
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