#include "model.h"
#include "curve.h"
#include "modelcurve.h"

#include <vector>
#include <fstream>

class Track {

  private:

  std::string path;

  public:

  Track(std::string path) : path(path) {
    reload();
  }

  // Recover broken objects etc.
  void reset() {
    //FIXME!
  }

  float fillCurve(std::shared_ptr<ModelCurve> modelCurve, bool rigid, bool random, const std::vector<std::shared_ptr<Model>>& models) {
  //FIXME: This logic will be in the level loader which will have model pools etc.
  // Tightly pack models along curve

    float pad = 0.5f + 0.1f;
    float offset = 0.0f;

//offset = modelCurve->curve.getOffset(6) - 0.5;

    unsigned int i = 0;
    while(offset <= modelCurve->curve.length()) {
      //FIXME: select model
      std::shared_ptr<Model> model;
      if (random) {
        model = models[0]; //FIXME: Pick random, FIXME: Also make sure this always gets the same seed!
      } else {
        model = models[i];
      }

#if 1
      //FIXME: This is probably not necessary with actual art, so it should probably be avoided altogether :P
      // Split this model into smaller pieces
      float length = model->mesh->boundingBox().x;
      if (!rigid) {
        float startOffset = offset - length; //FIXME: Use min/max bounding box instead
        float endOffset = offset + length;
        unsigned int startIndex = modelCurve->curve.getNodeIndex(startOffset);
        unsigned int endIndex = modelCurve->curve.getNodeIndex(endOffset);
        while(startIndex <= endIndex) {
          // FIXME: Create a copy for this mesh instead..
          float nodeOffset = modelCurve->curve.getOffset(startIndex);
          model->mesh->slice(Plane(glm::vec3(1.0f, 0.0f, 0.0f), nodeOffset - offset));

#if 0
          float nnodeOffset = modelCurve->curve.getOffset(startIndex + 1);
          model->mesh->slice(Plane(glm::vec3(1.0f, 0.0f, 0.0f), (nodeOffset * 0.33 + nnodeOffset * 0.66) - offset));
          model->mesh->slice(Plane(glm::vec3(1.0f, 0.0f, 0.0f), (nodeOffset * 0.66 + nnodeOffset * 0.33) - offset));
#endif

          startIndex++;
        }
      }
#endif
      
      modelCurve->addModel(offset, model, rigid); //FIXME: Use rigid from vector instead
std::cout << "Placed model at " << offset << std::endl;

      offset += /*length*/ 0.5f + pad; //FIXME: Use float from a vector instead (think streetlamps etc.)
      i++;

    }

    return offset;
  }

  // This reloads the whole track from scratch, can be used during debugging
  void reload() {

    //FIXME: Handle imports
    //FIXME: Guard against cyclic dependency

    std::ifstream file;
    file.open(path + ".track");

    if (file.fail()) {
      std::cerr << "Failed to open track " << path << std::endl;
      return;
    }

    //FIXME: Parse file
    while(!file.eof()) {
      std::string line;
      std::getline(file, line);

      std::cerr << "Could not parse '" << line << "'" << std::endl;
      //FIXME: fscanf magic!
    }

    file.close();
  }

};
