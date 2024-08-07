#pragma once
#include "../clickable.hpp"
#include "../outline.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class Button1 : virtual public Clickable, public AOutline {
protected:
  std::function<void()> action;

  std::vector<Activatable *> elements;

public:
  template <typename Action>
  Button1(Action action, std::vector<Activatable *> elements)
      : action(action), elements(elements) {
    appearance(Resource::unfocusedColor);
  }

  ~Button1() {
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

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    AOutline::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }

    AOutline::draw(target, states);
  }

  friend class RadioButton1;
  friend class SubButtons;

protected:
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }

    AOutline::appearance(color);
  }
};