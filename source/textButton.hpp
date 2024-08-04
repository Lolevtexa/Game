#pragma once
#include "button.hpp"
#include <SFML/Graphics/Text.hpp>
#include <codecvt>

#define ADD_BUTTON_BY_KEY 0
#define ADD_BUTTON_BY_TEXT 1

class TextButton : public Button {
protected:
  int type;

  std::string localizationKey;
  std::wstring string;

  sf::Text text;

public:
  template <typename Func>
  TextButton(const std::string &localizationKey, Func func,
             int type = ADD_BUTTON_BY_KEY)
      : Button(func), type(type), localizationKey(localizationKey) {
    if (type == ADD_BUTTON_BY_KEY) {
      this->string =
          utf8_to_wstring(Resource::localization["buttons"][localizationKey]);
    } else {
      this->string = utf8_to_wstring(localizationKey);
    }

    this->text.setFont(Resource::defaultFont);
    this->text.setString(string);
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

  void resetString() {
    if (type != ADD_BUTTON_BY_KEY) {
      return;
    }

    this->string = utf8_to_wstring(
        Resource::localization["buttons"][this->localizationKey]);
    text.setString(string);
    text.setPosition(body.getGlobalBounds().getPosition().x +
                         body.getGlobalBounds().getSize().x / 2.f -
                         text.getGlobalBounds().width / 2.f,
                     body.getGlobalBounds().getPosition().y +
                         body.getGlobalBounds().getSize().y / 2.f -
                         text.getGlobalBounds().height / 2.f);
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

  static std::wstring utf8_to_wstring(const std::string &str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
  }
};