#pragma once
#include "../activatable.hpp"
#include "../constantable/horizontalWigets.hpp"

class AHorizontalWigets : virtual public Activatable,
                          public CHorizontalWigets<Activatable> {
public:
  AHorizontalWigets(std::vector<Activatable *> elements)
      : CHorizontalWigets(elements) {
    appearance(Resource::unfocusedColor);
  }

protected:
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }
  }
};
