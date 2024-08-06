#pragma once
#include "activable.hpp"

class Button1 : public Activable {
protected:
  bool started = false;
  bool activate = false;

  std::function<void()> action;

  std::vector<Activable *> elements;

  sf::RectangleShape outLine;

public:
  template <typename Action>
  Button1(Action action, std::vector<Activable *> elements):
  action(action), elements(elements) {
    outLine.setOutlineThickness(3);

    updateAppearance(Resource::unfocusedColor);
  }

  ~Button1() {
    while (elements.size()) {
      delete *elements.rbegin();
      elements.pop_back();
    }
  }

  void eventProcessing(sf::Event event) {
    Activable::eventProcessing(event);

    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        started = focused;
      }
    }

    if (event.type == sf::Event::MouseButtonReleased) {        
      if (event.mouseButton.button == sf::Mouse::Left) {
        activate = focused && started;
        started = false;
      }
    }
  }

  void update() {
    Activable::update();

    if (activate) {
      action();
      activate = false;
    }
  }

  void setBound(float x, float y, float width, float height,
                        float indent) {
    Activable::setBound(x, y, width, height, indent);

    outLine.setPosition(x, y);
    outLine.setSize(sf::Vector2f(width, height));

    for (auto &element : elements) {
      element->setBound(x, y, width, height, indent);
    }
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }

    target.draw(outLine, states);
  }

protected:
  void updateAppearance(sf::Color color) {
    for (auto &element : elements) {
      element->updateAppearance(color);
    }

    outLine.setOutlineColor(color);
  }
};
