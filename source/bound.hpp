#pragma once
#include <SFML/Graphics/Rect.hpp>

class Bound {
protected:
  sf::FloatRect body;

public:
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    body.height = height;
    body.width = width;
    body.top = y;
    body.left = x;
  }

  sf::FloatRect getBound() { return body; }
};