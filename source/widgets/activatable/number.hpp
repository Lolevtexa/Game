#pragma once
#include "../activatable.hpp"
#include "../constantable/number.hpp"

class ANumber : public Activatable, public CNumber {
public:
  template <typename Func> ANumber(Func &getValue) : CNumber(getValue) {
    appearance(Resource::unfocusedColor);
  }

protected:
  void appearance(sf::Color color) { drawableNumber.setFillColor(color); }
};
