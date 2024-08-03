#pragma once
#include "button.hpp"
#include <functional>
#include <vector>

class RadioButton : public Button {
protected:
  bool selected = false;

  int buttonNumber = 0;
  std::vector<Button *> subButtons;

public:
  RadioButton(std::vector<Button *> subButtons)
      : Button([this]() { selected = !selected; }), subButtons(subButtons) {
    for (int i = 0; i < this->subButtons.size(); i++) {
      std::function<void()> action = this->subButtons[i]->action;
      this->subButtons[i]->action = [this, i, action]() {
        buttonNumber = i;
        action();
      };
    }

    updateAppearance(unpressedColor);
  }

  ~RadioButton() {
    while (subButtons.size()) {
      delete *(subButtons.rbegin());
      subButtons.pop_back();
    }
  }

  void eventProcessing(sf::Event event) {
    if (selected) {
      for (auto &button : subButtons) {
        button->eventProcessing(event);
      }
    }

    Button::eventProcessing(event);
  }

  void update() {
    if (selected) {
      for (auto &button : subButtons) {
        button->update();
      }
    }

    Button::update();
  }

  void setBound(int x, int y, int width, int height, int indent) {
    if (selected) {
      Button::setBound(x, y, width,
                       height * subButtons.size() +
                           indent * (subButtons.size() - 1));

      int deltaY = 0;
      for (int i = 0; i < subButtons.size(); i++) {
        subButtons[i]->setBound(x, y + deltaY, width, height, indent);
        deltaY += subButtons[i]->getBound().getSize().y + indent;
      }
    } else {
      Button::setBound(x, y, width, height);

      subButtons[buttonNumber]->setBound(x, y, width, height, indent);
    }
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (selected) {
      for (auto &button : subButtons) {
        target.draw(*button, states);
      }
    } else {
      target.draw(*subButtons[buttonNumber], states);
    }

    Button::draw(target, states);
  }

private:
  void updateAppearance(sf::Color color) { Button::updateAppearance(color); }
};
