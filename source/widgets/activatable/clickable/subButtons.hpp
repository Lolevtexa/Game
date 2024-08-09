#pragma once
#include "button.hpp"

class SubButtons : virtual public Clickable {
protected:
  bool selected = false;
  bool unselect = true;

  int buttonNumber = 0;
  std::vector<Button *> subButtons;

public:
  SubButtons(std::vector<Button *> subButtons) : subButtons(subButtons) {
    for (int i = 0; i < this->subButtons.size(); i++) {
      std::function<void()> action = this->subButtons[i]->action;
      this->subButtons[i]->action = [this, i, action]() {
        buttonNumber = i;
        action();
      };
      this->subButtons[i]->appearance(Resource::unfocusedColor);
    }
  }

  ~SubButtons() {
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
    } else {
      subButtons[buttonNumber]->Activatable::eventProcessing(event);
    }

    Clickable::eventProcessing(event);
  }

  void update() {
    if (selected) {
      for (auto &button : subButtons) {
        button->update();
      }
    } else {
      subButtons[buttonNumber]->Activatable::update();
    }

    if (unselect) {
      for (auto &button : subButtons) {
        button->appearance(Resource::unfocusedColor);
      }
      unselect = false;
    }

    if (activate) {
      selected = !selected;
      unselect = selected == false;
      activate = false;
    }
  }

  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;
    if (selected) {
      for (auto &button : subButtons) {
        button->setBound(x, y + deltaY, width, height, indent);
        deltaY += button->getBound().getSize().y + indent;
      }
      deltaY -= indent;
    } else {
      subButtons[buttonNumber]->setBound(x, y, width, height, indent);
      deltaY += subButtons[buttonNumber]->getBound().getSize().y;
    }

    Clickable::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (selected) {
      for (auto &button : subButtons) {
        target.draw(*button, states);
      }
    } else {
      target.draw(*subButtons[buttonNumber], states);
    }
  }

protected:
  void appearance(sf::Color color) {}
};
