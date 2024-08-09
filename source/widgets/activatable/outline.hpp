#pragma once
#include "../activatable.hpp"
#include "../constantable/outline.hpp"

class AOutline : virtual public Activatable, public COutline {
public:
  AOutline() : COutline() {}

protected:
  void appearance(sf::Color color) { outLine.setOutlineColor(color); }
};
