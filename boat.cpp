#include <glm/gtc/constants.hpp>

class Boat {

  public: // FIXME: Dirty hack to temporarily ease development!!

  glm::vec3 position;
  float heading = 0.0f;

  //FIXME: add overheating mechanic?
  //FIXME: Should probably just add some kind of overspeed - liftoff mechanic [ https://www.youtube.com/watch?v=Zy2FHqsS5r8 ]

  //FIXME: unsigned int adrenalineLevel; // [1 - 4]
  float adrenalineMeter = 0.0f; // [0, 1]

  float engineSpeed = 0.0f;
  float steerAngle = 0.0f;
  float velocity = 0.0f;
  bool drifting = false;

  // This will slowly recover but increases when sliding on walls etc.
  float damage = 0.0f; // [0, 1]

  struct {
    //FIXME: Use formula parameters for engine curve and steer instead?!
    float acceleration = 0.01f;
    float steerRange; // Controls how wide can be steered
    float steerSpeed; // Controls how fast the steer angle adjusts
    float weight; // Weight of the entire boat
    float drag; // Amount of drag from air and water
    float durability; // Sets how fast damage will decrease
  } parameters;

  std::shared_ptr<Model> model;

  struct {
    std::shared_ptr<Sound> engine;
  } sounds;

  public:

  //FIXME: Make private
  struct {
    float steer = 0.0f; // [-1, +1]
    float accelerate = 0.0f; // [0, 1]
    float brake = 0.0f; // [0, 1]
    bool boost = false;
  } input;

  Boat(std::string path) {
    model = std::make_shared<Model>("data/model/hydroplane-1"); // FIXME: path + "-model");
    sounds.engine = std::make_shared<Sound>("data/sound/engine"); // FIXME: path + "-engine");
    sounds.engine->play();
  }

  void step() {

    //FIXME: Sanatize inputs!

#if 0
    auto drag = [](float density, float velocity, float area, float dragCoefficient) -> float {
      return 0.5f * density * velocity * velocity * area * dragCoefficient;
    };

    // See https://en.wikipedia.org/wiki/Drag_coefficient
    float dragCoefficient = 0.04f;

    // Values from https://en.wikipedia.org/wiki/Density
    float airDrag = drag(1.23f, airArea, dragCoefficient);
    float waterDrag = drag(999.0f, waterArea, dragCoefficient);
#endif

    // FIXME: Generate some engine speedup curve
    // FIXME: ...

    // FIXME: Move to parameters
    float finRudderDistance = 2.0f;
    float rudderAngle = input.steer * 0.1f;

    // Calculate how much we travel this frame
    float distance = engineSpeed;
    if (abs(rudderAngle) > 0.0f) {

      // Assuming fin is at (0.0, 0.0) and rudder at (0.0, finRudderDistance)
      // finRudderDistance - 0 = sin(rudderAngle) * cornerRadius
      // d_heading * cornerRadius = distance

      float cornerRadius = finRudderDistance / sinf(rudderAngle);
      cornerRadius *= 1.0f + distance * distance * 0.5f; // FIXME: Hack to limit sharp corners at high speeds

      position.x -= cornerRadius * sinf(-heading);
      position.y -= cornerRadius * cosf(-heading);
      heading -= distance / cornerRadius;
      position.x += cornerRadius * sinf(-heading);
      position.y += cornerRadius * cosf(-heading);
    } else {
      position.x += distance * cosf(heading);
      position.y += distance * sinf(heading);
    }

    input.steer = 0.0f; //FIXME: Clamp only!
    engineSpeed += parameters.acceleration * (input.accelerate - input.brake);



    // Update sound effects
//    sounds.engine->setPitch(engineSpeed);
    sounds.engine->setPosition(position);
    sounds.engine->setPitch(0.5f + engineSpeed);

// From https://www.youtube.com/watch?v=2Vp8C8MXU84
// 23.5 x RPM => 3000.000 Hz
// 40.0 x RPM => 4000.000 Hz
// 91.0 x RPM => 6857.143 Hz
  }

  glm::vec3 getPosition() const {
    return position;
  }

  void draw() {
    //FIXME! Do translation etc
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(glm::degrees(heading), 0.0f, 0.0f, 1.0f);
    model->draw();
    glPopMatrix();
  }

};
