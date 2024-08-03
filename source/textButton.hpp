#pragma once
#include "button.hpp"
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
  
  void setBound(float x, float y, float width = Resource::buttonWidth,
                float height = Resource::buttonHeight,
                float indent = Resource::buttonIndent) {
    Button::setBound(x, y, width, height, indent);

    text.setPosition(x + width / 2.f - text.getGlobalBounds().width / 2.f,
                     y + height / 2.f - text.getGlobalBounds().height / 2.f);
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