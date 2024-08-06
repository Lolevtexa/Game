#pragma once
#include "../activatable.hpp"
#include "../constantable/text.hpp"
#include <SFML/Graphics/Text.hpp>

class AText : public Activatable, public CText {
public:
  AText(std::wstring text) : CText(text) {}

protected:
  void appearance(sf::Color color) {
    for (auto &line : drawableText) {
      line.setFillColor(color);
    }
  }
};