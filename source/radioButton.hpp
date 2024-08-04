#pragma once
#include "button.hpp"

class RadioButton : public Button {
protected:
  bool selected = false;

  int buttonNumber = 0;
  std::vector<Button *> subButtons;

public:
  RadioButton(std::vector<Button *> subButtons)
      : Button([this](int) { selected = !selected; }), subButtons(subButtons) {
    for (int i = 0; i < this->subButtons.size(); i++) {
      std::function<void(int)> action = this->subButtons[i]->action;
      this->subButtons[i]->action = [this, i, action](int) {
        buttonNumber = i;
        action(0);
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

  void setBound(float x, float y, float width, float height, float indent) {
    if (selected) {
      Button::setBound(x, y, width,
                       height * subButtons.size() +
                           indent * (subButtons.size() - 1),
                       indent);

      int deltaY = 0;
      for (int i = 0; i < subButtons.size(); i++) {
        subButtons[i]->setBound(x, y + deltaY, width, height, indent);
        deltaY += subButtons[i]->getBound().getSize().y + indent;
      }
    } else {
      Button::setBound(x, y, width, height, indent);

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
