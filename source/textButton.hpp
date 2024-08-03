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

    updateAppearance(unpressedColor);
  }

  TextButton &operator=(const TextButton &button) {
    body = button.body;
    text = button.text;
    action = button.action;
    isActive = button.isActive;

    return *this;
  }

  TextButton(const TextButton &button) : Button(button) {
    body = button.body;
    text = button.text;
    action = button.action;
    isActive = button.isActive;
  }

  void setBound(int x, int y, int width, int height, int indent = 0) {
    Button::setBound(x, y, width, height);

    text.setPosition(x + width / 2 - text.getGlobalBounds().width / 2,
                     y + height / 2 - text.getGlobalBounds().height / 2);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    Button::draw(target, states);
    target.draw(text, states);
  }

private:
  void updateAppearance(sf::Color color) {
    Button::updateAppearance(color);
    text.setFillColor(color);
  }
};