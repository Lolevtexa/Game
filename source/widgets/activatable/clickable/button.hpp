#pragma once
#include "../clickable.hpp"
#include "../outline.hpp"

class Button : virtual public Clickable, public AOutline {
protected:
  std::function<void()> action;

  std::vector<Activatable *> elements;

public:
  template <typename Action>
  Button(Action action, std::vector<Activatable *> elements)
      : action(action), elements(elements) {
    appearance(Resource::unfocusedColor);
  }

  ~Button() {
    while (elements.size()) {
      delete *elements.rbegin();
      elements.pop_back();
    }
  }

  void eventProcessing(sf::Event event) {
    for (auto &element : elements) {
      element->eventProcessing(event);
    }

    Clickable::eventProcessing(event);
  }

  void update() {
    for (auto &element : elements) {
      element->update();
    }

    if (activate) {
      action();
      activate = false;
    }

    Clickable::update();
  }

  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;
    float minWidth = width;

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
      minWidth = std::max(minWidth, element->getBound().width);
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    for (auto &element : elements) {
      element->setBound(element->getBound().left, element->getBound().top, minWidth, element->getBound().height, indent);
    }

    AOutline::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }

    AOutline::draw(target, states);
  }

  friend class RadioButton;
  friend class SubButtons;

protected:
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }

    AOutline::appearance(color);
  }
};
