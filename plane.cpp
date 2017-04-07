#include "platform.h"

//FIXME: Store origin?!
class Plane {

  private:

  glm::vec3 normal;
  glm::vec3 origin;

  public:

  //FIXME: Normalize normal!
  Plane(glm::vec3 normal, glm::vec3 origin) : normal(normal), origin(origin) {}
  Plane(glm::vec3 normal, float distance = 0.0f) : normal(normal) {
    origin = normal * distance;
  }

  glm::vec3 getOrigin() const {
    return origin;
  }

  glm::vec3 getNormal() const {
    return normal;
  }

  glm::vec3 project(glm::vec3 point, float distance = 0.0f) const {
    return point + (getDistance(point) - distance) * normal;
  }

  float getDistance(glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f)) const {
    return dot(normal, point - origin);
  }

  // Gets position on ray
  float collideRay(glm::vec3 origin, glm::vec3 direction) const {
    float projection = dot(direction, normal);
    return -getDistance(origin) / projection;
  }

  bool inFront(glm::vec3 point) const {
    return getDistance(point) > 0.0f;
  }

  //FIXME: Maybe just draw this more stupidly [axis aligned] and instead use a matrix?!
  //FIXME!!
  void draw(float gap = 1.0f, unsigned int segmentsX = 3, unsigned int segmentsY = 3, glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f)) {
    //FIXME: Project direction onto plane
    glm::vec3 directionOnPlane = project(origin + direction) - origin;
    directionOnPlane = glm::normalize(directionOnPlane);
    glm::vec3 directionOnPlaneY = glm::normalize(glm::cross(directionOnPlane, normal));

    float halfWidth = gap * segmentsX / 2.0f;

    glm::vec3 lineOrigin = origin - directionOnPlaneY * halfWidth;
    for(unsigned int i = 0; i <= segmentsX; i++) {
      glm::vec3 a = lineOrigin - directionOnPlane * halfWidth;
      glm::vec3 b = lineOrigin + directionOnPlane * halfWidth;
      lineOrigin += directionOnPlaneY * gap;
      Platform::drawLine(a, b);
    }

    // Draw the second axis
    if (segmentsY > 1) {
      draw(gap, segmentsY, 1, directionOnPlaneY);
    }

  }

};
