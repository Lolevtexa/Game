#pragma once
#include "radioButton.hpp"
#include "resource.hpp"
#include "textButton.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Music.hpp>
#include <iostream>
#include <vector>

class MainScene : public sf::Drawable {
private:
  const int buttonWidth = 150;
  const int buttonHeight = 50;
  const int buttonIndent = 10;

  std::vector<Button *> mainButtons;

  int settingsPage = 0;
  std::vector<std::vector<Button *>> settingsButtons{4};

  sf::Music *backgroundMusic = Resource::loadBackgroundMusic();

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

    addSettingsButton("Back", [this]() { setSettingsPage(0); }, 2);

    addSettingsResolutionButton(
        {"Resolution1", "Resolution2"},
        {[]() { std::cout << "Resolution1" << std::endl; },
         []() { std::cout << "Resolution2" << std::endl; }},
        3);
    addSettingsResolutionButton(
        {"Resolution3", "Resolution4", "Resolution5"},
        {[]() { std::cout << "Resolution3" << std::endl; },
         []() { std::cout << "Resolution4" << std::endl; },
         []() { std::cout << "Resolution5" << std::endl; }},
        3);
    addSettingsButton("Back", [this]() { setSettingsPage(0); }, 3);
  
    backgroundMusic->setLoop(true);
    backgroundMusic->setVolume(10);
    backgroundMusic->play();
  }

  ~MainScene() {
    while (mainButtons.size()) {
      delete *(mainButtons.rbegin());
      mainButtons.pop_back();
    }

    while (settingsButtons.size()) {
      while ((*settingsButtons.rbegin()).size()) {
        delete *((*settingsButtons.rbegin()).rbegin());
        (*settingsButtons.rbegin()).pop_back();
      }
      settingsButtons.pop_back();
    }

    delete backgroundMusic;
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

    updateSettingsButtonsBound();
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
                               buttonWidth, buttonHeight, buttonIndent);
    }
  }

  void addSettingsButton(const std::string &text, std::function<void()> func,
                         int page) {
    settingsButtons[page].emplace_back(new TextButton(text, func));
  }

  void addSettingsButton(const std::string &text, std::function<void()> func,
                         int page, int index) {
    settingsButtons[page].insert(settingsButtons[page].begin() + index,
                                 new TextButton(text, func));
  }

  void removeSettingsButton(int page, int index) {
    settingsButtons[page].erase(settingsButtons[page].begin() + index);
  }

  void addSettingsResolutionButton(const std::vector<std::string> texts,
                                   std::vector<std::function<void()>> funcs,
                                   int page) {
    std::vector<Button *> subButtons;
    for (int i = 0; i < texts.size(); i++) {
      subButtons.emplace_back(new TextButton(texts[i], funcs[i]));
    }

    settingsButtons[page].emplace_back(new RadioButton(subButtons));
  }

  void updateSettingsButtonsBound() {
    for (int i = 0; i < settingsButtons.size(); i++) {
      int deltaY = 0;
      for (int j = 0; j < settingsButtons[i].size(); j++) {
        settingsButtons[i][j]->setBound(
            buttonIndent + buttonWidth + buttonIndent, buttonIndent + deltaY,
            buttonWidth, buttonHeight, buttonIndent);
        deltaY += settingsButtons[i][j]->getBound().getSize().y + buttonIndent;
      }
    }
  }
};