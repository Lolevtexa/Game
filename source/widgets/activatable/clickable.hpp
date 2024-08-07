#pragma once
#include "../activatable.hpp"
#include <SFML/Window/Event.hpp>

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
        activate = focused && started;
        started = false;
      }
    }

    Activatable::eventProcessing(event);
  }
};