#pragma once

#include<string>
namespace global
{
    typedef const void* OpenGLDataArrayPtr;
    typedef OpenGLDataArrayPtr DataPtr;
    typedef char BYTE;

    static const float PI = 3.1415926f;
    static const std::string texturePath = "./RES/nanosuit";
    static const std::string vertexShaderPath = "./SHADERS/shader.vs";
    static const std::string fragmentShaderPath = "./SHADERS/shader.frag";
    static const std::string modelPath = "./RES/nanosuit/";
    static const std::string modelName = "nanosuit.obj";
    enum class Result
    {
        eOk, eErr
    };
    unsigned int UnsignedIntGenerator();
}