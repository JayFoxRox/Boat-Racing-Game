// Mostly taken from http://graphics.cs.cmu.edu/nsp/course/15-869/2006/papers/jakobsen.htm

#include "particles.h"

#include <iostream>

#define NUM_ITERATIONS 10
#define TIMESTEP (1.0f / 60.0f)

// Verlet integration step
void Particles::Verlet() {
  for(int i = 0; i < particles.size(); i++) {
    auto& x = particles[i].m_x;
    auto temp = x;
    auto& oldx = particles[i].m_oldx;
    auto& a = particles[i].m_a;
    float dt = TIMESTEP;
    x += x - oldx + a * dt * dt;
    oldx = temp;
  }
  return;
}

void Particles::AccumulateForces() {

  // All particles are influenced by gravity
  for(int i = 0; i < particles.size(); i++) {
    particles[i].m_a = m_vGravity;
  }

  return;
}

void Particles::SatisfyConstraints() {
  for(int j = 0; j < NUM_ITERATIONS; j++) {

    // First satisfy (C1)
    for(int i = 0; i < particles.size(); i++) { // For all particles
      auto& x = particles[i].m_x;
      x = glm::clamp(x, glm::vec3(-1000.0f, -1000.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
    }

    // Then satisfy (C2)
    for(int i = 0; i < distance_constraints.size(); i++) {
      const auto& c = distance_constraints[i];

      auto& x1 = particles[c.a].m_x;
      auto& x2 = particles[c.b].m_x;
      auto delta = x2 - x1;
#if 1
      float deltalength = glm::length(delta);
      std::cout << "delta: " << deltalength << std::endl;
      float diff = (deltalength - c.distance) / deltalength;
      x1 += delta * 0.5f * diff;
      x2 -= delta * 0.5f * diff;
#if 0
      // Pseudo-code to satisfy (C2) while respecting particle masses
      float diff = (deltalength - c.distance) / (deltalength * (invmass1 + invmass2));
      x1 += invmass1*delta*diff;
      x2 -= invmass2*delta*diff;
#endif

#else
      // Pseudo-code for satisfying (C2) using sqrt approximation
      float precalc = c.distance * c.distance;
      delta *= precalc / (delta * delta + precalc) - 0.5f;
      x1 += delta;
      x2 -= delta;
#endif
    }

  }

  return;
}

void Particles::Step() {
  AccumulateForces();
  Verlet();
  SatisfyConstraints();
  return;
}
