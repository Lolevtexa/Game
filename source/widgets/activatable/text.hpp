#pragma once
#include "../activatable.hpp"
#include "../constantable/text.hpp"

class AText : public Activatable, public CText {
public:
  AText(const std::wstring &string) : CText(string) {
    appearance(Resource::unfocusedColor);
  }
  AText(const std::vector<std::string> &localizationKeys)
      : CText(localizationKeys), Activatable() {
    appearance(Resource::unfocusedColor);
  }

protected:
  void appearance(sf::Color color) {
    textColor = color;
    for (auto &line : drawableText) {
      line.setFillColor(textColor);
    }
  }
};
