#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class Button : public sf::Drawable {
protected:
  static const sf::Color pressedColor;
  static const sf::Color unpressedColor;

  bool focused = false;
  bool started = false;
  bool activate = false;

  std::function<void()> action;

  sf::RectangleShape body;

public:
  template <typename Func> Button(Func func) {
    action = func;

    body.setOutlineThickness(3);
    body.setFillColor(sf::Color(0, 0, 0, 0));

    updateAppearance(unpressedColor);
  }

  virtual void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseMoved) {
      if (body.getGlobalBounds().contains(event.mouseMove.x,
                                          event.mouseMove.y)) {
        focused = true;
      } else {
        focused = false;
      }
    }

    if (focused) {
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          started = true;
        }
      }

      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          activate = started;
          started = false;
        }
      }
    }
  }

  virtual void update() {
    updateAppearance(focused ? pressedColor : unpressedColor);

    if (activate) {
      action();
      activate = false;
    }
  }

  virtual void setUnfocus() { updateAppearance(unpressedColor); }

  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
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
