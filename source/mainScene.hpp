#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "activatable/text.hpp"
#include "button1.hpp"
#include "radioButton.hpp"
#include "sliderButton.hpp"
#include "textButton.hpp"
#include <SFML/Audio/Music.hpp>
#include <cmath>

class MainScene : public sf::Drawable {
private:
  const float menuButtonRatio = 1.f / 4.f;
  const float settingsButtonRatio = 1.f / 3.f;

  int menuButtonWidth;
  int settingsButtonWidth;
  int buttonHeight = 50;
  int buttonIndent = 10;

  std::vector<Button *> menuButtons;

  int settingsPage = 0;
  std::vector<std::vector<Button *>> settingsButtons{4};

  std::vector<TextButton *> textByKeyButtons;
  std::vector<SliderButton *> sliderButtons;

  sf::Music *backgroundMusic = Resource::loadBackgroundMusic();
  sf::RectangleShape background;

  Button1 b1, b2;

public:
  template <typename Exit, typename SetFullscreen, typename SetWindowed>
  MainScene(Exit exit, SetFullscreen setFullscreen, SetWindowed setWindowed,
            sf::Vector2u windowSize)
      : b1([]() { std::cout << "button \'b1\' has pressed\n"; },
           {new AText(L"text")}),
        b2([]() { std::cout << "button \'b2\' has pressed\n"; },
           {new AText(L"aboba abob abo ab o")}),
        menuButtonWidth(windowSize.x * menuButtonRatio),
        settingsButtonWidth(windowSize.x * settingsButtonRatio) {
    b1.setBound(300, 300, 50, 50, 10);
    b2.setBound(350, 300, 50, 50, 10);
    setWindowed();

    getLocalizationsFunc([]() {})[0]();

    backgroundMusic->setVolume(10);
    backgroundMusic->setLoop(true);
    backgroundMusic->play();

    background.setTexture(&Resource::background);
    background.setSize(
        {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

    addMainButton("new game", [this]() { setSettingsPage(1); });
    addMainButton("load game", [this]() { setSettingsPage(2); });
    addMainButton("settings", [this]() { setSettingsPage(3); });
    addMainButton("exit", exit);
    updateMainButtonBounds();

    addSettingsTextButton(1, "start game",
                          []() { std::cout << "Start game" << std::endl; });
    addSettingsTextButton(1, "back", [this]() { setSettingsPage(0); });

    addSettingsTextButton(2, "back", [this]() { setSettingsPage(0); });

    addSettingsSliderButton(3, 10);
    addSettingsRadioButton(3, {"windowed", "fullscreen"},
                           {setWindowed, setFullscreen});
    addSettingsRadioButton(
        3, getLocalizationsNames(),
        getLocalizationsFunc([this]() { updateLocalization(); }),
        ADD_BUTTON_BY_TEXT);
    addSettingsTextButton(3, "back", [this]() { setSettingsPage(0); });
  }

  ~MainScene() {
    while (menuButtons.size()) {
      delete *(menuButtons.rbegin());
      menuButtons.pop_back();
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
    b1.eventProcessing(event);
    b2.eventProcessing(event);

    for (auto &button : menuButtons) {
      button->eventProcessing(event);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->eventProcessing(event);
    }

    if (event.type == sf::Event::Resized) {
      background.setSize({static_cast<float>(event.size.width),
                          static_cast<float>(event.size.height)});
      menuButtonWidth = event.size.width / 4;
      settingsButtonWidth = event.size.width / 2;
    }
  }

  void update() {
    b1.update();
    b2.update();

    for (auto &button : menuButtons) {
      button->update();
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->update();
    }

    updateMainButtonBounds();
    updateSettingsButtonsBound();
    backgroundMusic->setVolume(sliderButtons[0]->getValue());
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(background, states);

    target.draw(b1, states);
    target.draw(b2, states);

    for (auto &button : menuButtons) {
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

  void addMainButton(const std::string &text, std::function<void()> func,
                     int type = ADD_BUTTON_BY_KEY) {
    TextButton *textButton = new TextButton(text, func, type);
    menuButtons.emplace_back(textButton);

    if (type == ADD_BUTTON_BY_KEY) {
      textByKeyButtons.emplace_back(textButton);
    }
  }

  void updateMainButtonBounds() {
    for (int i = 0; i < menuButtons.size(); i++) {
      menuButtons[i]->setBound(buttonIndent,
                               buttonIndent + i * (buttonHeight + buttonIndent),
                               menuButtonWidth, buttonHeight, buttonIndent);
    }
  }

  void addSettingsTextButton(int page, const std::string &localizationKey,
                             std::function<void()> func,
                             int type = ADD_BUTTON_BY_KEY) {
    TextButton *textButton = new TextButton(localizationKey, func, type);
    settingsButtons[page].emplace_back(textButton);

    if (type == ADD_BUTTON_BY_KEY) {
      textByKeyButtons.emplace_back(textButton);
    }
  }

  void addSettingsRadioButton(int page,
                              const std::vector<std::string> localizationKeys,
                              std::vector<std::function<void()>> funcs,
                              int type = ADD_BUTTON_BY_KEY) {
    std::vector<Button *> subButtons;
    for (int i = 0; i < localizationKeys.size(); i++) {
      TextButton *textButton =
          new TextButton(localizationKeys[i], funcs[i], type);
      subButtons.emplace_back(textButton);

      if (type == ADD_BUTTON_BY_KEY) {
        textByKeyButtons.emplace_back(textButton);
      }
    }

    settingsButtons[page].emplace_back(new RadioButton(subButtons));
  }

  void addSettingsSliderButton(int page, int defaultValue = 10) {
    SliderButton *sliderButton = new SliderButton(defaultValue);
    sliderButtons.emplace_back(sliderButton);
    settingsButtons[page].emplace_back(sliderButton);
  }

  void updateSettingsButtonsBound() {
    for (int i = 0; i < settingsButtons.size(); i++) {
      int deltaY = 0;
      for (int j = 0; j < settingsButtons[i].size(); j++) {
        settingsButtons[i][j]->setBound(
            buttonIndent + menuButtonWidth + buttonIndent,
            buttonIndent + deltaY, settingsButtonWidth, buttonHeight,
            buttonIndent);
        deltaY += settingsButtons[i][j]->getBound().getSize().y + buttonIndent;
      }
    }
  }

  template <typename Func>
  std::vector<std::function<void()>>
  getLocalizationsFunc(Func updateLocalization) {
    std::vector<std::function<void()>> loadLanguageFuncs;
    for (auto &langugage : Resource::listLocalizations()) {
      loadLanguageFuncs.emplace_back([langugage, updateLocalization]() {
        std::ifstream file(langugage);
        Resource::localization = nlohmann::json::parse(file);
        updateLocalization();
      });
    }

    return loadLanguageFuncs;
  }

  std::vector<std::string> getLocalizationsNames() {
    std::vector<std::string> localizationsNames;
    std::vector<std::string> listLocalizations = Resource::listLocalizations();

    for (auto &langugage : listLocalizations) {
      nlohmann::json localization =
          nlohmann::json::parse(std::ifstream(langugage));
      localizationsNames.emplace_back(localization["language"]);
    }

    return localizationsNames;
  }

  void updateLocalization() {
    for (auto &button : textByKeyButtons) {
      button->resetString();
    }

    updateMainButtonBounds();
    updateSettingsButtonsBound();
  }
};