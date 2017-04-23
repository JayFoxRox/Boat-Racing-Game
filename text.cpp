// Use https://github.com/andryblack/fontbuilder to generate the fonts!
//FIXME: Get rid of the struct packing?!

#include <cstdint>
#include <fstream>
#include <memory>
#include <algorithm>

#include "texture.h"
#include "text.h"
#include "shader.h"

class Text {

  const char magic[13] { 'Z', 'G', 'L', '_', 'F', 'O', 'N', 'T', '_', 'I', 'N', 'F', 'O' };

  struct __attribute__((packed)) Header {
    char magic[13];
    uint16_t pages;
    uint16_t chars;
    uint32_t maxHeight;
    uint32_t maxShiftY;
    uint32_t padding;
  };

  struct __attribute__((packed)) Symbol {
    int32_t id;
    int32_t page;
    uint8_t width;
    uint8_t height;
    //FIXME: Not sure about these types, they were `int` in the original tool..
    int32_t shiftX;
    int32_t shiftY;
    int32_t shiftP;
    struct {
      float x;
      float y;
    } texCoords[4];
  };
  
  //FIXME: Static assert about struct sizes!

  Header header;
  std::vector<Symbol> symbols;
  std::shared_ptr<Texture> texture;

  public:

  Text(std::string path) {
    texture = std::make_shared<Texture>(path);

    // Load the ZenGL Font Information
    std::ifstream file;
    file.open(path + ".zfi");

    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    std::cout << header.magic << std::endl;

    // FIXME: Verify header magic?!
    assert(header.pages == 1);

    for(unsigned int i = 0; i < header.chars;i++) {
      Symbol symbol;
      file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
      symbols.push_back(symbol);
    }

    file.close();
  }

  //FIXME: Set text elsewhere!
  void draw(std::string text) {

    static Shader shader("data/shader/font");

    shader.activate();
    shader.set("texture", 0u);

    texture->activate(0);

    glm::vec2 position = glm::vec2(0.0f);

    glBegin(GL_QUADS);
    for(auto c : text) {

      if (c == '\n') {
        position.x = 0;
        position.y += header.maxHeight;
        continue;
      }

      auto isMatchingSymbol = [&](const Symbol& symbol) -> bool {
          return symbol.id == static_cast<unsigned char>(c);
      };
      const auto& it = std::find_if(symbols.begin(), symbols.end(), isMatchingSymbol);
      assert(it != symbols.end());
      const auto& symbol = *it;

      //FIXME: Use proper kerning etc
      glm::vec2 size = glm::vec2(symbol.width, symbol.height);

      glm::vec2 shift = glm::vec2(symbol.shiftX, -symbol.shiftY);

      glTexCoord2fv(reinterpret_cast<const GLfloat*>(&symbol.texCoords[3]));
      glVertex2fv(glm::value_ptr(position + shift + size * glm::vec2(0.0f, 0.0f)));

      glTexCoord2fv(reinterpret_cast<const GLfloat*>(&symbol.texCoords[2]));
      glVertex2fv(glm::value_ptr(position + shift + size * glm::vec2(1.0f, 0.0f)));

      glTexCoord2fv(reinterpret_cast<const GLfloat*>(&symbol.texCoords[1]));
      glVertex2fv(glm::value_ptr(position + shift + size * glm::vec2(1.0f, 1.0f)));

      glTexCoord2fv(reinterpret_cast<const GLfloat*>(&symbol.texCoords[0]));
      glVertex2fv(glm::value_ptr(position + shift + size * glm::vec2(0.0f, 1.0f)));

      position.x += symbol.shiftP;
    }
    glEnd();
  }
};
