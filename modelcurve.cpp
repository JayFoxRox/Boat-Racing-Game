#include "model.h"
#include <vector>
#include <memory>

#include "shader.h"

#include "curve.h"

#include <glm/gtc/matrix_transform.hpp>

class ModelCurve {

  public:

  struct Placement {
    float offset;
    std::shared_ptr<Model> model;
    bool rigid;
  };

  Curve2 curve;

  ModelCurve(Curve2 curve) : curve(curve) {}
  
  void addModel(float offset, std::shared_ptr<Model> model, bool rigid = false) {
    placements.push_back((Placement){offset,model,rigid}); //FIXME!
  }

  void draw() const {

    static Shader genericShader("data/shader/modelcurve", "data/shader/generic");
    Shader* shader = &genericShader; //FIXME: genericShader will be a shared_ptr at some point

#if 1
    //FIXME: Do instanced rendering
    for(auto& p : placements) {
      if (p.rigid) {
        unsigned int nodeIndex = curve.getNodeIndex(p.offset);
        float nodeOffset = curve.getOffset(nodeIndex);
        glm::mat4 matrix = curve.getMatrix(nodeIndex);
        matrix = glm::translate(matrix, glm::vec3(-nodeOffset, 0.0f, 0.0f));

        shader->activate();
        shader->set("offset", p.offset); //currentStartOffset);
        shader->set("startOffset", 0.0f);
        shader->set("endOffset", 1.0f);
        shader->set("startMatrix", matrix);
        shader->set("endMatrix", matrix);

        p.model->draw();
      } else {
        // We draw this mesh in pieces by clipping very often.
        //FIXME: This is very bad for perf.; Instanced + Baking on CPU etc.

        std::cout << std::endl;

        float length = p.model->mesh->boundingBox().x;
        float startOffset = p.offset - length;
        float endOffset = p.offset + length;

        glEnable(GL_CLIP_PLANE0);
        glEnable(GL_CLIP_PLANE1);

        float s = 0.0f;
        unsigned int currentIndex = 0;

        while((s <= endOffset) && (currentIndex < (curve.nodes.size() - 1))) {
          float e;

          glm::mat4 sm;
          if (currentIndex <= 0) {
            s = 0.0f;
            sm = curve.getMatrix(0);
          } else {
            sm = curve.getMatrix(currentIndex, true);
          }

          glm::mat4 em;
          if (currentIndex < (curve.nodes.size() - 2)) {
            e = curve.getOffset(currentIndex + 1);
            em = curve.getMatrix(currentIndex + 1, true);
          } else {
            em = curve.getMatrix(curve.nodes.size() - 2);
            float ePrev = curve.getOffset(curve.nodes.size() - 2);
            e = curve.getOffset(curve.nodes.size() - 1);
            em = glm::translate(em, glm::vec3(e - ePrev, 0.0f, 0.0f));
          }

          sm = glm::translate(sm, glm::vec3(-s, 0.0f, 0.0f));
          em = glm::translate(em, glm::vec3(-e, 0.0f, 0.0f));

          // We include the offsets for clipping and blending purposes
          shader->activate();
          shader->set("offset", p.offset); //currentStartOffset);
          shader->set("startOffset", s); //currentStartOffset);
          shader->set("endOffset", e); //currentEndOffset);
          shader->set("startMatrix", sm);
          shader->set("endMatrix", em);

          std::cout << "Drawing for " << std::to_string(currentIndex) << " at [" << s << ", " << e << "]" << std::endl; 
          p.model->draw();

          s = e;
          currentIndex++;
        }

        glDisable(GL_CLIP_PLANE0);
        glDisable(GL_CLIP_PLANE1);
      }
    }
#endif
  }

  private:
  
  std::vector<Placement> placements;

};
