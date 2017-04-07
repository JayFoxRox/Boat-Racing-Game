
class Sparks {

  private:

  // FIXME: Move this elsewhere?!
  const glm::vec3 gravity = glm::vec3(0.0f, 0.0f, -9.81f);

  class Particle {

    public:

    //FIXME: getters and setters!
    glm::vec3 position; // m
    glm::vec3 velocity; // m/s
    //FIXME: Add some sort of air resistance damping
    
    Particle(glm::vec3 position, glm::vec3 velocity) : position(position), velocity(velocity) {}
  };

  std::shared_ptr<Texture> texture;

  std::vector<Particle> particles;
  
  public:

  Sparks(glm::vec3 position, glm::vec3 velocity) {
    //FIXME: accept an std::string path
    //FIXME: Also load a settings file so we can use the same particle class for smoke, fire, sparks, ... ?!

    texture = std::make_shared<Texture>("data/particles/spark");

    for(unsigned int i = 0; i < 5; i++) {
      //FIXME: Use c++ rand and move to some `Random` namespace
      auto randf = [](float min = 0.0f, float max = 1.0f) -> float {
        float r = static_cast<float> (rand()) / static_cast<float>(RAND_MAX);
        return min + r * (max - min);
      };
      glm::vec3 v = velocity;
      v += glm::vec3(randf(-1.0f), randf(-1.0f), 0.0f);
      particles.emplace_back(position, v);
    }
  }

  void step() {
    float dt = 1.0f / 60.0f;
    for (auto& p : particles) {
      p.position += p.velocity * dt;
      p.velocity += gravity * dt;
    }
  }

  void draw() const {
    texture->activate(0);

    glBegin(GL_TRIANGLES);
    for (const auto& p : particles) {

      // FIXME: Set billboard orientation
      //FIXME

      glm::vec3 position = p.position;

      glColor3f(1.0f, 1.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3fv(glm::value_ptr(position + glm::vec3(-1.0f, 0.0f, -1.0f)));
      glTexCoord2f(0.0f, 1.0f); glVertex3fv(glm::value_ptr(position + glm::vec3(+1.0f, 0.0f, -1.0f)));
      glTexCoord2f(1.0f, 1.0f); glVertex3fv(glm::value_ptr(position + glm::vec3(0.0f, 0.0f, 1.0f)));

    }
    glEnd();
  }  
};
