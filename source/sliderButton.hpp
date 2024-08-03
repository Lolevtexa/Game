#include "button.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <algorithm>

class SliderButton : public Button {
protected:
  float minValue = 0;
  float maxValue = 100;
  float value;

  bool mouseMoved = false;
  float mouseOffset = 0;

  sf::CircleShape slider;
  sf::RectangleShape sliderLineLeft;
  sf::RectangleShape sliderLineRight;

public:
  template <typename Func>
  SliderButton(Func func, int defaultValue = 10)
      : Button(func), value(defaultValue) {
    slider.setOutlineThickness(3);
    sliderLineLeft.setOutlineThickness(3);
    sliderLineRight.setOutlineThickness(3);

    updateAppearance(unpressedColor);
  }

  void eventProcessing(sf::Event event) {
    mouseMoved = false;
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (slider.getGlobalBounds().contains(event.mouseButton.x,
                                              event.mouseButton.y)) {
          updateAppearance(pressedColor);
          isActive = true;
        }
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (isActive) {
          isActive = false;
        }
        updateAppearance(unpressedColor);
      }
    } else if (event.type == sf::Event::MouseMoved) {
      mouseMoved = true;
      mouseOffset = event.mouseMove.x;
    }
  }

  void update() {
    if (isActive && mouseMoved) {
      float x = mouseOffset;
      float minX = sliderLineLeft.getPosition().x;
      float maxX = sliderLineRight.getPosition().x;
      x = std::max(x, minX);
      x = std::min(x, maxX);
      float scaleFactor = (x - minX) / (maxX - minX);
      value = minValue + scaleFactor * (maxValue - minValue);
      action(value);
    }
  }

  float getValue() { return value; }

  void setBound(float x, float y, float width = Resource::buttonWidth,
                float height = Resource::buttonHeight,
                float indent = Resource::buttonIndent) {
    Button::setBound(x, y, width, height, indent);

    float radius = height / 2.f - indent;
    float scaleFactor = (value - minValue) / (maxValue - minValue);
    slider.setRadius(radius);
    slider.setOrigin(radius, radius);
    slider.setPosition(x + indent + radius +
                           (width - 2 * indent - 2 * radius) * scaleFactor,
                       y + height / 2.f);

    sliderLineLeft.setSize(sf::Vector2f(
        std::max((width - 2 * indent - 2 * radius) * scaleFactor - radius, 0.f),
        0));
    sliderLineLeft.setPosition(x + indent + radius, y + height / 2.f);

    sliderLineRight.setSize(sf::Vector2f(
        std::min((width - 2 * indent - 2 * radius) * (scaleFactor - 1.f) +
                     radius,
                 0.f),
        0));
    sliderLineRight.setPosition(x + width - indent - radius, y + height / 2.f);
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    Button::draw(target, states);
    target.draw(slider, states);
    target.draw(sliderLineLeft, states);
    target.draw(sliderLineRight, states);
  }

protected:
  void updateAppearance(sf::Color color) {
    Button::updateAppearance(color);

    slider.setOutlineColor(color);
    sliderLineLeft.setOutlineColor(color);
    sliderLineRight.setOutlineColor(color);
  }
};