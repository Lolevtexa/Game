#pragma once
#include "button.hpp"
#include <SFML/Graphics/Text.hpp>
#include <cstdio>

class TextButton : public Button {
protected:
  sf::Text text;

public:
  template <typename Func>
  TextButton(const sf::Font &font, std::string text, Func func) 
    : Button(func) {
    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(24);

    updateAppearance(unpressedAlpha);
  }

  void setBound(float x, float y, float width, float height) {
    body.setSize(sf::Vector2f(width, height));
    body.setPosition(x, y);
    body.setFillColor(sf::Color::White);

    text.setPosition(x + width / 2 - text.getGlobalBounds().width / 2,
                     y + height / 2 - text.getGlobalBounds().height / 2);
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

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(body, states);
    target.draw(text, states);
  }

private:
  void updateAppearance(int alpha) {
    body.setOutlineColor(sf::Color(0, 0, 0, alpha));
    text.setFillColor(sf::Color(0, 0, 0, alpha));
  }
};