#pragma once
#include "constantable.hpp"
#include "resource.hpp"
#include <SFML/Window/Event.hpp>

class Activatable : virtual public Constantable {
protected:
  bool focused = false;
  bool resetColor = true;

public:
  void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseMoved) {
      if (body.contains(event.mouseMove.x, event.mouseMove.y)) {
        resetColor = focused == false;
        focused = true;
      } else {
        resetColor = focused == true;
        focused = false;
      }
    }

    if (event.type == sf::Event::MouseLeft) {
      resetColor = focused == true;
      focused = false;
    }
  }

  void update() {
    if (resetColor) {
      appearance(focused ? Resource::focusedColor : Resource::unfocusedColor);
    }
  }

  friend class Button1;

protected:
  virtual void appearance(sf::Color color) = 0;
};