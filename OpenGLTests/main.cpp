#pragma once
#include<stdlib.h>
#define GLEW_STATIC
#include"libs/glew-1.13.0/include/GL/glew.h"
#include"libs/glfw-3.2.1/include/x86/glfw3.h"

#include"libs/glm/glm.hpp"
#include"libs/glm/gtc/matrix_transform.hpp"
#include"libs/glm/gtc/type_ptr.hpp"
#include"libs/SOIL/SOIL.h"

#include<functional>
#include<vector>
#include<iostream>
#include<assert.h>
#include<string>
#include<fstream>

#include"../../currying/CurryinginCPP/source/currying.hpp"
#include"shaderHandle.h"
#include"assimpModelLoad.h"
#include"modelHandle.h"
#include"camera.h"
//#include"filesystem.h"
//#include"model.h"

using namespace std;

GLFWwindow* pWindow = nullptr;
int width = 800, height = 600;
shaderHandle::program* pProg = nullptr;

GLfloat vertices[] =
{
    -0.5f, -0.5f, 0.0f,
    0.5f,  -0.5f, 0.0f,
    0.0f,   0.5f, 0.0f
};

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void init();
void initGLEW();
void initGLFW();
void render(modelHandle::SceneWrapper&);

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

void draw(modelHandle::SceneWrapper& scene, shaderHandle::program& prog);
void draw(modelHandle::NodeWrapper& node, shaderHandle::program& prog);
void draw(modelHandle::MeshWrapper& mesh, shaderHandle::program& prog);
modelHandle::Scene loadModel(const string& modelPath);
string textTypeToStr(modelHandle::textureType type);

int main(void)
{
    init();    

    modelHandle::Scene scene = loadModel(global::modelPath + global::modelName);
    modelHandle::SceneWrapper sw(scene);

    while (!glfwWindowShouldClose(pWindow))
    {
         //Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        Do_Movement();
        render(sw);

        glfwSwapBuffers(pWindow);
    }
    
    if(pProg != nullptr) delete pProg;
    pProg = nullptr;
    glfwTerminate();
    return 0;
}

void init()
{
    initGLFW();
    initGLEW();

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    /* callback */
    glfwSetKeyCallback(pWindow, keyCallBack);
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetScrollCallback(pWindow, scroll_callback);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    /* shader */
    using namespace shaderHandle;

    shader<shaderType::eVertexShader> vertexShaderO;
    vertexShaderO.setShaderFromPath(global::vertexShaderPath);
    vertexShaderO.compileShader();
    
    shader<shaderType::eFragmentShader> fragmentShaderO;
    fragmentShaderO.setShaderFromPath(global::fragmentShaderPath);
    fragmentShaderO.compileShader();
    
    pProg = new program();
    pProg->attach(vertexShaderO);
    pProg->attach(fragmentShaderO);
    pProg->linkProgram();
    pProg->detach(vertexShaderO);
    pProg->detach(fragmentShaderO);

}

void initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    pWindow = glfwCreateWindow(800, 600, "simple model viewer", nullptr, nullptr);
    if (pWindow == nullptr)
    {
        cout << "fail to create a window" << endl;
        glfwTerminate();
        assert(-1);
    }
    glfwMakeContextCurrent(pWindow);
    glfwGetFramebufferSize(pWindow, &width, &height);
}

void initGLEW()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "fail to init glew" << endl;
        assert(-1);
    }
}

void keyCallBack(GLFWwindow* p, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void render(modelHandle::SceneWrapper& sw)
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pProg->use();

    glm::mat4 projection = glm::perspective(camera.Zoom, (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(pProg->getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(pProg->getProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

    // Draw the loaded model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    // added end
    glUniformMatrix4fv(glGetUniformLocation(pProg->getProgramID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    draw(sw, *pProg);
}

modelHandle::Scene loadModel(const string& modelPath)
{
    using namespace modelHandle;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "sth wrong with assimp " << importer.GetErrorString() << endl;
    }
    ModelFactory<assimp> MF(scene);
    return MF.produce();
}

void draw(modelHandle::SceneWrapper& scene, shaderHandle::program& prog)
{
    draw(*(scene.m_pRootNode), prog);
}

void draw(modelHandle::NodeWrapper& node, shaderHandle::program& prog)
{
    for (auto& mesh : node.m_Meshes)
    {
        draw(*mesh, prog);
    }

    for (auto& child : node.m_Children)
    {
        draw(*child, prog);
    }
}

void draw(modelHandle::MeshWrapper& mesh, shaderHandle::program& prog)
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    GLuint ambientNr = 1;
    GLuint shininessNr = 1;
    GLuint heightNr = 1;
    GLuint normalsNr = 1;
    for (GLuint i = 0; i < mesh.m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        stringstream ss;
        string number;
        string name = textTypeToStr(mesh.m_textures[i]->m_type);
        if (name == "texture_diffuse")
            ss << diffuseNr++; 
        else if (name == "texture_specular")
            ss << specularNr++; 
        else if (name == "texture_ambient")
            ss << ambientNr++; 
        else if (name == "texture_shiniess")
            ss << shininessNr++;
        else if (name == "texture_height")
            ss << heightNr++; 
        else if (name == "texture_normals")
            ss << normalsNr++; 
        number = ss.str();
        glUniform1i(glGetUniformLocation(prog.getProgramID(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, mesh.m_textures[i]->m_textureID);
    }

    // Draw mesh
    glBindVertexArray(mesh.m_VAO);
    glDrawElements(GL_TRIANGLES, mesh.m_elementNum, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (GLuint i = 0; i < mesh.m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

string textTypeToStr(modelHandle::textureType type)
{
    string str;
    switch (type)
    {
    case modelHandle::textureType::ambient:
        str = "texture_ambient";
        break;
    case modelHandle::textureType::diffuse:
        str = "texture_diffuse";
        break;
    case modelHandle::textureType::specular:
        str = "texture_specular";
        break;
    case modelHandle::textureType::shininess:
        str = "texture_shininess";
        break;
    case modelHandle::textureType::normals:
        str = "texture_normals";
        break;
    case modelHandle::textureType::height:
        str = "texture_height";
        break;
    case modelHandle::textureType::undefined:
        break;
    default:
        break;
    }
    return str;
}



