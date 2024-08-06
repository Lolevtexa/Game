#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class Inactivable : public sf::Drawable {
protected:
  sf::FloatRect body;

public:
  void setBound(float x, float y, float width, float height,
                        float indent) {
    body.height = height;
    body.width = width;
    body.top = y + indent;
    body.left = x + indent;
  }

  sf::FloatRect getBound() {
    return body;
  }
};