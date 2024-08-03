#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <stdexcept>

class Resource {
public:
  static const int buttonWidth = 150;
  static const int buttonHeight = 50;
  static const int buttonIndent = 10;

  static const sf::Font defaultFont;

  static sf::Music *loadBackgroundMusic() {
    sf::Music *music = new sf::Music();
    if (!music->openFromFile("assets/music/hound-dog-jazz.mp3")) {
      throw std::runtime_error("Cannot load music");
    }

    return music;
  }
};

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();