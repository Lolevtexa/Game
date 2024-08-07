#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class COutline : virtual public Constantable {
protected:
  sf::FloatRect *rect;
  sf::RectangleShape outLine;

public:
  COutline() {
    outLine.setOutlineThickness(3);
    outLine.setFillColor(Resource::transparentColor);
  }

  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    outLine.setPosition(x, y);
    outLine.setSize(sf::Vector2f(width, height));

    Bound::setBound(x, y, width, height, indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(outLine, states);
  }
};