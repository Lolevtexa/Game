#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <string>


class Button : public sf::Drawable {
private:
  static const int unpressedAlpha = 127;
  static const int pressedAlpha = 255;

  std::string filename;
  std::string text;
  bool isActive = false;

  sf::RectangleShape button;
  sf::Image icon;
  sf::Texture texture;

  std::function<void()> eventButtonPressed;

public:
  template <typename Func>
  Button(const std::string &filename, int x, int y, int width, int height,
         Func func) {
    this->filename = filename;
    eventButtonPressed = func;

    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);

    loadSprite(unpressedAlpha);
  }

  Button(const Button &other)
      : filename(other.filename), eventButtonPressed(other.eventButtonPressed),
        button(other.button), icon(other.icon), texture(other.texture) {
    button.setTexture(&texture);
  }

  void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (button.getGlobalBounds().contains(event.mouseButton.x,
                                              event.mouseButton.y)) {
          loadSprite(pressedAlpha);
        }
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (button.getGlobalBounds().contains(event.mouseButton.x,
                                              event.mouseButton.y)) {
          isActive = true;
        }

        loadSprite(unpressedAlpha);
      }
    }
  }

  void update() {
    if (isActive) {
      eventButtonPressed();
      isActive = false;
    }
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(button, states);
  }

private:
  void loadSprite(int alpha) {
    if (icon.loadFromFile(filename)) {
      icon.createMaskFromColor(sf::Color::Black, alpha);
      if (texture.loadFromImage(icon)) {
        button.setTexture(&texture);
        return;
      }
    }
  }
};