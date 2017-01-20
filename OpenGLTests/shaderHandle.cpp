//
//#include"shaderHandle.h"
//namespace shaderHandle
//{
//    using namespace std;
//
//
//    GLuint shader<shaderType::eVertexShader>::_initShaderID()
//    {
//        return glCreateShader(GL_VERTEX_SHADER);
//    }
//
//    inline GLboolean shader<shaderType::eVertexShader>::operator==(const myType & shaderObj) const
//    {
//        if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
//        else return GL_FALSE;
//    }
//
//    GLuint shader<shaderType::eFragmentShader>::_initShaderID()
//    {
//        return glCreateShader(GL_FRAGMENT_SHADER);
//    }
//
//    inline GLboolean shaderBase::operator == (const myType& shaderObj) const
//    {
//        if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
//        else return GL_FALSE;
//    }
//
//    inline GLboolean shader<shaderType::eFragmentShader>::operator == (const myType& shaderObj) const
//    {
//        if (getShaderID() == shaderObj.getShaderID()) return GL_TRUE;
//        else return GL_FALSE;
//    }
//
//    inline void shaderBase::_setShader(string& str)
//    {
//        m_shader = str;
//    }
//
//    inline GLboolean shaderBase::hasShaderStr() const
//    {
//        if (m_shader.empty())
//        {
//            return GL_FALSE;
//        }
//        else
//        {
//            return GL_TRUE;
//        }
//    }
//
//    inline GLboolean shaderBase::setShaderFromPath(const string &strArg)
//    {
//        ifstream fileStream(strArg, ios::in, ios::binary);
//        string charVec;
//        if (!fileStream.is_open())
//        {
//            cout << "fail to open shader file  " << strArg.c_str() << endl;
//            return GL_FALSE;
//        }
//        m_shader.assign(istreambuf_iterator<char>(fileStream.rdbuf()), istreambuf_iterator<char>());
//        fileStream.close();
//        return GL_TRUE;
//    }
//
//    inline GLboolean shaderBase::bindShaderStr(string& str)
//    {
//        if (str.empty())
//        {
//            cout << "loaded shader string is empty  " << str << endl;
//            return GL_FALSE;
//        }
//        _setShader(str);
//        return GL_TRUE;
//    }
//
//    GLuint shaderBase::getShaderID() const
//    {
//        return m_shaderID;
//    }
//
//    GLboolean shaderBase::isShaderIdCreated() const
//    {
//        return glIsShader(m_shaderID);
//    }
//
//    GLboolean shaderBase::compileShader()
//    {
//        if (m_shader.empty())
//        {
//            _error_log("no shader string");
//            return GL_FALSE;
//        }
//        const GLchar* const shaderStr = m_shader.c_str();
//        glShaderSource(m_shaderID, 1, &shaderStr, NULL);
//        glCompileShader(m_shaderID);
//        checkShaderError();
//        return GL_TRUE;
//    }
//
//    void shaderBase::checkShaderError()
//    {
//        GLint result = GL_FALSE;
//        GLint infoLogLength = 0;
//        glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &result);
//        glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
//        if (infoLogLength > 0) {
//            int i = 0;
//            char* ProgramErrorMessage = new char[infoLogLength + 1];
//            ProgramErrorMessage[infoLogLength] = '\0';
//            glGetShaderInfoLog(m_shaderID, infoLogLength + 1, &i, ProgramErrorMessage);
//            _error_log(ProgramErrorMessage);
//            delete[] ProgramErrorMessage;
//        }
//    }
//
//    void shaderBase::_error_log(string str)
//    {
//        cout << str << endl;
//    }
//
//    void shaderBase::_error_log(vector<char> charVec)
//    {
//        cout << &charVec[0] << endl;
//    }
//
//    inline program::program() {
//        m_programID = glCreateProgram();
//    };
//
//    inline program::program(shader<shaderType::eVertexShader>& vertexShader, shader<shaderType::eFragmentShader>& fragmentShader) :m_programID(glCreateProgram())
//    {
//        attach(vertexShader);
//        attach(fragmentShader);
//    };
//
//
//
//    inline GLboolean program::attach(GLuint shaderID)
//    {
//        if (!alreadyHave(shaderID))
//            m_shaderIDVec.push_back(shaderID);
//        glAttachShader(m_programID, shaderID);
//        return GL_TRUE;
//    }
//
//    inline GLboolean program::detach(GLuint shaderID)
//    {
//        if (!glIsShader(shaderID))
//        {
//            _error_log("shaderID to detach is not valid");
//            return GL_FALSE;
//        }
//        for (auto it = m_shaderIDVec.begin(); it != m_shaderIDVec.end(); it++)
//        {
//            if (*it == shaderID)
//            {
//                glDetachShader(m_programID, shaderID);
//                m_shaderIDVec.erase(it);
//            }
//        }
//        return GL_TRUE;
//    }
//
//
//    inline GLboolean program::use()
//    {
//        glUseProgram(m_programID);
//        return GL_TRUE;
//    }
//
//    inline GLboolean program::linkProgram()
//    {
//        glLinkProgram(m_programID);
//        m_uniform.clear();
//        return checkProgram();
//    }
//
//    inline GLboolean program::checkProgram()
//    {
//        GLint infoLogLength = 0;
//        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
//        if (infoLogLength > 0)
//        {
//            std::vector<char> programErrorMessage(infoLogLength + 1);
//            glGetProgramInfoLog(m_programID, infoLogLength, NULL, &programErrorMessage[0]);
//            _error_log(programErrorMessage);
//        }
//        GLint result = GL_TRUE;
//        glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
//        return result;
//    }
//
//
//    template<shaderType TType>
//    GLboolean program::alreadyHave(shader<TType> &shader)
//    {
//        return alreadyHave(shader.getShaderID());
//    }
//
//    inline GLboolean program::alreadyHave(GLuint shaderID)
//    {
//        for (auto it : m_shaderIDVec)
//        {
//            if (it == shaderID) return GL_TRUE;
//        }
//        return GL_FALSE;
//    }
//
//    inline GLuint program::createUnifLoca(string uniName)
//    {
//        GLuint uniID = glGetUniformLocation(getProgramID(), uniName.data());
//        m_uniform[uniName] = uniID;
//        return uniID;
//    }
//
//    inline GLuint program::getUniform(string uniName)
//    {
//        return m_uniform[uniName];
//    }
//
//    inline GLboolean program::uniformEmpty()
//    {
//        return m_uniform.empty() ? GL_TRUE : GL_FALSE;
//    }
//
//    inline vector<string>&& program::uniforms_str()
//    {
//        vector<string> rst(m_uniform.size());
//        for (auto& it : m_uniform)
//        {
//            rst.push_back(it.first);
//        }
//        return move(rst);
//    }
//
//    inline void program::_error_log(string str)
//    {
//        cout << str << endl;
//    }
//
//    inline void program::_error_log(vector<char> charVec)
//    {
//        cout << &charVec[0] << endl;
//    }
//}
