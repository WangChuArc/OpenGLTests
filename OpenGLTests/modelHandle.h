#pragma once
#include"glew.h"
#include<vector>
#include<algorithm>

using namespace std;

namespace modelHandle
{
    struct vec2
    {
        float x;
        float y;
    vec2() :x(0.0f), y(0.0f) {};
    };

    struct vec3
    {
        float x;
        float y;
        float z;
        vec3() :x(0.0f), y(0.0f), z(0.0f) {};
    };

    enum class textureType{ eDiffuse, eSpecular };

    struct vertex
    {
        vec3 position;
        vec3 normal;
        vec2 texcoords;
    };

    struct texture
    {
        GLuint textID;
        textureType type;
    };

    class mesh
    {
    public:
        vector<vertex> m_vertexs;
        vector<texture> m_textures;
        vector<GLuint> m_indices;
    public:
        mesh();
        ~mesh();
        void set_vertexs(vector<vertex>&& vec) { m_vertexs = vec; }
        void set_texture(vector<texture>&& vec) { m_textures = vec; }
        void set_indices(vector<GLuint>&& vec) { m_indices = vec; }

    private:

    };

    mesh::mesh()
    {
    }

    mesh::~mesh()
    {
        m_vertexs.clear();
        m_textures.clear();
        m_indices.clear();
    }
}