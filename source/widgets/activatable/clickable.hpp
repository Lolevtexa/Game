#pragma once
#include "../activatable.hpp"

class Clickable : virtual public Activatable {
protected:
  bool started = false;
  bool activate = false;

public:
  virtual void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        started = focused;
      }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        activate =
            body.contains(event.mouseButton.x, event.mouseButton.y) && started;
        started = false;
      }
    }

    Activatable::eventProcessing(event);

    if (event.type == sf::Event::MouseMoved) {
      if (!body.contains(event.mouseMove.x, event.mouseMove.y) && started) {
        focused = true;
      }
    }
  }
};
