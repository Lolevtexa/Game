#include <SFML/Graphics/Font.hpp>
#include <stdexcept>

class Resource {
public:
  static const sf::Font defaultFont;
};

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.loadFromFile("../assets/fonts/arial.ttf")) {
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();