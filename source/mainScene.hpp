#pragma once
#include "textButton.hpp"
#include "radioButton.hpp"
#include "sliderButton.hpp"
#include <SFML/Audio/Music.hpp>
#include <iostream>
#include <vector>

class MainScene : public sf::Drawable {
private:
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

    addSettingsTextButton(
        "Start game", []() { std::cout << "Start game" << std::endl; }, 1);
    addSettingsTextButton("Back", [this]() { setSettingsPage(0); }, 1);

    addSettingsTextButton("Back", [this]() { setSettingsPage(0); }, 2);

    addSettingsRadioButton({"Resolution1", "Resolution2"},
                           {[]() { std::cout << "Resolution1" << std::endl; },
                            []() { std::cout << "Resolution2" << std::endl; }},
                           3);
    addSettingsRadioButton({"Resolution3", "Resolution4", "Resolution5"},
                           {[]() { std::cout << "Resolution3" << std::endl; },
                            []() { std::cout << "Resolution4" << std::endl; },
                            []() { std::cout << "Resolution5" << std::endl; }},
                           3);
    addSettingsSliderButton([]() {}, 10, 3);
    addSettingsTextButton("Back", [this]() { setSettingsPage(0); }, 3);

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

    backgroundMusic->setVolume(settingsButtons[3][2]->getValue());
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
      mainButtons[i]->setBound(
          Resource::buttonIndent,
          Resource::buttonIndent +
              i * (Resource::buttonHeight + Resource::buttonIndent));
    }
  }

  void addSettingsTextButton(const std::string &text,
                             std::function<void()> func, int page) {
    settingsButtons[page].emplace_back(new TextButton(text, func));
  }

  void addSettingsRadioButton(const std::vector<std::string> texts,
                              std::vector<std::function<void()>> funcs,
                              int page) {
    std::vector<Button *> subButtons;
    for (int i = 0; i < texts.size(); i++) {
      subButtons.emplace_back(new TextButton(texts[i], funcs[i]));
    }

    settingsButtons[page].emplace_back(new RadioButton(subButtons));
  }

  void addSettingsSliderButton(std::function<void()> func, int defaultValue,
                               int page) {
    settingsButtons[page].emplace_back(new SliderButton(func, defaultValue));
  }

  void updateSettingsButtonsBound() {
    for (int i = 0; i < settingsButtons.size(); i++) {
      int deltaY = 0;
      for (int j = 0; j < settingsButtons[i].size(); j++) {
        settingsButtons[i][j]->setBound(Resource::buttonIndent +
                                            Resource::buttonWidth +
                                            Resource::buttonIndent,
                                        Resource::buttonIndent + deltaY);
        deltaY += settingsButtons[i][j]->getBound().getSize().y +
                  Resource::buttonIndent;
      }
    }
  }
};