#include"assimpModelLoad.h"

namespace modelHandle
{
    textureType TraitHelper<Lib::assimp>::convertTextType(aiTextureType type)
    {
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            return textureType::diffuse;
        case aiTextureType_SPECULAR:
            return textureType::specular;
        case aiTextureType_AMBIENT:
            return textureType::ambient;
        case aiTextureType_SHININESS:
            return textureType::shininess;
        case aiTextureType_NORMALS:
            return textureType::normals;
        case aiTextureType_HEIGHT:
            return textureType::height;
            /* // 现在还未使用
            case aiTextureType_NONE:
            break;
            case aiTextureType_EMISSIVE:
            break;
            case aiTextureType_OPACITY:
            break;
            case aiTextureType_DISPLACEMENT:
            break;
            case aiTextureType_LIGHTMAP:
            break;
            case aiTextureType_REFLECTION:
            break;
            case aiTextureType_UNKNOWN:
            break;
            case _aiTextureType_Force32Bit:
            break;
            */
        default:
            return textureType::undefined;
        }
    }
    aiTextureType TraitHelper<Lib::assimp>::convertTextType(textureType type)
    {
        switch (type)
        {
        case textureType::ambient:
            return aiTextureType_AMBIENT;
        case textureType::diffuse:
            return aiTextureType_DIFFUSE;
        case textureType::shininess:
            return aiTextureType_SHININESS;
        case textureType::specular:
            return aiTextureType_SPECULAR;
        case textureType::normals:
            return aiTextureType_NORMALS;
        case textureType::height:
            return aiTextureType_HEIGHT;
        default:
            return aiTextureType_UNKNOWN;
        }
    }
    
    ModelFactory<Lib::assimp>::ModelFactory(const aiScene* pScene) : m_pScene(pScene)
    {
        /* do nothing */
    }

    vector<TextureShared> ModelFactory<Lib::assimp>::loadTexture()
    {
        vector<TextureShared> vecRst;
        if (!getScenePtr()->HasMaterials()) return vecRst;

        for (size_t i = 0; i < getScenePtr()->mNumMaterials; ++i)
        {
            aiMaterial& mat = *getScenePtr()->mMaterials[i];
            for (auto it : libTextureTypes)
            {
                loadTypeTexture(&mat, it, vecRst);
            }
        }
        return std::move(vecRst);
    }

    void ModelFactory<Lib::assimp>::loadTypeTexture(aiMaterial* mat, aiTextureType type, vector<TextureShared>& textVec)
    {
        for (int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            textVec.push_back(make_shared<Texture>(TraitHelper<Lib::assimp>::convertTextType(type), str.C_Str(), global::modelPath));
        }

    }

    inline const aiScene* ModelFactory<Lib::assimp>::getScenePtr()
    {
        return m_pScene;
    }

    inline aiNode* ModelFactory<Lib::assimp>::getRootNode()
    {
        return getScenePtr()->mRootNode;
    }

    Scene ModelFactory<Lib::assimp>::produce()
    {
        Scene _scene;
        fillScene(_scene);
        return std::move(_scene);
    }

    void ModelFactory<Lib::assimp>::fillScene(Scene& scene)
    {
        scene.m_TexturePtrs = loadTexture();
        auto pNode = make_shared<Node>();
        fillNode(getScenePtr()->mRootNode, *pNode, scene);
        scene.m_rootNode = pNode;
    }

    void ModelFactory<Lib::assimp>::fillNode(aiNode* pNodeSrc, Node& nodeDst, Scene& scene)
    {
        size_t meshNum = pNodeSrc->mNumMeshes;
        nodeDst.m_MeshesPtrVec.resize(meshNum);
        for (size_t i = 0; i < meshNum; ++i)
        {
            auto pMesh = make_shared<Mesh>();
            fillMesh(getScenePtr()->mMeshes[pNodeSrc->mMeshes[i]], *pMesh, scene);
            nodeDst.m_MeshesPtrVec[i] = pMesh;
        }


        size_t nodeNum = pNodeSrc->mNumChildren;
        nodeDst.m_ChildrenPtrVec.resize(nodeNum);
        for (size_t i = 0; i < nodeNum; ++i)
        {
            auto pNode = make_shared<Node>();
            pNode->m_pParent = &nodeDst;
            fillNode(pNodeSrc->mChildren[i], *pNode, scene);
            nodeDst.m_ChildrenPtrVec[i] = pNode;
        }
    }

    void ModelFactory<Lib::assimp>::fillMesh(aiMesh* pMeshSrc, Mesh& meshDst, Scene& scene)
    {
        size_t numVertex = pMeshSrc->mNumVertices;
        meshDst.m_vertexVec.resize(numVertex);
        for (size_t i = 0; i < numVertex; ++i)
        {
            Vertex _vertex;

            // position
            _vertex.position.data[0] = pMeshSrc->mVertices[i].x;
            _vertex.position.data[1] = pMeshSrc->mVertices[i].y;
            _vertex.position.data[2] = pMeshSrc->mVertices[i].z;

            // normal
            _vertex.normal.data[0] = pMeshSrc->mNormals[i].x;
            _vertex.normal.data[1] = pMeshSrc->mNormals[i].y;
            _vertex.normal.data[2] = pMeshSrc->mNormals[i].z;

            // texture coord
            _vertex.textCoord.data[0] = pMeshSrc->mTextureCoords[0][i].x;
            _vertex.textCoord.data[1] = pMeshSrc->mTextureCoords[0][i].y;

            meshDst.m_vertexVec[i] = _vertex;

        }

        /* texture */
        for (auto type : libTextureTypes)
        {
            aiString name;
            aiMaterial* pMat = m_pScene->mMaterials[pMeshSrc->mMaterialIndex];
            for (int i = 0; i < pMat->GetTextureCount(type); i++)
            {
                pMat->GetTexture(type, i, &name);
                for (auto &it : scene.m_TexturePtrs)
                {
                    if (it->name == name.C_Str())
                    {
                        meshDst.m_textureVec.push_back(it);
                        break;
                    }
                }
                name.Clear();
            }
        }

        /* index */
        for (size_t i = 0; i < pMeshSrc->mNumFaces; i++)
        {
            for (size_t j = 0; j < pMeshSrc->mFaces[i].mNumIndices; j++)
            {
                meshDst.m_indexVec.push_back(pMeshSrc->mFaces[i].mIndices[j]);
            }
        }
    }
}