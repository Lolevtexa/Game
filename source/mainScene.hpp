#pragma once
// #include "imageButton.hpp"
#include "textButton.hpp"
#include <SFML/Graphics/Font.hpp>
#include <iostream>

class MainScene : public sf::Drawable {
private:
  const int buttonWidth = 150;
  const int buttonHeight = 50;
  const int buttonIndent = 10;

  // std::vector<ImageButtons> imageButtons;
  std::vector<TextButton> textButtons;
  std::vector<Button *> mainButtons;
  std::vector<Button *> settingsButtons;

public:
  MainScene(std::function<void()> exitFunc) {
    textButtons.emplace_back("New game", []() {
      std::cout << "Play button pressed" << std::endl;
    });
    textButtons.emplace_back("Load game", []() {
      std::cout << "Load button pressed" << std::endl;
    });
    textButtons.emplace_back("Settings", []() {
      std::cout << "Settings button pressed" << std::endl;
    });
    textButtons.emplace_back("Exit", exitFunc);

    for (int i = 0; i < textButtons.size(); i++) {
      mainButtons.push_back(&textButtons[i]);
    }

    setButtonsBound();
  }

  void eventProcessing(sf::Event event) {
    for (auto &button : textButtons) {
      button.eventProcessing(event);
    }
  }

  void update() {
    for (auto &button : textButtons) {
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

private:
  void setButtonsBound() {
    for (int i = 0; i < mainButtons.size(); i++) {
      mainButtons[i]->setBound(10, 10 + i * (buttonHeight + buttonIndent),
                               buttonWidth, buttonHeight);
    }
  }
};