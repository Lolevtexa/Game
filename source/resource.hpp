#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Music.hpp>
#include <stdexcept>

class Resource {
public:
  static const sf::Font defaultFont;
  
  static sf::Music* loadBackgroundMusic() {
    sf::Music* music = new sf::Music();
    if (!music->openFromFile("../assets/music/hound-dog-jazz.mp3")) {
      throw std::runtime_error("Cannot load music");
    }
  
    return music;
  }
};

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.loadFromFile("../assets/fonts/arial.ttf")) {
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();