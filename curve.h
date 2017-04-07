#ifndef __CURVE_H__
#define __CURVE_H__

#include <vector>

#include <glm/glm.hpp>

//FIXME: Implement these as generic?

template <class T>
class Curve {

  private:

  std::vector<T> nodes;

  public:

  Curve<T>() {}

};

//using Curve2 = Curve<glm::vec2>;
using Curve3 = Curve<glm::vec3>;

class Curve2 {


  public:

  struct Node {
    glm::vec2 position;
  
    Node(glm::vec2 position) : position(position) {}

  };

  std::vector<Node> nodes;

  const Node& getNode(int nodeIndex) const {
    return nodes[nodeIndex % nodes.size()];
  }

  Node getNode(float offset) const {
    int nodeIndex = getNodeIndex(offset);
    float nodeOffset = getOffset(nodeIndex);
    float nextNodeOffset = getOffset(nodeIndex + 1);
    float t = (offset - nodeOffset) / (nextNodeOffset - nodeOffset);
    const Node& a = getNode(nodeIndex);
    const Node& b = getNode(nodeIndex + 1);
    return Node(glm::mix(a.position, b.position, t));
  }

//FIXME: Failed attempt, this should be same as getOffset with an extra break condition?!
  int getNodeIndex(float offset) const {

//FIXME!!!!! This is merely a hack!!
if (offset < 0.0f) {
  return 0;
}

    //FIXME: Handle negative offsets?
    assert(offset >= 0.0f);

    int loopCount = 0;

    // Get rid of loops
    float loopLength = getOffset(nodes.size());
std::cout << "Loop length: " << loopLength << std::endl;
std::cout << "Offset: " << offset << std::endl;
    if (offset >= loopLength) {
      float loops = offset / loopLength;
      offset = fmod(offset, loopLength);
std::cout << "Loops: " << loops << std::endl;
std::cout << "remainder: " << offset << std::endl;
      loopCount = floor(loops);
    }

    int result = 0;
    while(getOffset(++result) <= offset);
    result--;

    return loopCount * nodes.size() + result;
  }

  float getOffset(int nodeIndex) const {

    float result = 0.0f;

    //FIXME: Handle negative indices?!
    if (nodeIndex < 0) { return 0.0f; }

    auto getDistance = [](const Node& a, const Node& b) -> float {
      return glm::length(b.position - a.position);
    };

    int loopCount = nodeIndex / nodes.size();
    int remainder = nodeIndex % nodes.size();

    float loopLength = 0.0f;
    unsigned int partCount = loopCount ? (nodes.size() - 1) : remainder;
    for(int i = 0; i < partCount; i++) {
      float partLength = getDistance(nodes[i], nodes[i + 1]);
      if (loopCount) { loopLength += partLength; }
      if (i < remainder) { result += partLength; }
    }

    return result + loopCount * loopLength;
  }

  glm::mat4 getMatrix(int nodeIndex, bool smooth = false) const {
    const Node& node = getNode(nodeIndex);
    const Node& nextNode = getNode(nodeIndex + 1);

    glm::vec3 forward = glm::vec3(glm::normalize(nextNode.position - node.position), 0.0f);

    if (smooth) {
      const Node& prevNode = getNode(nodeIndex - 1);
      glm::vec3 prevForward = glm::vec3(glm::normalize(node.position - prevNode.position), 0.0f);
      forward = glm::mix(prevForward, forward, 0.5f);
#if 1
      //FIXME: Compensate width?!
      forward /= glm::dot(prevForward, forward);
#endif
    }

    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 right = glm::cross(forward, up);

    glm::mat3 rotation = glm::mat3(forward, right, up);
    glm::vec3 position = glm::vec3(node.position, 0.0f);

    glm::mat4 matrix;
    matrix = glm::translate(matrix, position);
    matrix = matrix * glm::mat4(rotation);
    //matrix = glm::translate(matrix, glm::vec3(-getOffset(nodeIndex), 0.0f, 0.0f));
    return matrix;
  }

  float length() const {
    return getOffset(nodes.size() - 1);
  }

  void draw() const {
#if 1
    // Draw this using legacy GL
    glBegin(GL_LINES);
    //for(auto& node : nodes) {

    auto draw = [&](const Node& a, const Node& b) {
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3f(a.position.x, a.position.y, 0.0f);
      glColor3f(1.0f, 1.0f, 1.0f);
      glVertex3f(b.position.x, b.position.y, 0.0f);
    };

    for(unsigned int i = 0; i < nodes.size() - 1; i++) {
      draw(nodes[i], nodes[i + 1]);
    }

    glEnd();
#endif
  }
};

#endif
