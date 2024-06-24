#pragma once
#include "SFML/Graphics/RenderTarget.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

class Button : public sf::Drawable {
protected:
  static const int unpressedAlpha = 127;
  static const int pressedAlpha = 255;

  bool isActive = false;

  sf::RectangleShape body;
  std::function<void()> eventButtonPressed;

public:
  Button(std::function<void()> func) {
    body.setOutlineThickness(1);

    eventButtonPressed = func;
  }

  virtual void eventProcessing(sf::Event event) = 0;

  void update() {
    if (isActive) {
      eventButtonPressed();
      isActive = false;
    }
  }

  virtual void setBound(float x, float y, float width, float height) = 0;
};
