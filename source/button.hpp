#pragma once
#include "SFML/Graphics/Drawable.hpp"
#include "resource.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

class Button : public sf::Drawable {
protected:
  static const sf::Color pressedColor;
  static const sf::Color unpressedColor;

  bool started = false;
  bool isActive = false;

  std::function<void(int)> action;

  sf::RectangleShape body;
  std::vector<sf::Drawable *> drawables;

public:
  template <typename Func>
  Button(Func func) {
    action = func;

    body.setOutlineThickness(3);
    body.setFillColor(sf::Color(0, 0, 0, 0));

    updateAppearance(unpressedColor);
  }

  virtual void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (body.getGlobalBounds().contains(event.mouseButton.x,
                                            event.mouseButton.y)) {
          started = true;
          updateAppearance(pressedColor);
        }
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (body.getGlobalBounds().contains(event.mouseButton.x,
                                            event.mouseButton.y)) {
          if (started) {
            isActive = true;
            started = false;
          }
        }

        updateAppearance(unpressedColor);
      }
    }
  }

  virtual void update() {
    if (isActive) {
      action(0);
      isActive = false;
    }
  }

  virtual void setBound(float x, float y, float width = Resource::buttonWidth,
                        float height = Resource::buttonHeight,
                        float indent = Resource::buttonIndent) {
    body.setSize(sf::Vector2f(width, height));
    body.setPosition(x, y);
  }

  sf::RectangleShape getBound() { return body; }

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(body, states);

    for (auto drawable : drawables) {
      target.draw(*drawable, states);
    }
  }

  // virtual float getValue() { return 0; }

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
