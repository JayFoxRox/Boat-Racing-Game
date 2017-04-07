#include "mesh.h"
#include "texture.h"

#include <memory>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {

  const struct aiScene* scene = nullptr; //FIXME: Remove (move data into meshes instead)

  public:

  glm::mat4 matrix;
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Texture> texture;
  std::vector<std::shared_ptr<Model>> children;

  void recursive_render(glm::mat4 matrix, const struct aiScene *sc, const struct aiNode* nd) {

    glm::mat4 t;
    for(unsigned int i = 0; i < 16; i++) {
      t[i % 4][i / 4] = nd->mTransformation[i / 4][i % 4];
    }
    matrix *= t;

    /* draw all meshes assigned to this node */
    for (unsigned int i = 0; i < nd->mNumMeshes; i++) {
	    const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[i]];

	    //FIXME: apply_material(sc->mMaterials[mesh->mMaterialIndex]);

/*
	    if(mesh->mNormals == NULL) {
		    glDisable(GL_LIGHTING);
	    } else {
		    glEnable(GL_LIGHTING);
	    }
*/

	    for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
		    const struct aiFace* face = &mesh->mFaces[j];

        if (face->mNumIndices != 3) { continue; } //FIXME: Remove
        assert(face->mNumIndices == 3);

		    glBegin(GL_TRIANGLES);

		    for(i = 0; i < face->mNumIndices; i++) {
			    int index = face->mIndices[i];

			    if(mesh->mTextureCoords[0] != NULL)
				    glTexCoord3fv((GLfloat*)&mesh->mTextureCoords[0][index]);
			    if(mesh->mColors[0] != NULL)
				    glColor4fv((GLfloat*)&mesh->mColors[0][index]);
			    if(mesh->mNormals != NULL)
				    glNormal3fv(&mesh->mNormals[index].x);
			    glVertex3fv(&mesh->mVertices[index].x);
		    }

		    glEnd();

	    }

    }

    /* draw all children */
    for (unsigned int i = 0; i < nd->mNumChildren; i++) {
	    recursive_render(matrix, sc, nd->mChildren[i]);
    }
  }

  Model(std::string modelPath, std::string texturePath) {
  	scene = aiImportFile((modelPath + ".obj").c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
    if (scene == nullptr) {
      std::cerr << "Failed to load model " << texturePath << std::endl;
    }
    texture = std::make_shared<Texture>(texturePath);
	  //aiReleaseImport(scene); FIXME
  }
  Model(std::string path) : Model(path, path) {}
  Model(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}

  void draw(glm::mat4 matrix = glm::mat4()) {
    // FIXME: Draw children
    if (texture != nullptr) {
      texture->activate(0);
    }
    if (scene != nullptr) {
      recursive_render(glm::mat4(), scene, scene->mRootNode);
    }
/*
    mesh->draw();
    for(m : children) {
      m->draw(matrix);
    }
*/
  }

};

