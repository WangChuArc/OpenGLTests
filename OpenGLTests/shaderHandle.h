#pragma once
#include<iostream>
#include<stdlib.h>
#include<memory>
#include<fstream>
#include<sstream>
#include<vector>
#include<list>
#include<map>
#define GLEW_STATIC
#include"./libs/glew-1.13.0/include/GL/glew.h"

#include"global.h"
using namespace std;

const GLuint gl_uint_max = (GLuint)-1;

namespace shaderHandle
{
    enum class shaderType { eVertexShader, eFragmentShader, eGeometryShader, eTessControlShader, eTessEvalShader };

    template<shaderType TType>
    class shader {};

    class shaderBase
    {
    public:
        typedef shaderBase myType;

    protected:
        shaderBase();
        shaderBase(string& shaderPath);
        ~shaderBase();

    public:
        GLboolean operator == (const myType&) const;
        template<shaderType T>
        GLboolean operator == (const shader<T>& shaderObj) const { return false; };
        GLboolean operator == (GLuint shaderID) const { return getShaderID() == shaderID; };
        GLboolean bindShaderStr(string&);
        GLboolean setShaderFromPath(const string&);
        GLboolean hasShaderStr() const;
        GLuint getShaderID() const;
        GLboolean compileShader(); 
		GLboolean isShaderIdCreated() const;

    private:
        void _setShader(string&);
        void _error_log(string);
		void _error_log(vector<char>);
		void checkShaderError();

    public:

    protected:
        string m_shader;
        GLuint m_shaderID;

    };

    template<>
    class shader<shaderType::eVertexShader> : public shaderBase
    {
    public:
        typedef shader<shaderType::eVertexShader> myType;
        typedef shaderBase baseType;
        enum { eType = shaderType::eVertexShader };
        shader();
        shader(string& shaderPath);

    private:
        GLboolean operator == (const myType&) const;
    };

    template<>
    class shader<shaderType::eFragmentShader> : public shaderBase
    {
    public:
        typedef shader<shaderType::eFragmentShader> myType;
        typedef shaderBase baseType;
        enum { eType = shaderType::eFragmentShader };
        shader();
        shader(string& shaderPath);

    private:
        GLboolean operator == (const myType&) const;
    };

	class program
	{
	public:
		program();
		program(shader<shaderType::eVertexShader>& vertexShader, shader<shaderType::eFragmentShader>& fragmentShader);
		program(shader<shaderType::eVertexShader>& vertexShader,
                shader<shaderType::eFragmentShader>& fragmentShader,
                shader<shaderType::eGeometryShader>& geometryShader,
                shader<shaderType::eTessControlShader>& tessControlShader,
                shader<shaderType::eTessEvalShader>& tessEvalShader);
		~program();

        template<shaderType TType>
		GLboolean attach(shader<TType> &shaderObj);
        GLboolean attach(GLuint shaderID);
        template<shaderType TType>
		GLboolean alreadyHave(shader<TType> &shaderObj);
        GLboolean alreadyHave(GLuint shaderID);
		GLboolean linkProgram();
        template<shaderType TType>
		GLboolean detach(shader<TType> &shaderObj);
		GLboolean detach(GLuint shaderID);
        //GLboolean erase(shader &shaderObj);
        //GLboolean erase(GLuint shaderID);
		GLboolean use();
        GLuint getProgramID();
        GLuint createUnifLoca(string uniName);
        GLuint getUniform(string uniName);
        GLboolean uniformEmpty();
        vector<string>&& uniforms_str();

	private:
		GLboolean checkProgram();
		void _error_log(string str);
		void _error_log(vector<char> charVec);

	public:

	private:
		GLuint m_programID;
        vector<GLuint> m_shaderIDVec;
        map<string, GLuint> m_uniform;
	};
}

namespace shaderHandle
{
    using namespace std;

    /*template<shaderType TType>
    GLboolean program::attach(shader<TType> &shaderObj)
    {
        return attach(shaderObj.getShaderID());
    }

    template<shaderType TType>
    GLboolean program::detach(shader<TType> &shaderObj)
    {
        return detach(shaderObj.getShaderID());
    }
    inline shaderBase::shaderBase()
    {
        m_shaderID = _initShaderID();
    }

    inline shaderBase::shaderBase(string& shaderPath)
    {
        m_shaderID = _initShaderID();
        setShaderFromPath(shaderPath);
    }

    inline shaderBase::~shaderBase()
    {
        if (isShaderIdCreated()) glDeleteShader(m_shaderID);
    }

    inline GLuint shaderBase::_initShaderID()
    {
        return global::UnsignedIntGenerator();
    } 
    
    inline program::~program()
    {
        glUseProgram(0);
        glDeleteProgram(getProgramID());
    }

    inline GLuint program::getProgramID()
    {
        return m_programID;
    }*/



    inline GLboolean shader<shaderType::eVertexShader>::operator==(const myType & shaderObj) const
    {
        if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
        else return GL_FALSE;
    }

    inline shaderBase::shaderBase()
    {
        m_shaderID = global::UnsignedIntGenerator();
    }

    inline shaderBase::shaderBase(string& shaderPath) : shaderBase()
    {
        setShaderFromPath(shaderPath);
    }

    inline shader<shaderType::eVertexShader>::shader()
    {
        m_shaderID = glCreateShader(GL_VERTEX_SHADER);
    }

    inline shader<shaderType::eVertexShader>::shader(string& shaderPath) : shader()
    {
        setShaderFromPath(shaderPath);
    }

    inline shader<shaderType::eFragmentShader>::shader()
    {
        m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    }

