#pragma once
#include<stdlib.h>
#define GLEW_STATIC
#include"libs/glew-1.13.0/include/GL/glew.h"
#include"libs/glfw-3.1.2/include/GLFW/glfw3.h"
#include<functional>
#include<vector>
#include<iostream>
#include"shaderHandle.h"
#include"libs/FreeImage/Dist/x32/FreeImage.h"
#include"matrixHandle.h"
#include<cmath>
#include"libs/lib3ds/lib3ds.h"
#include"libs/assimp/include/Importer.hpp"
#include"libs/assimp/include/scene.h"
#include"libs/assimp/include/postprocess.h"
#include"global.h"
GLFWwindow* pWindow;
void winResizeFunc(GLFWwindow*, int, int);
void winMouseMoveFunc(GLFWwindow*, double, double);
void winBtnClickFunc(GLFWwindow* p, int button, int state, int key);
void loadModel(string path);
void processNode(aiScene& scene);
float imageRatio = 1.0f;
bool leftButtonDown = false;
matrix_3x3 modelMat(1);
matrix_3x3 mouseMat(1);
typedef std::vector<std::function<void()>> FuncVec;

struct Pos_2D
{
    int x;
    int y;
} mousePos = {0, 0};

void inline checkMaterials(Lib3dsFile* pFile, int n);
global::Result init();

