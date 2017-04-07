#include <cstdbool>

#include "texture.h"

// This is a 2D animation [collection of textures]
//FIXME: varying playback speeds?!
class Animation {

  private:
  
  bool playing;
  std::vector<std::shared_ptr<Texture>> frames;
  unsigned int frame = 0;

  public:

  Animation(std::string path) {
    unsigned int i = 0;
    //FIXME: Load asynchronous
    while(i < 10) { //FIXME: While true but figure out if texture creation fails
      std::shared_ptr<Texture> texture = std::make_shared<Texture>(path + std::to_string(++i));
      if (texture == nullptr) {
        break;
      }
      frames.push_back(texture);
    }
  }

  unsigned int length() {
    return frames.size();
  }

  // Negative = relative to end
  void setFrame(int frame) {
    this->frame = (frame + length()) % length();
  }

  void play() {
    playing = true;
  }

  void pause() {
    playing = false;
  }

  bool isPlaying() {
    return playing;
  }

  void step() {
    if (playing) {
      frame = (frame + 1) % length();
    }
  }

  std::shared_ptr<Texture> texture() {
    return frames[frame];
  }
};
