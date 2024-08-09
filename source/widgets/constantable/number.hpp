#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

class CNumber : virtual public Constantable {
protected:
  bool valueIsNumber = false;

  int value = -1;
  std::function<int()> getValue;

  sf::Text drawableNumber;

public:
  template <typename Func> CNumber(Func &getValue) : getValue(getValue) {
    drawableNumber.setFont(Resource::defaultFont);
    drawableNumber.setFillColor(Resource::focusedColor);
    drawableNumber.setCharacterSize(Resource::characterSize);
    drawableNumber.setLineSpacing(Resource::lineSpacing);
  }

  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    if (value != getValue()) {
      value = getValue();
      drawableNumber.setString(std::to_string(value));
      valueIsNumber = false;
    }

    drawableNumber.setPosition(
        x + width / 2 - drawableNumber.getGlobalBounds().width / 2, y);
    float deltaY = drawableNumber.getGlobalBounds().height +
                   drawableNumber.getLineSpacing();
    float minWidth = std::min(width, drawableNumber.getGlobalBounds().width);

    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(drawableNumber, states);
  }
};
