#pragma once
#include "textButton.hpp"
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include <vector>

class MainScene : public sf::Drawable {
private:
  const int buttonWidth = 150;
  const int buttonHeight = 50;
  const int buttonIndent = 10;

  std::vector<Button *> mainButtons;

  bool resolutionMenuOpened = false;
  // TODO: make it more flexible

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
    addSettingsButton("Back", [this]() { setSettingsPage(0); }, 2);
    // addSettingsButton(
    //     "Resolution1",
    //     [this]() {
    //       resolutionMenuOpened = !resolutionMenuOpened;
    //       if (resolutionMenuOpened) {
    //         addSettingsButton(
    //             "Resolution1",
    //             [this]() {
    //               static_cast<TextButton *>(settingsButtons[3][0])
    //                   ->setText("Resolution1");
    //               removeSettingsButton(3, 2);
    //               removeSettingsButton(3, 1);
    //               resolutionMenuOpened = !resolutionMenuOpened;
    //             },
    //             3);
    //         addSettingsButton(
    //             "Resolution2",
    //             [this]() {
    //               static_cast<TextButton *>(settingsButtons[3][0])
    //                   ->setText("Resolution2");
    //               removeSettingsButton(3, 2);
    //               removeSettingsButton(3, 1);
    //               resolutionMenuOpened = !resolutionMenuOpened;
    //             },
    //             3);
    //         setSettingsButtonsBound();
    //       } else {
    //         removeSettingsButton(3, 2);
    //         removeSettingsButton(3, 1);
    //       }
    //     },
    //     3);
    addSettingsResolutionButton({"Resolution1", "Resolution2"}, {}, 3);
    addSettingsResolutionButton({"Resolution3", "Resolution4"}, {}, 3);
    // addSettingsButton("Sound", []() { std::cout << "Sound" << std::endl; },
    // 3); addSettingsButton("Back", [this]() { setSettingsPage(0); }, 3);
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
                               buttonWidth, buttonHeight);
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
    int size = settingsButtons[page].size();
    addSettingsButton(
        texts[0],
        [this, texts, size, page]() {
          resolutionMenuOpened = !resolutionMenuOpened;
          if (resolutionMenuOpened) {
            for (int i = 0; i < texts.size(); i++) {
              addSettingsButton(
                  texts[i],
                  [this, i, texts, size, page]() {
                    static_cast<TextButton *>(settingsButtons[page][size])
                        ->setText(texts[i]);
                    for (int j = 0; j < texts.size(); j++) {
                      removeSettingsButton(page, size + 1);
                    }
                    resolutionMenuOpened = !resolutionMenuOpened;
                  },
                  page, size + 1 + i);
            }
          } else {
            for (int i = 0; i < texts.size(); i++) {
              removeSettingsButton(page, size + 1);
            }
          }
        },
        page);
  }

  void updateSettingsButtonsBound() {
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