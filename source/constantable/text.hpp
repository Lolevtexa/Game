#pragma once
#include "../constantable.hpp"
#include "../resource.hpp"
#include <SFML/Graphics/Text.hpp>

#define ADD_TEXT_BY_KEY 0
#define ADD_TEXT_BY_STRING 1

class CText : virtual public Constantable {
protected:
  int type;

  std::vector<std::string> localizationKeys;
  std::wstring text;

  std::vector<sf::Text> drawableText;

public:
  CText(const std::wstring &string, int type = ADD_TEXT_BY_STRING)
      : text(removeOverspaces(string + L" ")), type(type) {}

  CText(const std::vector<std::string> &localizationKeys,
        int type = ADD_TEXT_BY_KEY)
      : localizationKeys(localizationKeys), type(type),
        text(removeOverspaces(unpackingLocalization(localizationKeys) + L" ")) {
  }

  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    std::wstring word, string;
    for (auto &c : text) {
      if (c != L' ') {
        word += c;
      }
      if (c == L' ') {
        if (doStringPlaced(string + L" " + word, width)) {
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

    float deltaY = 0;
    for (auto &line : drawableText) {
      line.setPosition(x + width / 2 - line.getGlobalBounds().width / 2,
                       y + deltaY);
      deltaY += line.getGlobalBounds().height + line.getLineSpacing();
    }

    Bound::setBound(x, y, width, deltaY, indent);
  }

  void resetString() {
    if (type != ADD_TEXT_BY_KEY) {
      return;
    }

    text = removeOverspaces(unpackingLocalization(localizationKeys) + L" ");
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

  void drawableTextEmplaceBack(const std::wstring &string) {
    drawableText.emplace_back();
    drawableText.rbegin()->setFont(Resource::defaultFont);
    drawableText.rbegin()->setFillColor(Resource::focusedColor);
    drawableText.rbegin()->setCharacterSize(Resource::characterSize);
    drawableText.rbegin()->setLineSpacing(Resource::lineSpacing);
    drawableText.rbegin()->setString(string);
  }

  static bool doStringPlaced(const std::wstring &string, const float &width) {
    sf::Text tmp;
    tmp.setFont(Resource::defaultFont);
    tmp.setCharacterSize(Resource::characterSize);
    tmp.setString(string);

    return tmp.getGlobalBounds().width <= width;
  }

  static std::wstring utf8_to_wstring(const std::string &str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
  }

  static std::wstring
  unpackingLocalization(const std::vector<std::string> &localizationKeys) {
    nlohmann::json j = Resource::localization;

    for (auto &key : localizationKeys) {
      j = j[key];
    }

    return utf8_to_wstring(j);
  }
};