    inline shader<shaderType::eFragmentShader>::shader(string& shaderPath) : shader()
    {
        setShaderFromPath(shaderPath);
    }

    inline shaderBase::~shaderBase()
    {
        if (isShaderIdCreated()) glDeleteShader(m_shaderID);
    }

    inline GLboolean shaderBase::operator == (const myType& shaderObj) const
    {
        if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
        else return GL_FALSE;
    }

    inline GLboolean shader<shaderType::eFragmentShader>::operator == (const myType& shaderObj) const
    {
        if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
        else return GL_FALSE;
    }

    inline void shaderBase::_setShader(string& str)
    {
        m_shader = str;
        const GLchar* const shaderStr = m_shader.c_str();
        glShaderSource(m_shaderID, 1, &shaderStr, NULL);
    }

    inline GLboolean shaderBase::hasShaderStr() const
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

    inline GLboolean shaderBase::setShaderFromPath(const string &strArg)
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
        const GLchar* const shaderStr = m_shader.c_str();
        glShaderSource(m_shaderID, 1, &shaderStr, NULL);
        return GL_TRUE;
    }

    inline GLboolean shaderBase::bindShaderStr(string& str)
    {
        if (str.empty())
        {
            cout << "loaded shader string is empty  " << str << endl;
            return GL_FALSE;
        }
        _setShader(str);
        return GL_TRUE;
    }

    inline GLuint shaderBase::getShaderID() const
    {
        return m_shaderID;
    }

    inline GLboolean shaderBase::isShaderIdCreated() const
    {
        return glIsShader(m_shaderID);
    }

    inline GLboolean shaderBase::compileShader()
    {
        if (m_shader.empty())
        {
            _error_log("no shader string");
            return GL_FALSE;
        }
        glCompileShader(m_shaderID);
        checkShaderError();
        return GL_TRUE;
    }

    inline void shaderBase::checkShaderError()
    {
        GLint result = GL_FALSE;
        GLint infoLogLength = 0;
        glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            int i = 0;
            char* ProgramErrorMessage = new char[infoLogLength + 1];
            ProgramErrorMessage[infoLogLength] = '\0';
            glGetShaderInfoLog(m_shaderID, infoLogLength + 1, &i, ProgramErrorMessage);
            _error_log(ProgramErrorMessage);
            delete[] ProgramErrorMessage;
        }
    }

    inline void shaderBase::_error_log(string str)
    {
        cout << str << endl;
    }

    inline void shaderBase::_error_log(vector<char> charVec)
    {
        cout << &charVec[0] << endl;
    }

    inline program::program() {
        m_programID = glCreateProgram();
    };

    inline program::program(shader<shaderType::eVertexShader>& vertexShader, shader<shaderType::eFragmentShader>& fragmentShader) :m_programID(glCreateProgram())
    {
        attach(vertexShader);
        attach(fragmentShader);
    };

    inline program::~program()
    {
        glUseProgram(0);
        glDeleteProgram(getProgramID());
    }

    template<shaderType TType>
    GLboolean program::attach(shader<TType> &shaderObj)
    {
        return attach(shaderObj.getShaderID());
    }

    inline GLboolean program::attach(GLuint shaderID)
    {
        if (!alreadyHave(shaderID))
            m_shaderIDVec.push_back(shaderID);
        glAttachShader(m_programID, shaderID);
        return GL_TRUE;
    }

    template<shaderType TType>
    GLboolean program::detach(shader<TType> &shaderObj)
    {
        return detach(shaderObj.getShaderID());
    }

    inline GLboolean program::detach(GLuint shaderID)
    {
        if (!glIsShader(shaderID))
        {
            _error_log("shaderID to detach is not valid");
            return GL_FALSE;
        }
        for (auto it = m_shaderIDVec.begin(); it != m_shaderIDVec.end(); it++)
        {
            if (*it == shaderID)
            {
                glDetachShader(m_programID, shaderID);
                m_shaderIDVec.erase(it);
                break;
            }
        }
        return GL_TRUE;
    }


    inline GLboolean program::use()
    {
        glUseProgram(m_programID);
        return GL_TRUE;
    }

    inline GLboolean program::linkProgram()
    {
        glLinkProgram(m_programID);
        m_uniform.clear();
        return checkProgram();
    }

    inline GLboolean program::checkProgram()
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


    template<shaderType TType>
    GLboolean program::alreadyHave(shader<TType> &shader)
    {
        return alreadyHave(shader.getShaderID());
    }

    inline GLboolean program::alreadyHave(GLuint shaderID)
    {
        for (auto it : m_shaderIDVec)
        {
            if (it == shaderID) return GL_TRUE;
        }
        return GL_FALSE;
    }
    inline GLuint program::getProgramID()
    {
        return m_programID;
    }

    inline GLuint program::createUnifLoca(string uniName)
    {
        GLuint uniID = glGetUniformLocation(getProgramID(), uniName.data());
        m_uniform[uniName] = uniID;
        return uniID;
    }

    inline GLuint program::getUniform(string uniName)
    {
        return m_uniform[uniName];
    }

    inline GLboolean program::uniformEmpty()
    {
        return m_uniform.empty() ? GL_TRUE : GL_FALSE;
    }

    inline vector<string>&& program::uniforms_str()
    {
        vector<string> rst(m_uniform.size());
        for (auto& it : m_uniform)
        {
            rst.push_back(it.first);
        }
        return move(rst);
    }

    inline void program::_error_log(string str)
    {
        cout << str << endl;
    }

    inline void program::_error_log(vector<char> charVec)
    {
        cout << &charVec[0] << endl;
    }
}