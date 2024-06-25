#pragma once
#include "textButton.hpp"
#include <SFML/Graphics/Font.hpp>
#include <iostream>

class MainScene : public sf::Drawable {
private:
  const int buttonWidth = 150;
  const int buttonHeight = 50;
  const int buttonIndent = 10;

  std::vector<Button *> mainButtons;

  int settingsPage = 0;
  std::vector<std::vector<Button *>> settingsButtons{4};

public:
  MainScene(std::function<void()> exitFunc) {
    addMainButton("New game", [this]() { setSettingsPage(1); });
    addMainButton("Load game", [this]() { setSettingsPage(2); });
    addMainButton("Settings", [this]() { setSettingsPage(3); });
    addMainButton("Exit", exitFunc);
    setMainButtonsBound();

    addSettingsButton(
        "Start game", []() { std::cout << "Start game" << std::endl; }, 1);
    addSettingsButton("Back", [this]() { setSettingsPage(0); }, 1);
    addSettingsButton(
        "Full screen", []() { std::cout << "Full screen" << std::endl; }, 2);
    addSettingsButton("Back", [this]() { setSettingsPage(0); }, 2);
    addSettingsButton("Back", [this]() { setSettingsPage(0); }, 3);
    setSettingsButtonsBound();
  }

  void eventProcessing(sf::Event event) {
    for (auto &button : mainButtons) {
      button->eventProcessing(event);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->eventProcessing(event);
    }
  }

  void update() {
    for (auto &button : mainButtons) {
      button->update();
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->update();
    }
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    for (auto &button : mainButtons) {
      target.draw(*button, states);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      target.draw(*button, states);
    }
  }

private:
  void setSettingsPage(int page) {
    settingsPage = settingsPage == page ? 0 : page;
  }

  void addMainButton(const std::string &text, std::function<void()> func) {
    mainButtons.emplace_back(new TextButton(text, func));
  }

  void setMainButtonsBound() {
    for (int i = 0; i < mainButtons.size(); i++) {
      mainButtons[i]->setBound(buttonIndent,
                               buttonIndent + i * (buttonHeight + buttonIndent),
                               buttonWidth, buttonHeight);
    }
  }

  void addSettingsButton(const std::string &text, std::function<void()> func,
                         int page) {
    settingsButtons[page].emplace_back(new TextButton(text, func));
  }

  void setSettingsButtonsBound() {
    for (int i = 0; i < settingsButtons.size(); i++) {
      for (int j = 0; j < settingsButtons[i].size(); j++) {
        settingsButtons[i][j]->setBound(buttonIndent * 2 + buttonWidth,
                                        buttonIndent +
                                            j * (buttonHeight + buttonIndent),
                                        buttonWidth, buttonHeight);
      }
    }
  }
};