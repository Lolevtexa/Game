#pragma once
#include "inactivable.hpp"
#include "resource.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class Activable : public Inactivable {
protected:
  bool focused = false;
  bool resetColor = true;

public:
  virtual void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseMoved) {
      if (body.contains(event.mouseMove.x, event.mouseMove.y)) {
        resetColor = focused == false;
        focused = true;
      } else {
        resetColor = focused == true;
        focused = false;
      }
    }
  }

  virtual void update() {
    if (resetColor) {
      updateAppearance(focused ? Resource::focusedColor
                               : Resource::unfocusedColor);
    }
  }

  friend class Button1;

protected:
  virtual void updateAppearance(sf::Color color) = 0;
};