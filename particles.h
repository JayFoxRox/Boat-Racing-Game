#pragma once

#include <vector>

#include <glm/glm.hpp>

// Sample code for physics simulation
class Particles {

public:
  struct Particle {
    glm::vec3 m_x;    // Current positions
    glm::vec3 m_oldx; // Previous positions
    glm::vec3 m_a;    // Force accumulators
  };

  struct DistanceConstraint {
    unsigned int a;
    unsigned int b;
    float distance;
  };

  std::vector<Particle> particles;
  std::vector<DistanceConstraint> distance_constraints;

  glm::vec3    m_vGravity = glm::vec3(0.0f, 0.0f, -9.81f);            // Gravity

public:

  void addDistanceConstraint(unsigned int a, unsigned int b) {
    DistanceConstraint c;
    c.a = a;
    c.b = b;
    //FIXME: Take length from parameters instead
    c.distance = glm::distance(particles[a].m_x, particles[b].m_x);
    distance_constraints.emplace_back(c);
  }

  void addParticle(glm::vec3 position, glm::vec3 velocity) {
    Particle p;
    p.m_x = position;
    p.m_oldx = position - velocity;
    particles.emplace_back(p);
  }

  void Step();

private:
  void       Verlet();
  void       SatisfyConstraints();
  void       AccumulateForces();
};
