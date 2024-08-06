#pragma once
#include "activatable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class Button1 : public Activatable {
protected:
  bool started = false;
  bool activate = false;

  std::function<void()> action;

  std::vector<Activatable *> elements;

  sf::RectangleShape outLine;

public:
  template <typename Action>
  Button1(Action action, std::vector<Activatable *> elements)
      : action(action), elements(elements) {
    outLine.setOutlineThickness(3);
    outLine.setFillColor(Resource::transparentColor);

    appearance(Resource::unfocusedColor);
  }

  ~Button1() {
    while (elements.size()) {
      delete *elements.rbegin();
      elements.pop_back();
    }
  }

  void eventProcessing(sf::Event event) {
    Activatable::eventProcessing(event);

    if (focused) {
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          started = true;
        }
      }

      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          activate = started;
          started = false;
        }
      }
    }
  }

  void update() {
    Activatable::update();

    if (activate) {
      action();
      activate = false;
    }
  }

  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;
    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height;
    }

    outLine.setPosition(x, y);
    outLine.setSize(sf::Vector2f(width, deltaY));

    Activatable::setBound(x, y, width, deltaY, indent);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }

    target.draw(outLine, states);
  }

protected:
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }

    outLine.setOutlineColor(color);
  }
};
