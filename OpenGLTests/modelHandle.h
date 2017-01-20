#pragma once
#ifndef _MODEL_HANDLE_

#define _MODEL_HANDLE_

#define GLEW_STATIC
#include<vector>
#include"libs/glew-1.13.0/include/GL/glew.h"
#include"libs/glfw-3.2.1/include/x86/glfw3.h"
#include"libs/SOIL/SOIL.h"
#include<map>
#include"shaderHandle.h"
#include<algorithm>
#include<memory>
#include<iostream>
#include<list>
#include<math.h>
#include<limits>
#include"global.h"
extern GLFWwindow* pWindow;

namespace modelHandle
{
    using namespace std;

    /* 加载模型的库注册 */
    namespace Lib
    {
        struct LibListHead {};
#define LIB_LIST_HEAD LibListHead
#define LIB_LIST_TAIL LibListHead

        template<typename T>
        struct Liblist {};

        template<>
        struct Liblist<LIB_LIST_HEAD> { typedef void preType; };

#define ADD_TO_LIB_LIST(x) struct x {}; template<>struct Liblist<x> { typedef LIB_LIST_TAIL preType; };
    }

    /* 保存模型数据的类和一些定义 */

    template<typename T, size_t Length>
    struct vec
    {
        typedef T type;
        T data[Length];
        T& operator[] (int i) { return data[i]; }
    };


    typedef vec<GLfloat, 4> vec4;
    typedef vec<GLfloat, 3> vec3;
    typedef vec<GLfloat, 2> vec2;
    
    struct Texture;  struct Scene; struct Node; struct Mesh;
    typedef shared_ptr<Texture>  TextureShared;
    typedef shared_ptr<Scene>    SceneShared;
    typedef shared_ptr<Node>     NodeShared;
    typedef shared_ptr<Mesh>     MeshShared;

    extern vector<SceneShared> sceneVec;

    struct Vertex
    {
        Vertex() {};
        union
        {
            struct
            {
                vec3 position;
                vec3 normal;
                vec2 textCoord;
            };
            global::BYTE data[sizeof(vec3) + sizeof(vec3) + sizeof(vec2)];
        };
    };
    static_assert(!is_pod<Vertex>::value, "Vertex should not be a POD");
    static_assert(is_standard_layout<Vertex>::value, "Vertex should be standard_layout");


    enum class textureType { ambient, diffuse, specular, shininess, normals, height, undefined };

    struct Texture
    {
        Texture(textureType t, string&& s_n, const string& s_p) : type(t), name(s_n), path(s_p) {};
        textureType type;
        string name;
        // 用于存储材质文件目录的地址
        string path;
    };

    struct Mesh
    {
        typedef int indexType;

        const GLuint m_ID;
        vector<Vertex> m_vertexVec;
        vector<indexType> m_indexVec;
        vector<TextureShared> m_textureVec;
        shared_ptr<shaderHandle::program> m_pShader;

        Mesh() : m_ID(generate_ID()), m_pShader(nullptr) { };

        Mesh(Mesh&&) = default;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        void setShaderProg(shared_ptr<shaderHandle::program> p) { m_pShader = p; };
    private:
        static unsigned int generate_ID() { return global::UnsignedIntGenerator(); };
    };

    struct Node
    {
        const GLuint m_ID;
        Node* m_pParent;
        vector<NodeShared> m_ChildrenPtrVec;
        vector<MeshShared> m_MeshesPtrVec;

        Node() : m_ID(generate_ID()), m_pParent(nullptr) { };

        Node(Node&&) = default;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

    private:
        static unsigned int generate_ID() { return global::UnsignedIntGenerator(); };
    };

    struct Scene
    {
        NodeShared m_rootNode;
        vector<TextureShared> m_TexturePtrs;

        Scene() = default;
        Scene(Scene&&) = default;

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
    };


    template<typename T>
    class TraitHelper {};

    template<typename T>
    class ModelFactory {};

    class SceneWrapper;     typedef shared_ptr<SceneWrapper> SceneWrapShared;
    class NodeWrapper;      typedef shared_ptr<NodeWrapper> NodeWrapShared;
    class MeshWrapper;      typedef shared_ptr<MeshWrapper> MeshWrapShared;
    class TextureWrapper;   typedef shared_ptr<TextureWrapper> TextWrapShared;

    class SceneWrapper
    {
    public:
        SceneWrapper(Scene& scene);
        void uploadTexture(Scene& scene);

        NodeWrapShared m_pRootNode;
        vector<TextWrapShared> m_textures;
    };

    class NodeWrapper
    {
    public:
        NodeWrapper(Node&, SceneWrapper&);

        vector<NodeWrapShared> m_Children;
        vector<MeshWrapShared> m_Meshes;
    };

    class MeshWrapper
    {
    public:
        MeshWrapper(Mesh&, SceneWrapper&);
        ~MeshWrapper() { glDeleteVertexArrays(1, &m_VAO); glDeleteBuffers(m_buffers.size(), m_buffers.data()); };

        GLuint m_VAO;
        int m_vertexNum;
        int m_elementNum;
        vector<TextWrapShared> m_textures;
        vector<GLuint> m_buffers;
    };

    class TextureWrapper
    {
    public:
        TextureWrapper(Texture&);
        void TextureFromFile(string& name);

        textureType m_type;
        GLuint m_textureID;
        string m_name;
    };
}

/////////////////////////////////////
//  Assimp Lib instance declare
/////////////////////////////////////
namespace modelHandle
{
    


}

/////////////////////////////////////////
//  some template implementation
/////////////////////////////////////////
//#include"modelHandle.tpp"

namespace modelHandle
{


}

#endif // !_MODEL_HANDLE_