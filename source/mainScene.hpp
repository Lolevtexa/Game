#pragma once
#include "button.hpp"
#include <iostream>

class MainScene : public sf::Drawable {
private:
  std::vector<Button> buttons;
  std::vector<sf::Drawable *> mainButtons;
  std::vector<sf::Drawable *> settingsButtons;

public:
  MainScene() {
    buttons.push_back(Button("../assets/pictures/play.png", 100, 100, 100, 100, []() {
      std::cout << "Play button pressed" << std::endl;
    }));
    buttons.push_back(Button("../assets/pictures/settings.png", 100, 300, 100, 100, []() {
      std::cout << "Settings button pressed" << std::endl;
    }));
    buttons.push_back(Button("../assets/pictures/exit.png", 100, 500, 100, 100, []() {
      std::cout << "Exit button pressed" << std::endl;
    }));
    // Create main buttons
    for (auto &button : buttons) {
      mainButtons.push_back(&button);
    }
  }

  void eventProcessing(sf::Event event) {
    for (auto &button : buttons) {
      button.eventProcessing(event);
    }
  }

  void update() {
    for (auto &button : buttons) {
      button.update();
    }
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    for (auto &button : mainButtons) {
      target.draw(*button, states);
    }

    for (auto &button : settingsButtons) {
      target.draw(*button, states);
    }
  }
};