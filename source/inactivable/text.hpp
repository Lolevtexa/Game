#pragma once
#include "../inactivable.hpp"
#include "../resource.hpp"
#include <SFML/Graphics/Text.hpp>

class IText : public Inactivable {
protected:
  sf::Text text;

public:
  IText(std::wstring string) {
    text.setString(string);
    text.setFont(Resource::defaultFont);
    text.setFillColor(Resource::focusedColor);
    text.setCharacterSize(24);
  }

  void setBound(float x, float y, float width, float height, float indent) {
    Inactivable::setBound(x, y, width, height, indent);

    text.setPosition(x + width / 2.f - text.getGlobalBounds().width / 2.f, y + height / 2.f - text.getGlobalBounds().height / 2.f);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(text, states);
  }
};