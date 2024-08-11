#pragma once
#include "../activatable.hpp"
#include "../constantable/verticalWigets.hpp"

class AVerticalWigets : virtual public Activatable,
                          public CVerticalWigets<Activatable> {
public:
  AVerticalWigets(std::vector<Activatable *> elements)
      : CVerticalWigets(elements) {
    appearance(Resource::unfocusedColor);
  }

protected:
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }
  }
};
