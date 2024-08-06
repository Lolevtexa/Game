#pragma once
#include "../activable.hpp"
#include "../inactivable/text.hpp"
#include <SFML/Graphics/Text.hpp>

class AText : public Activable, public IText {
public:
  AText(std::wstring string): IText(string) { }

  void setBound(float x, float y, float width, float height, float indent) {
    Activable::setBound(x, y, width, height, indent);
    IText::setBound(x, y, width, height, indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    IText::draw(target, states);
  }

protected:
  void updateAppearance(sf::Color color) {
    text.setFillColor(color);
  }
};