#include"modelHandle.h"
#include<algorithm>
#include<memory>
#include<iostream>
#include<list>
#include<math.h>
#include<limits>

// debug
#include<assert.h>

namespace modelHandle
{

    SceneWrapper::SceneWrapper(Scene & scene)
    {
        uploadTexture(scene);
        m_pRootNode = make_shared<NodeWrapper>(*(scene.m_rootNode), *this);
    }

    void SceneWrapper::uploadTexture(Scene & scene)
    {
        for (auto& it : scene.m_TexturePtrs)
            m_textures.push_back(make_shared<TextureWrapper>(*it));
    }
    
    void TextureWrapper::TextureFromFile(string& name)
        {
            //Generate texture ID and load texture data 
            glGenTextures(1, &m_textureID);
            int width, height;
            unsigned char* image = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
            // Assign texture to ID
            glBindTexture(GL_TEXTURE_2D, m_textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            SOIL_free_image_data(image);
        }

    NodeWrapper::NodeWrapper(Node & node, SceneWrapper& scene)
    {
        for(auto it : node.m_MeshesPtrVec)
        {
            m_Meshes.push_back(make_shared<MeshWrapper>(*it, scene));
        }

        for (auto& it : node.m_ChildrenPtrVec)
        {
            m_Children.push_back(make_shared<NodeWrapper>(*it, scene));
        }
    }

    MeshWrapper::MeshWrapper(Mesh & mesh, SceneWrapper& scene)
    {
        const size_t position_size = sizeof(Vertex::position);
        const size_t normal_size = sizeof(Vertex::normal);
        const size_t textCoord_size = sizeof(Vertex::textCoord);
        const size_t stride = sizeof(Vertex);
        const size_t vertexNum = mesh.m_vertexVec.size(); m_vertexNum = vertexNum;
        const size_t data_length = stride * vertexNum;
        const size_t indexNum = mesh.m_indexVec.size(); m_elementNum = indexNum;
        const size_t index_length =sizeof(Mesh::indexType) * indexNum;
        global::DataPtr data = reinterpret_cast<global::DataPtr>(mesh.m_vertexVec.data()->data);
        global::DataPtr elemData = reinterpret_cast<global::DataPtr>(mesh.m_indexVec.data());
        size_t offset = 0;

        GLuint VAO, VBO, EBO, textureID;

        /* VAO */
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        /* VBO */
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data_length, data, GL_STATIC_DRAW);
        m_buffers.push_back(VBO);

        /* EBO */
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_length, elemData, GL_STATIC_DRAW);
        m_buffers.push_back(EBO);

        /* 开启几个顶点属性 */
        glVertexAttribPointer(0, position_size / sizeof(decltype(Vertex::position)::type), GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
        glEnableVertexAttribArray(0);
        offset += position_size;

        glVertexAttribPointer(1, normal_size / sizeof(decltype(Vertex::normal)::type), GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
        glEnableVertexAttribArray(1);
        offset += normal_size;

        glVertexAttribPointer(2, textCoord_size / sizeof(decltype(Vertex::textCoord)::type), GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
        glEnableVertexAttribArray(2);

        /* texture */
        for (auto& it : mesh.m_textureVec) /* 内存中的mesh中的texture */
        {
            for (auto& it2 : scene.m_textures) /* 显存中的scene中的texture */
            {
                if (it->name == it2->m_name)
                {
                    m_textures.push_back(TextWrapShared(it2));
                    break;
                }
            }
        }


        /* 绘制命令在这 */
        /*glDrawArrays(GL_TRIANGLES, 0, vertexNum);
        rst = glGetError();
        if (rst != GL_NO_ERROR) cout << "6" << glewGetErrorString(rst) << endl;
        assert(rst == GL_NO_ERROR);

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);*/
        m_VAO = VAO;
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    TextureWrapper::TextureWrapper(Texture & text)
    {
        m_type = text.type;
        m_name = text.name;
        TextureFromFile(text.path + text.name);
    }

}