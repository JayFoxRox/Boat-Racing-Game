#include "plane.h"

#include "glm/gtx/string_cast.hpp"

#include <vector>
#include <array>

class Mesh {
  //FIXME: assimp magic?!

  class Vertex {

    public:

    Vertex() {}
    Vertex(glm::vec3 position) : position(position) {}

    glm::vec3 position;
    glm::vec2 uv;

    //FIXME: Overload * and +

  };

  class Triangle {

    public:

    Triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
      vertices[0] = a;
      vertices[1] = b;
      vertices[2] = c;
    }
    
    //std::array<Vertex, 3> vertices;
    Vertex vertices[3];

  };

  public:

  std::vector<Triangle> triangles;

  Mesh() {

    //FIXME!
#if 1
    {
      Vertex a(glm::vec3( 0.0f, 0.0f, -0.1f));
      Vertex b(glm::vec3( 0.0f, 0.0f,  0.1f));
      Vertex c(glm::vec3(1.0f, 0.0f,  0.1f));
      Vertex d(glm::vec3(1.0f, 0.0f, -0.1f));
      addQuad(a, b, c, d);
    }
    {
      Vertex a(glm::vec3( 0.0f, 0.0f, 0.1f));
      Vertex b(glm::vec3( 0.0f, 0.3f, 0.1f));
      Vertex c(glm::vec3(1.0f, 0.3f, 0.1f));
      Vertex d(glm::vec3(1.0f, 0.0f, 0.1f));
      addQuad(a, b, c, d);
    }

#else
    Vertex a(glm::vec3( 0.0f, 0.0f,  0.1f));
    Vertex b(glm::vec3(-0.1f, 0.0f, -0.1f));
    Vertex c(glm::vec3( 0.1f, 0.0f, -0.1f));
    triangles.emplace_back(a, b, c);
#endif
  }

  void addTriangle(const Vertex& a, const Vertex& b, const Vertex& c) {
    triangles.emplace_back(a, b, c);
  }

  // Adds 2 triangles and minimizes the length of diagonal
  void addQuad(Vertex a, Vertex b, Vertex c, Vertex d) {
    float acLength = glm::length(a.position - c.position);
    float bdLength = glm::length(d.position - b.position);

    if (acLength <= bdLength) {
      // Add diagonal from C to A
      addTriangle(a, b, c);
      addTriangle(c, d, a);
    } else {
      // Add diagonal from B to D
      addTriangle(a, b, d);
      addTriangle(b, c, d);
    }
  }

  //FIXME: Slicing already sliced meshes is really bad!
  //       A solution would be to merge vertices if they don't add shape
  //       and don't add extra variables which can't be interpolated
  void slice(const Plane& plane) {
    
    unsigned int originalSize = triangles.size();
    for(unsigned int i = 0; i < originalSize; i++) {

      Triangle& t = triangles[i];

      auto a = t.vertices[0];
      auto b = t.vertices[1];
      auto c = t.vertices[2];

      //FIXME: This might be broken?!
      auto intersect = [&](glm::vec3 a, glm::vec3 b) -> float {
        glm::vec3 direction = glm::normalize(b - a);
        float ta = plane.collideRay(a, direction);
        float tb = plane.collideRay(b, direction);        
        float length = abs(tb - ta);
        return ta / length;
      };

      auto lerp = [](const Vertex& a, const Vertex& b, float t) -> Vertex {
        Vertex result;
        result.position = glm::mix(a.position, b.position, t);
        return result;
        //FIXME: return a * (1.0f - t) + b * t;
      };

      // Do a slice on edge between A & B and A & C
      auto doSlice = [&](const Vertex& a, const Vertex& b, const Vertex& c) -> bool {

          std::cout << "Slice " << glm::to_string(a.position) << " " << plane.inFront(a.position)
                                << "  " << glm::to_string(b.position) << " " << plane.inFront(b.position)
                                << "  " << glm::to_string(c.position) << " " << plane.inFront(c.position) << std::endl;

        // We won't do a slice if these edges don't intersect with a plane
        if ((plane.inFront(a.position) == plane.inFront(b.position)) ||
            (plane.inFront(a.position) == plane.inFront(c.position))) {
          std::cout << "Fail!" << std::endl;
          return false;
        }

        float tab = intersect(a.position, b.position);
        float tac = intersect(a.position, c.position);

#if 0
        // Handle case where tab or tac == 0.0 or 1.0 = split through corner
        //FIXME: Ideally we'd create 2 triangles here, for now, do nothing
        if (tab <= 0.0f || tab >= 1.0f) { return false; }
        if (tac <= 0.0f || tac >= 1.0f) { return false; }
#endif

      
        Vertex ab = lerp(a, b, tab);
        Vertex ac = lerp(a, c, tac);

        std::cout << "Done!!! " << tab << " " << glm::to_string(ab.position)
                        << "  " << tac << " " << glm::to_string(ac.position) << std::endl;

        //FIXME: Handle vertices which are shared across multiple triangles

        triangles.emplace_back(a, ab, ac);
        addQuad(ab, b, c, ac);

        // Erase vertex and process this index (item changed now) again
        triangles.erase(triangles.begin() + i--);

        return true;
      };

      // Attempt each slice
      if (!doSlice(a, b, c)) {
        if (!doSlice(b, c, a)) {
          doSlice(c, a, b);
        }
      }

    }
  }

  //FIXME: Move to mesh
  glm::vec3 boundingBox() const {
    glm::vec3 bounds(0.0f);
    for(auto& t : triangles) {
      for(auto& v : t.vertices) {
        bounds = glm::max(glm::abs(v.position), bounds);
      }
    }
    return bounds;
  }

  void draw() const {
    glBegin(GL_TRIANGLES);
    for(auto& t : triangles) {
      for(auto& v : t.vertices) {
        glVertex3f(v.position.x, v.position.y, v.position.z);
      }
    }
    glEnd();
  }

};
