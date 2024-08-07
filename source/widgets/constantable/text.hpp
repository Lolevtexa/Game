#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

#define TEXT_BY_KEY 0
#define TEXT_BY_STRING 1

class CText : virtual public Constantable {
protected:
  int type;

  std::vector<std::string> localizationKeys;
  std::wstring text;

  std::vector<sf::Text> drawableText;

public:
  CText(const std::wstring &string)
      : text(removeOverspaces(string + L" ")), type(TEXT_BY_STRING) {}

  CText(const std::vector<std::string> &localizationKeys)
      : localizationKeys(localizationKeys), type(TEXT_BY_KEY),
        text(removeOverspaces(unpackingLocalization(localizationKeys) + L" ")) {
  }

  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    if (body.width == 0.f) {
      body.width = width;
      updateDrawableText();
    }

    float deltaY = 0;
    float minWidth = width;
    for (auto &line : drawableText) {
      line.setPosition(x + width / 2 - line.getGlobalBounds().width / 2,
                       y + deltaY);
      deltaY += line.getGlobalBounds().height + line.getLineSpacing();
      minWidth = std::max(minWidth, line.getGlobalBounds().width);
    }

    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  void resetString() {
    if (type != TEXT_BY_KEY) {
      return;
    }

    text = removeOverspaces(unpackingLocalization(localizationKeys) + L" ");
    updateDrawableText();
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &line : drawableText) {
      target.draw(line, states);
    }
  }

protected:
  static std::wstring removeOverspaces(const std::wstring &string) {
    std::wstring stringWithoutSpaces;
    bool isWord = false;

    for (auto &c : string) {
      if (c != L' ' && c != L'\n') {
        stringWithoutSpaces += c;
        isWord = true;
      } else if ((c == L' ' || c == L'\n') && isWord) {
        stringWithoutSpaces += L' ';
        isWord = false;
      }
    }

    return stringWithoutSpaces;
  }

  static bool doStringPlaced(const std::wstring &string, const float &width) {
    sf::Text tmp;
    tmp.setFont(Resource::defaultFont);
    tmp.setCharacterSize(Resource::characterSize);
    tmp.setString(string);

    return tmp.getGlobalBounds().width <= width;
  }

  static std::wstring
  unpackingLocalization(const std::vector<std::string> &localizationKeys) {
    nlohmann::json j = Resource::localization;

    for (auto &key : localizationKeys) {
      j = j[key];
    }

    return utf8_to_wstring(j);
  }

  void drawableTextEmplaceBack(const std::wstring &string) {
    drawableText.emplace_back();
    drawableText.rbegin()->setFont(Resource::defaultFont);
    drawableText.rbegin()->setFillColor(Resource::focusedColor);
    drawableText.rbegin()->setCharacterSize(Resource::characterSize);
    drawableText.rbegin()->setLineSpacing(Resource::lineSpacing);
    drawableText.rbegin()->setString(string);
  }

  void updateDrawableText() {
    std::wstring word, string;
    drawableText.clear();

    for (auto &c : text) {
      if (c != L' ') {
        word += c;
      }
      if (c == L' ') {
        if (doStringPlaced(string + L" " + word, body.width)) {
          string += (string.size() ? L" " : L"") + word;
          word.clear();
        } else {
          if (!string.size()) {
            string = word;
            word.clear();
          }

          drawableTextEmplaceBack(string);

          string = word;
          word.clear();
        }
      }
    }
    if (string.size()) {
      drawableTextEmplaceBack(string);
    }
  }
};