#pragma once
#include <iostream>
#include <stdlib.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#define GLEW_STATIC
#include"glew.h"
using namespace std;

const GLuint gl_uint_max = (GLuint)-1;

enum class shaderType {eNoShader, eVertexShader, eFragmentShader, eGeometryShader, eTessControlShader, eTessEvalShader};
namespace shaderHandle
{
	enum class overwriteType {eAbandon, eOverwrite};
    class shader
    {
    public:
        shader();
        shader(string shaderPath, shaderType eType = shaderType::eNoShader);
		~shader();
		GLboolean operator == (shader& shaderObj);
        GLboolean bindShaderStr(string, shaderType eType = shaderType::eNoShader);
        GLboolean setShaderFromPath(const string&, shaderType eType = shaderType::eNoShader);
        GLboolean hasShaderStr();
        GLboolean setShaderType(shaderType eType);
        shaderType getShaderType();
        GLuint getShaderID();
        GLboolean compileShader(); 
		GLboolean isShaderIdCreated();
		GLboolean createShaderID(overwriteType eType = overwriteType::eAbandon);

    private:
        void _setShader(string);
        GLboolean _setShaderType(shaderType eType);
        shaderType _getShaderType();
        void _error_log(string);
		void _error_log(vector<char>);
		void checkShader();

    public:

    private:
        string m_shader;
        shaderType m_shaderType;
        GLuint m_shaderID;

    };

	class program
	{
	public:
		program();
		program(GLuint vertexShader, GLuint fragmentShader);
		program(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader, GLuint tessControlShader, GLuint tessEvalShader);
		~program();
		GLboolean attach(shader &shaderObj);
		GLboolean unique(shader &shaderObj);
		GLboolean linkProgram();
		GLboolean detach(shader &shaderObj);
		GLboolean detach(GLuint shaderID);
		GLboolean use();
        GLuint getProgramID();

	private:
		GLboolean checkProgram();
		void _error_log(string str);
		void _error_log(vector<char> charVec);

	public:

	private:
		GLuint m_programID;
		vector<shader*> m_shaderVec;
	};
}