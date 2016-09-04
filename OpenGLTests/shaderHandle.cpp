#include "shaderHandle.h"
#include <stdlib.h>
#include <iostream>
//#define GLEW_STATIC
//#include"glew.h"
using namespace shaderHandle;
using namespace std;

shader::shader() : m_shaderType(shaderType::eNoShader), m_shaderID(gl_uint_max)
{
    // do nothing
}

shader::shader(string shaderPath, shaderType eType) : m_shaderType(shaderType::eNoShader), m_shaderID(gl_uint_max)
{
    setShaderFromPath(shaderPath, eType);
    if (!(eType == shaderType::eNoShader))
    {
        _setShaderType(eType);
    }
}

shader::~shader()
{
	if (isShaderIdCreated()) glDeleteShader(m_shaderID);
}

GLboolean shader::operator == (shader& shaderObj)
{
	if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
	else return GL_FALSE;
}

void shader::_setShader(string str)
{
    m_shader = str;
}

GLboolean shader::hasShaderStr()
{
    if (m_shader.empty())
    {
        return GL_FALSE;
    }
    else
    {
        return GL_TRUE;
    }
}

GLboolean shader::setShaderFromPath(const string &strArg, shaderType eType)
{
    ifstream fileStream(strArg, ios::in, ios::binary);
	string charVec;
    if (!fileStream.is_open())
    {
        cout << "fail to open shader file  " << strArg.c_str() << endl;
        return GL_FALSE;
    }
    m_shader.assign(istreambuf_iterator<char>(fileStream.rdbuf()), istreambuf_iterator<char>());
	fileStream.close();
    if (!(eType == shaderType::eNoShader))
    {
        _setShaderType(eType);
    }
    return GL_TRUE;
}

GLboolean shader::bindShaderStr(string str, shaderType eType)
{
    if (str.empty())
    {
        cout << "loaded shader string is empty  " << str << endl;
        return GL_FALSE;
    }
    _setShader(str);
    if (!(eType == shaderType::eNoShader))
    {
        _setShaderType(eType);
    }
    return GL_TRUE;
}

GLboolean shader::setShaderType(shaderType eType)
{
    return _setShaderType(eType);
}

GLboolean shader::_setShaderType(shaderType eType)
{
    m_shaderType = eType;
	return GL_TRUE;
}

shaderType shader::getShaderType()
{
    return _getShaderType();
}

shaderType shader::_getShaderType()
{
    return m_shaderType;
}

/*
GLuint shader::getShaderID()
{
    return _getShaderID();
}
*/
GLuint shader::getShaderID()
{
    return m_shaderID;
}

GLboolean shader::isShaderIdCreated()
{
	return glIsShader(m_shaderID);
}

GLboolean shader::createShaderID(overwriteType eType)
{
	if (isShaderIdCreated())
	{
		if(eType == overwriteType::eAbandon)
		{
			return GL_TRUE;
		}
		else if (m_shaderType == shaderType::eNoShader)
		{
			_error_log("shader type is not determined");
			return GL_FALSE;
		}
		else
		{
			glDeleteShader(m_shaderID);
			return GL_TRUE;
		}
	}
	else
	{
		switch (m_shaderType)
		{
		case shaderType::eNoShader:
			_error_log("shader type is not determined");
			return GL_FALSE;

		case shaderType::eVertexShader:
			m_shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;

		case shaderType::eFragmentShader:
			m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;

		case shaderType::eGeometryShader:
			m_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
			break;

		case shaderType::eTessControlShader:
			m_shaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;

        case shaderType::eTessEvalShader:
            m_shaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;

		default:
			_error_log("something wrong with shaderType");
			return GL_FALSE;
		}
		return GL_TRUE;
	}
}

GLboolean shader::compileShader()
{
    if (m_shader.empty())
    {
        _error_log("no shader string");
        return GL_FALSE;
    }
	createShaderID();
    const GLchar* const shaderStr = m_shader.c_str();
    glShaderSource(m_shaderID, 1, &shaderStr, NULL);
    glCompileShader(m_shaderID);
    checkShader();
    return GL_TRUE;
}

void shader::checkShader()
{
    GLint result = GL_FALSE;
	GLint infoLogLength = 0;
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		//std::vector<char> ProgramErrorMessage(infoLogLength + 1);
        int i = 0;
        char* ProgramErrorMessage = new char[infoLogLength + 1];
        ProgramErrorMessage[infoLogLength] = '\0';
		glGetShaderInfoLog(m_shaderID, infoLogLength+1, &i, ProgramErrorMessage);
		_error_log(ProgramErrorMessage);
        delete[] ProgramErrorMessage;
	}
}

void shader::_error_log(string str)
{
    cout << str << endl;
}

void shader::_error_log(vector<char> charVec)
{
	cout << &charVec[0] << endl;
}

program::program() :m_programID(glCreateProgram()) {};

program::program(GLuint vertexShader, GLuint fragmentShader) :m_programID(glCreateProgram()) {};

program::~program()
{
	GLsizei size = 0;
	vector<GLuint> shaders;
	shaders.resize(m_shaderVec.size());
	glGetAttachedShaders(m_programID, m_shaderVec.size(), &size, &shaders[0]);
	int i = 0;
	for (vector<GLuint>::iterator it = shaders.begin(); i < size; i++)
	{
		glDetachShader(m_programID, *it);
	}
}

GLboolean program::attach(shader &shaderObj)
{
	if (unique(shaderObj))
		m_shaderVec.push_back(&shaderObj);
	glAttachShader(m_programID, shaderObj.getShaderID());
	return GL_TRUE;
}

GLboolean program::detach(shader &shaderObj)
{
	return detach(shaderObj.getShaderID());
}

GLboolean program::detach(GLuint shaderID)
{
	if (!glIsShader(shaderID))
	{
		_error_log("detach shaderID is not valid");
		return GL_FALSE;
	}
	for (vector<shader*>::iterator it = m_shaderVec.begin(); it != m_shaderVec.end(); it++)
	{
		if ((**it).getShaderID() == shaderID)
		{
			glDetachShader(m_programID, shaderID);
			return GL_TRUE;
		}
	}
	_error_log("detach shaderID not found");
	return GL_FALSE;
}

GLboolean program::use()
{
	glUseProgram(m_programID);
	return GL_TRUE;
}

GLboolean program::linkProgram()
{
	glLinkProgram(m_programID);
	return checkProgram();
}

GLboolean program::checkProgram()
{
	GLint infoLogLength = 0;
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) 
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &programErrorMessage[0]);
		_error_log(programErrorMessage);
	}
	GLint result = GL_TRUE;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	return result;
}

GLboolean program::unique(shader &shaderObj)
{
	for (vector<shader*>::iterator it = m_shaderVec.begin(); it != m_shaderVec.end(); it++)
	{
		if (shaderObj == **it) return GL_FALSE;
	}
	return GL_TRUE;
}
GLuint program::getProgramID() 
{
    return m_programID;
}

void program::_error_log(string str)
{
	cout << str << endl;
}

void program::_error_log(vector<char> charVec)
{
	cout << &charVec[0] << endl;
}