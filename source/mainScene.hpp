#pragma once
#include "radioButton.hpp"
#include "sliderButton.hpp"
#include "textButton.hpp"
#include <SFML/Audio/Music.hpp>

class MainScene : public sf::Drawable {
private:
  static const int mainButtonWidth = 200;
  static const int settingsButtonWidth = 300;
  static const int buttonHeight = 50;
  static const int buttonIndent = 10;

  std::vector<Button *> mainButtons;

  int settingsPage = 0;
  std::vector<std::vector<Button *>> settingsButtons{4};

  std::vector<TextButton *> textButtons;

  sf::Music *backgroundMusic = Resource::loadBackgroundMusic();

public:
  template <typename Func1, typename Func2, typename Func3>
  MainScene(Func1 exit, Func2 setFullscreen, Func3 setWindowed) {
    setWindowed(0);

    getLocalizationsFunc([]() {})[0](0);

    backgroundMusic->setVolume(10);
    backgroundMusic->setLoop(true);
    backgroundMusic->play();

    addMainButton("new game", [this](int) { setSettingsPage(1); });
    addMainButton("load game", [this](int) { setSettingsPage(2); });
    addMainButton("settings", [this](int) { setSettingsPage(3); });
    addMainButton("exit", exit);
    setMainButtonsBound();

    addSettingsTextButton(1, "start game",
                          [](int) { std::cout << "Start game" << std::endl; });
    addSettingsTextButton(1, "back", [this](int) { setSettingsPage(0); });

    addSettingsTextButton(2, "back", [this](int) { setSettingsPage(0); });

    addSettingsSliderButton(3,
                            [this](int x) { backgroundMusic->setVolume(x); });
    addSettingsRadioButton(3, {"windowed", "fullscreen"},
                           {setWindowed, setFullscreen});
    addSettingsRadioButton(
        3, getLocalizationsNames(),
        getLocalizationsFunc([this]() { updateLocalization(); }),
        ADD_BUTTON_BY_TEXT);
    addSettingsTextButton(3, "back", [this](int) { setSettingsPage(0); });
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

  void addMainButton(const std::string &text, std::function<void(int)> func,
                     int type = ADD_BUTTON_BY_KEY) {
    TextButton *textButton = new TextButton(text, func, type);
    mainButtons.emplace_back(textButton);

    if (type == ADD_BUTTON_BY_KEY) {
      textButtons.emplace_back(textButton);
    }
  }

  void setMainButtonsBound() {
    for (int i = 0; i < mainButtons.size(); i++) {
      mainButtons[i]->setBound(buttonIndent,
                               buttonIndent + i * (buttonHeight + buttonIndent),
                               mainButtonWidth, buttonHeight, buttonIndent);
    }
  }

  void addSettingsTextButton(int page, const std::string &localizationKey,
                             std::function<void(int)> func,
                             int type = ADD_BUTTON_BY_KEY) {
    TextButton *textButton = new TextButton(localizationKey, func, type);
    settingsButtons[page].emplace_back(textButton);

    if (type == ADD_BUTTON_BY_KEY) {
      textButtons.emplace_back(textButton);
    }
  }

  void addSettingsRadioButton(int page,
                              const std::vector<std::string> localizationKeys,
                              std::vector<std::function<void(int)>> funcs,
                              int type = ADD_BUTTON_BY_KEY) {
    std::vector<Button *> subButtons;
    for (int i = 0; i < localizationKeys.size(); i++) {
      TextButton *textButton =
          new TextButton(localizationKeys[i], funcs[i], type);
      subButtons.emplace_back(textButton);

      if (type == ADD_BUTTON_BY_KEY) {
        textButtons.emplace_back(textButton);
      }
    }

    settingsButtons[page].emplace_back(new RadioButton(subButtons));
  }

  void addSettingsSliderButton(int page, std::function<void(int)> func,
                               int defaultValue = 10) {
    settingsButtons[page].emplace_back(new SliderButton(func, defaultValue));
  }

  void updateSettingsButtonsBound() {
    for (int i = 0; i < settingsButtons.size(); i++) {
      int deltaY = 0;
      for (int j = 0; j < settingsButtons[i].size(); j++) {
        settingsButtons[i][j]->setBound(
            buttonIndent + mainButtonWidth + buttonIndent,
            buttonIndent + deltaY, settingsButtonWidth, buttonHeight,
            buttonIndent);
        deltaY += settingsButtons[i][j]->getBound().getSize().y + buttonIndent;
      }
    }
  }

  template <typename Func>
  std::vector<std::function<void(int)>>
  getLocalizationsFunc(Func updateLocalization) {
    std::vector<std::function<void(int)>> loadLanguageFuncs;
    for (auto &langugage : Resource::listLocalizations()) {
      loadLanguageFuncs.emplace_back([langugage, updateLocalization](int) {
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
    for (auto &button : textButtons) {
      button->resetString();
    }
  }
};