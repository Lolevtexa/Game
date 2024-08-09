#pragma once
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class Image : virtual public Constantable {
protected:
  sf::RectangleShape image;

public:
  Image(sf::Texture *texture) { image.setTexture(texture); }

  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    image.setPosition(x, y);
    image.setSize(sf::Vector2f(width, height));

    Bound::setBound(x, y, width, height, indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(image, states);
  }
};
