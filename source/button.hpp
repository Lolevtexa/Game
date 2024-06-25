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

  void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (body.getGlobalBounds().contains(event.mouseButton.x,
                                            event.mouseButton.y)) {
          updateAppearance(pressedAlpha);
        }
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (body.getGlobalBounds().contains(event.mouseButton.x,
                                            event.mouseButton.y)) {
          isActive = true;
        }

        updateAppearance(unpressedAlpha);
      }
    }
  }

  void update() {
    if (isActive) {
      eventButtonPressed();
      isActive = false;
    }
  }

  virtual void setBound(float x, float y, float width, float height) = 0;

protected:
  virtual void updateAppearance(int alpha) = 0;
};
