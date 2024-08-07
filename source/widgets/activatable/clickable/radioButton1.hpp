#pragma once
#include "button1.hpp"
#include "subButtons.hpp"

class RadioButton1 : public Button1, public SubButtons {
public:
  template <typename Action>
  RadioButton1(Action action, std::vector<Activatable *> elements,
               std::vector<Button1 *> subButtons)
      : Button1(action, elements), SubButtons(subButtons) {
    appearance(Resource::unfocusedColor);
  }

  void eventProcessing(sf::Event event) {
    if (selected) {
      for (auto &button : subButtons) {
        button->eventProcessing(event);
      }
    } else {
      subButtons[buttonNumber]->Activatable::eventProcessing(event);
    }

    for (auto &element : elements) {
      element->eventProcessing(event);
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

    for (auto &element : elements) {
      element->update();
    }

    if (activate) {
      selected = !selected;
      unselect = selected == false;
      activate = false;
      action();
    }

    Clickable::update();
  }

  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    if (selected) {
      for (auto &button : subButtons) {
        button->setBound(x, y + deltaY, width, height, indent);
        deltaY += button->getBound().getSize().y + indent;
      }
      deltaY -= indent;
    } else {
      subButtons[buttonNumber]->setBound(x, y + deltaY, width, height, indent);
      deltaY += subButtons[buttonNumber]->getBound().getSize().y;
    }

    AOutline::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    SubButtons::draw(target, states);
    Button1::draw(target, states);
  }

private:
  void appearance(sf::Color color) { Button1::appearance(color); }
};
