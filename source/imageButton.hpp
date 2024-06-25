#pragma once
#include "button.hpp"
#include <SFML/Graphics/Texture.hpp>

class ImageButtons : public Button {
protected:
  std::string filename;
  sf::Image icon;
  sf::Texture texture;

public:
  template <typename Func>
  ImageButtons(const std::string &filename, int x, int y, int width, int height,
               Func func)
      : Button(sf::Vector2f(width, height), sf::Vector2f(x, y), func) {
    this->filename = filename;

    updateAppearance(unpressedAlpha);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(body, states);
  }

private:
  void updateAppearance(int alpha) {
    if (icon.loadFromFile(filename)) {
      icon.createMaskFromColor(sf::Color::Black, alpha);
      if (texture.loadFromImage(icon)) {
        body.setTexture(&texture);
        return;
      }
    }
  }
};