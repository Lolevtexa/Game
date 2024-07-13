#pragma once
#include "button.hpp"
#include "resource.hpp"
#include <SFML/Graphics/Text.hpp>

class TextButton : public Button {
protected:
  sf::Text text;

public:
  template <typename Func>
  TextButton(std::string text, Func func) : Button(func) {
    this->text.setFont(Resource::defaultFont);
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

  void setText(std::string text) {
    this->text.setString(text);
    this->text.setPosition(body.getPosition().x + body.getSize().x / 2 -
                               this->text.getGlobalBounds().width / 2,
                           body.getPosition().y + body.getSize().y / 2 -
                               this->text.getGlobalBounds().height / 2);
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