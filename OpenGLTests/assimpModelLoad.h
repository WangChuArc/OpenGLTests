#pragma once

#include"modelHandle.h"
#include"libs/assimp/include/scene.h"
#include"libs/assimp/include/Importer.hpp"
#include"libs/assimp/include/postprocess.h"


namespace modelHandle
{
    namespace Lib
    {

        ADD_TO_LIB_LIST(assimp)
#define LIB_LIST_TAIL assimp
    }
    using namespace Lib;

    template<>
    struct TraitHelper<Lib::assimp>
    {
  /*  struct TypesAccord2Lib
    {
    typedef aiScene TScene;
    typedef aiNode  TNode;
    typedef aiMesh  TMesh;
    };*/
    static textureType convertTextType(aiTextureType);
    static aiTextureType convertTextType(textureType);
    };

    template<>
    class ModelFactory<Lib::assimp>
    {
        static constexpr aiTextureType libTextureTypes[] = { aiTextureType_AMBIENT, aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_SHININESS, aiTextureType_HEIGHT, aiTextureType_NORMALS };
    public:
        typedef Lib::assimp libType;
        explicit ModelFactory(const aiScene* pScene);
        ModelFactory(const ModelFactory<Lib::assimp>&) = delete;
        vector<TextureShared> loadTexture();
        void loadTypeTexture(aiMaterial* mat, aiTextureType type, vector<TextureShared>& textVec);
        inline const aiScene* getScenePtr();
        inline aiNode* getRootNode();
        Scene produce();
        void fillScene(Scene&);
        void fillNode(aiNode*, Node&, Scene&);
        void fillMesh(aiMesh*, Mesh&, Scene&);
    private:
        const aiScene* m_pScene;
    };
}
