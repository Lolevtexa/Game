#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

class Button : public sf::Drawable {
protected:
  static const sf::Color pressedColor;
  static const sf::Color unpressedColor;

  bool isActive = false;

  sf::RectangleShape body;
  std::function<void()> action;

public:
  Button(std::function<void()> func) {
    action = func;

    body.setOutlineThickness(3);
    body.setFillColor(sf::Color(0, 0, 0, 0));

    updateAppearance(unpressedColor);
  }

  Button &operator=(const Button &button) {
    body = button.body;
    action = button.action;
    isActive = button.isActive;

    return *this;
  }

  Button(const Button &button) {
    body = button.body;
    action = button.action;
    isActive = button.isActive;
  }

  virtual void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (body.getGlobalBounds().contains(event.mouseButton.x,
                                            event.mouseButton.y)) {
          updateAppearance(pressedColor);
        }
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (body.getGlobalBounds().contains(event.mouseButton.x,
                                            event.mouseButton.y)) {
          isActive = true;
        }

        updateAppearance(unpressedColor);
      }
    }
  }

  virtual void update() {
    if (isActive) {
      action();
      isActive = false;
    }
  }

  virtual void setBound(int x, int y, int width, int height, int indent = 0) {
    body.setSize(sf::Vector2f(width, height));
    body.setPosition(x, y);
  }

  sf::RectangleShape getBound() { return body; }

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(body, states);
  }

  friend class TextButton;
  friend class RadioButton;

protected:
  virtual void updateAppearance(sf::Color color) {
    body.setOutlineColor(color);
  }
};

const sf::Color Button::pressedColor = []() { return sf::Color::Black; }();

const sf::Color Button::unpressedColor = []() {
  return sf::Color(127, 127, 127);
}();