int main(void)
{
    typedef shaderHandle::shader shader;
    shader vertexShader("SHADERS//ri.vertexshader", shaderType::eVertexShader);
    shader fragmentShader("SHADERS//tri.fragmentshader", shaderType::eFragmentShader);
    shader geometeryShader("SHADERS//tri.geometeryshader", shaderType::eGeometryShader);
    //int rst = 0;
    vertexShader.compileShader();
    fragmentShader.compileShader();
    geometeryShader.compileShader();

    shaderHandle::program program;
    program.attach(vertexShader);
    program.attach(fragmentShader);
    program.attach(geometeryShader);
    program.linkProgram();
    program.use();
    program.detach(vertexShader);
    program.detach(fragmentShader);
    program.detach(geometeryShader);
    //program.delete(vertexShader);
    //program.delete(fragmentShader);
    GLuint textUnit, modelMatUnif, ambient, diffuse, specular, shininess, lightDirect;
    textUnit = glGetUniformLocation(program.getProgramID(), "sampler");
    modelMatUnif = glGetUniformLocation(program.getProgramID(), "modelMat");
    ambient = glGetUniformLocation(program.getProgramID(), "ambient");
    diffuse = glGetUniformLocation(program.getProgramID(), "diffuse");
    specular = glGetUniformLocation(program.getProgramID(), "specular");
    shininess = glGetUniformLocation(program.getProgramID(), "shininess");
    lightDirect = glGetUniformLocation(program.getProgramID(), "lightDirect");


    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("RES//astronautbazze_cosmonaut.3DS", aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "error::assimp" << importer.GetErrorString() << endl;
    }

    FIBITMAP *image = FreeImage_Load(FIF_JPEG, "RES//Arcueid.jpg", JPEG_ACCURATE);
    int imageWidth = FreeImage_GetWidth(image);
    int imageHeight = FreeImage_GetHeight(image);
    imageRatio = (float)imageWidth / (float)imageHeight;
    int imageBPP = FreeImage_GetBPP(image) / 8;
    /*if(!FreeImage_GetChannel(image, FICC_RGB))
    {
        FIBITMAP *newImage = FreeImage_Allocate(imageWidth, imageHeight, 24);
        FreeImage_SetChannel(newImage, image, FICC_RGB);
        FreeImage_Unload(image);
        image = newImage;
    }*/

    //BYTE* imageData = new BYTE[imageWidth * imageHeight * imageBPP];
    BYTE* imageData = FreeImage_GetBits(image);
    GLuint imageBuf;
    glGenBuffers(1, &imageBuf);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, imageBuf);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, (imageWidth * imageHeight * imageBPP), NULL, GL_STREAM_READ);
    BYTE* pMapBuf = (BYTE*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    memcpy(pMapBuf, imageData, sizeof(BYTE)*(imageWidth * imageHeight * imageBPP));
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    FreeImage_Unload(image);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1d(textUnit, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniformMatrix3fv(modelMatUnif, 1, GL_FALSE, &modelMat[0][0]);

    GLuint VBOs[3];
    Lib3dsFile *pFile = lib3ds_file_open("RES//astronaut//bazze_cosmonaut.3DS");
    //float lightPoint[3] = { 600.0f, 600.0f, - 600.0f };
    float lightDirectData[3] = { -600.0f, -600.0f, 600.0f };
    vector_3 ldv(lightDirectData);
    ldv.normalize();
    glUniform3fv(lightDirect, 3, &ldv[0]);
    Lib3dsMesh *pMesh = nullptr;
    Lib3dsFace *pFace = nullptr;
    vector<float> vertexBuf;
    vector<float> textureBuf;
    //vertexBuf.reserve(200000);
    //textureBuf.reserve(200000);
    vertexBuf.reserve(pFile->meshes_size * 1 * 3 * 3);
    textureBuf.reserve(pFile->meshes_size * 1 * 3 * 2);
    const int CNT_PER_TURN = 16;
    glGenBuffers(3, VBOs);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    int old_idx = 0;
   

    auto checkVectorEmpty = [&]()->bool
    {
        if (vertexBuf.empty())
        {
            //vertexBuf.clear();
            textureBuf.clear();
            return false;
        }
        else
        {
            return true;
        }
    };

    auto drawInBuffer = [&]()->void
    {
        if (!checkVectorEmpty()) return;

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, vertexBuf.size()*sizeof(float), &vertexBuf[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, textureBuf.size()*sizeof(float), &textureBuf[0], GL_STREAM_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        modelMat.rotate(1.0f, 0.0f, 0.0f, global::PI);
        modelMat.rotate(0.0f, 1.0f, 0.0f, global::PI);
        modelMat.normalize();
        modelMat*0.002f;
        glUniformMatrix3fv(modelMatUnif, 1, GL_FALSE, &modelMat[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, vertexBuf.size() / 3);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        /*(turns < 2);/* (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(pWindow) == 0);*/
        vertexBuf.clear();
        textureBuf.clear();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    };

    auto drawNSwapMat = [&](int n)->void
    {
        Lib3dsMaterial* pMat = nullptr;
        pMat = *(pFile->materials + n);
        glUniform3fv(ambient, 3, pMat->ambient);
        glUniform3fv(diffuse, 3, pMat->diffuse);
        glUniform3fv(specular, 3, pMat->specular);
        glUniform1f(shininess, pMat->shininess);
        drawInBuffer();
    };

    float max = 0.0f;

    for (int k = 0, cnt = CNT_PER_TURN, turns = 0, meshes_size = pFile->meshes_size; k < 2; ++k)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //meshes_size = 512;
        cnt = CNT_PER_TURN;
        turns = 0;
        //loadMaterials(pFile);

        do
        {
            meshes_size >= cnt ? meshes_size -= cnt : cnt = meshes_size;
            for (int i = 0; i < cnt; ++i)
            {
                pMesh = *(pFile->meshes + i + turns * CNT_PER_TURN);
                if (pMesh == nullptr)
                {
                    cout << "pmesh is nullptr  " << turns << "  " << cnt << "  " << i << endl;
                    continue;
                }
                for (int j = 0; j < pMesh->nfaces; ++j)
                {
                    pFace = &pMesh->faces[j];
                    if (pFace->material != old_idx)
                    {
                        old_idx = pFace->material;
                        if(pFace->material == -1)
                            drawNSwapMat(0);
                        else
                            drawNSwapMat(pFace->material);
                    }
                    vertexBuf.push_back(pMesh->vertices[pFace->index[0]][0]);
                    vertexBuf.push_back(pMesh->vertices[pFace->index[0]][1]);
                    vertexBuf.push_back(pMesh->vertices[pFace->index[0]][2]);
                    //if (max < pMesh->vertices[pFace->index[0]][1]) max = pMesh->vertices[pFace->index[0]][1];

                    vertexBuf.push_back(pMesh->vertices[pFace->index[1]][0]);
                    vertexBuf.push_back(pMesh->vertices[pFace->index[1]][1]);
                    vertexBuf.push_back(pMesh->vertices[pFace->index[1]][2]);
                    //if (max < pMesh->vertices[pFace->index[1]][1]) max = pMesh->vertices[pFace->index[1]][1];

                    vertexBuf.push_back(pMesh->vertices[pFace->index[2]][0]);
                    vertexBuf.push_back(pMesh->vertices[pFace->index[2]][1]);
                    vertexBuf.push_back(pMesh->vertices[pFace->index[2]][2]);
                    //if (max < pMesh->vertices[pFace->index[2]][1]) max = pMesh->vertices[pFace->index[2]][1];

                    if(pMesh->texcos != nullptr)
                    {
                        textureBuf.push_back(pMesh->texcos[pFace->index[0]][0]);
                        textureBuf.push_back(pMesh->texcos[pFace->index[0]][1]);
                        textureBuf.push_back(pMesh->texcos[pFace->index[1]][0]);
                        textureBuf.push_back(pMesh->texcos[pFace->index[1]][1]);
                        textureBuf.push_back(pMesh->texcos[pFace->index[2]][0]);
                        textureBuf.push_back(pMesh->texcos[pFace->index[2]][1]);
                    }
                    else
                    {
                        textureBuf.push_back(0.0f);
                        textureBuf.push_back(0.0f);
                        textureBuf.push_back(0.0f);
                        textureBuf.push_back(0.0f);
                        textureBuf.push_back(0.0f);
                        textureBuf.push_back(0.0f);
                    }
                }
            }
            drawInBuffer();
            turns += 1;
        } while (cnt == CNT_PER_TURN);

        glfwSwapBuffers(pWindow);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    while((glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(pWindow) == 0))
    {
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
   /* GLfloat vertexs[] =
    {
        -1.0f, -1.0f,  1.0f,    //1
         1.0f, -1.0f,  1.0f,    //2
        -1.0f,  1.0f,  1.0f,    //3
         1.0f,  1.0f,  1.0f,    //4
        /*
        -1.0f, -1.0f, -1.0f,    //5
         1.0f, -1.0f, -1.0f,    //6
        -1.0f,  1.0f, -1.0f,    //7
         1.0f,  1.0f, -1.0f,    //8
         1.0f, -1.0f,  1.0f,    //2
         1.0f, -1.0f, -1.0f,    //6
         1.0f,  1.0f,  1.0f,    //4
         1.0f,  1.0f, -1.0f,    //8

         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f

    };
         */
    /*
    GLfloat textPos[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
        
    };
    GLuint elements[] =
    {
        2,  0,  1,
        2,  1,  3,

        6,  4,  5,
        6,  5,  7,

       10,  8,  9,
       10,  9, 11,

       14, 12, 13,
       14, 13, 15,

       18, 16, 17,
       18, 17, 19,

       22, 20, 21,
       22, 21, 23
    };
    */
    
    //cleanUp();
    FreeImage_DeInitialise();
    return 0;
}

void winResizeFunc(GLFWwindow* pWindow, int w, int h)
{
    if(w / h > imageRatio)
        glViewport(0, 0, h*imageRatio, h);
    else
        glViewport(0, 0, w, w/imageRatio);
}

void winMouseMoveFunc(GLFWwindow* p, double x, double y)
{
    if (leftButtonDown)
    {
        float buffer[3];
        vector_3 axis(buffer);
        float deltaX = mousePos.x - x, deltaY = mousePos.y - y;
        // 坐标系不同，y轴是反的
        axis.init(deltaY, deltaX, 0);
        mouseMat * axis;
        /*
        static auto clacAngle = [&x, &y]()->float
        {
            return (x*x + y*y) / 100;
        }
        */
        modelMat * matrix_3x3(axis, (deltaX*deltaX + deltaY*deltaY) / 4000.0f);
    }
    mousePos.x = x; mousePos.y = y;
}

void winBtnClickFunc(GLFWwindow* p, int button, int state, int key)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
    {
        leftButtonDown = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
    {
        leftButtonDown = false;
        mouseMat = modelMat;
    }
    else
        return;
}
void inline checkMaterials(Lib3dsFile* pFile, int n)
{
}

template<typename T>
global::Result initGlfw(T &cleanUpStru);

global::Result init()
{
    using global::Result;
    FuncVec cleanUpFuncs;
    // 定义清理函数
    auto cleanUp = [&cleanUpFuncs]()->void
    {
        //for (FuncVec::iterator it = cleanUpFuncs.begin(); it != cleanUpFuncs.end(); it++) (*it)();
        for (auto func : cleanUpFuncs) { func(); }
    };

    struct CleanUpStru
    {
        function<void()> &cleanUp;
        FuncVec &cleanUpFuncs;
    }cus{function<void()>(cleanUp), cleanUpFuncs};

    initGlfw(cus);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "fail to init \"glew\"" << std::endl;
        cleanUp();
        return Result::eErr;
    }

    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(pWindow, GLFW_DOUBLEBUFFER, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glfwSetWindowSizeCallback(pWindow, winResizeFunc);
    glfwSetMouseButtonCallback(pWindow, winBtnClickFunc);
    glfwSetCursorPosCallback(pWindow, winMouseMoveFunc);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
}
template<typename T>
global::Result initGlfw(T &cleanUpStru)
{
    using global::Result;

    // 初始化
    if (!glfwInit())
    {
        std::cout << "fail to init \"glfw\"" << std::endl;
        cleanUp();
        return Result::eErr;
    }
    else
    {
        cleanUpFuncs.push_back(std::move(std::function<void()>(glfwTerminate)));
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(1024, 768, "An OpenGL window", NULL, NULL);
    if (nullptr == pWindow)
    {
        std::cout << "fail to create a window" << std::endl;
        cleanUp();
        return Result::eErr;
    }

    glfwMakeContextCurrent(pWindow);
}