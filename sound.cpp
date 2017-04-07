#include "AL/al.h"
#include "AL/alut.h"

//FIXME: Add sound effects such as echo etc.

class Sound {

  private:

  ALuint source;
  ALuint buffer;

  public:

  Sound(std::string path) {
    //FIXME: Add streamable format and somehow get data from that..
    buffer = alutCreateBufferFromFile((path + ".wav").c_str());
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcef(source, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcei(source, AL_LOOPING, AL_TRUE); // FIXME: Somewhere else. make this an option
  }

  void play() {
    alSourcePlay(source);
  }

  void setPosition(glm::vec3 position) {
    alSource3f(source, AL_POSITION, position.x, position.y, position.z);
  }

  void setVelocity(glm::vec3 velocity) {
    alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
  }

  void removeDirection() {
    alSource3f(source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
  }

  void setDirection(glm::vec3 direction, float innerAngle, float outerAngle) {
    alSource3f(source, AL_DIRECTION, direction.x, direction.y, direction.z);
    alSourcef(source, AL_CONE_INNER_ANGLE, innerAngle);
    alSourcef(source, AL_CONE_OUTER_ANGLE, outerAngle);
  }

  void setPitch(float pitch) {
    alSourcef(source, AL_PITCH, pitch);
  }

};
