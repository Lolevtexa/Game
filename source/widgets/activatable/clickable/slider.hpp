#include "../clickable.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Slider : public Clickable {
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
  Slider(int defaultValue = 50) : value(defaultValue) {
    slider.setOutlineThickness(3);
    sliderLineLeft.setOutlineThickness(3);
    sliderLineRight.setOutlineThickness(3);

    appearance(Resource::unfocusedColor);
  }

  void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseMoved) {
      mouseMoved = true;
      mouseOffset = event.mouseMove.x;
    }

    Clickable::eventProcessing(event);
  }

  void update() {
    if (started && mouseMoved) {
      float x = mouseOffset;
      float minX = sliderLineLeft.getPosition().x;
      float maxX = sliderLineRight.getPosition().x;
      x = std::max(x, minX);
      x = std::min(x, maxX);
      float scaleFactor = (x - minX) / (maxX - minX);
      value = minValue + scaleFactor * (maxValue - minValue);
      mouseMoved = false;
    }
  }

  void setBound(float x, float y, float width, float height, float indent) {
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

    Clickable::setBound(x, y, width, height, indent);
  }

  int getValue() { return value; }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sliderLineLeft, states);
    target.draw(sliderLineRight, states);
    target.draw(slider, states);
  }

protected:
  void appearance(sf::Color color) {
    slider.setOutlineColor(color);
    sliderLineLeft.setOutlineColor(color);
    sliderLineRight.setOutlineColor(Resource::unfocusedColor);
  }
};