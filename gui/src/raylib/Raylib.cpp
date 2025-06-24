#include "Raylib.hpp"

gui::Raylib::Raylib(network::ServerCommunication& serverCommunication)
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(GameScreen::LOGO),
      _serverCommunication(serverCommunication),
      _gameState(0, 0),
      _commandHandler(_gameState) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
}

void gui::Raylib::run() {
  _window.SetTargetFPS(60);

  while (!WindowShouldClose()) {
    processNetworkMessages();
    switch (_currentScreen) {
      case GameScreen::LOGO:
        updateLogoScreen();
        break;
      case GameScreen::TITLE:
        updateTitleScreen();
        break;
      case GameScreen::GAMEPLAY:
        updateGameplayScreen();
        break;
      case GameScreen::ENDING:
        updateEndingScreen();
        break;
      default:
        break;
    }

    _window.BeginDrawing();
    _window.ClearBackground(RAYWHITE);

    switch (_currentScreen) {
      case GameScreen::LOGO:
        renderLogoScreen();
        break;
      case GameScreen::TITLE:
        renderTitleScreen();
        break;
      case GameScreen::GAMEPLAY:
        renderGameplayScreen();
        break;
      case GameScreen::ENDING:
        renderEndingScreen();
        break;
      default:
        break;
    }

    _window.EndDrawing();
  }
}

void gui::Raylib::updateLogoScreen() {
  _framesCounter++;
  if (_framesCounter > LOGO_DURATION_FRAMES) {
    _currentScreen = GameScreen::TITLE;
    _framesCounter = 0;
  }
}

void gui::Raylib::updateTitleScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = GameScreen::GAMEPLAY;
}

void gui::Raylib::updateGameplayScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = GameScreen::ENDING;
}

void gui::Raylib::updateEndingScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = GameScreen::TITLE;
}

void gui::Raylib::renderLogoScreen() {
  DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
  DrawText("Fake loading... Please wait 2 seconds.", 230, 220, 20, GRAY);
}

void gui::Raylib::renderTitleScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
  DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
  DrawText("PRESS ENTER to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

void gui::Raylib::renderGameplayScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PURPLE);
  DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
  DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

void gui::Raylib::renderEndingScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
}

void gui::Raylib::processNetworkMessages() {
  if (!_serverCommunication.isConnected())
    return;

  try {
    while (_serverCommunication.hasIncomingData()) {
      std::string message = _serverCommunication.receiveMessage();
      if (message.empty())
        break;
      if (!message.empty() && message.back() == '\n')
        message.pop_back();

      if (message.substr(0, 3) == "msz") {
        _commandHandler.handleMsz(message);
      } else if (message.substr(0, 3) == "sgt") {
        _commandHandler.handleSgt(message);
      } else if (message.substr(0, 3) == "tna") {
        _commandHandler.handleTna(message);
      } else if (message.substr(0, 3) == "bct") {
        _commandHandler.handleBct(message);
      } else if (message.substr(0, 3) == "pnw") {
        _commandHandler.handlePnw(message);
      } else if (message.substr(0, 3) == "ppo") {
        _commandHandler.handlePpo(message);
      } else if (message.substr(0, 3) == "plv") {
        _commandHandler.handlePlv(message);
      } else if (message.substr(0, 3) == "pin") {
        _commandHandler.handlePin(message);
      } else if (message.substr(0, 3) == "enw") {
        _commandHandler.handleEnw(message);
      } else if (message.substr(0, 3) == "ebo") {
        _commandHandler.handleEbo(message);
      } else if (message.substr(0, 3) == "edi") {
        _commandHandler.handleEdi(message);
      } else if (message.substr(0, 3) == "pdi") {
        _commandHandler.handlePdi(message);
      } else if (message == "WELCOME") {
        std::cout << "Received server welcome message" << std::endl;
      } else {
        std::cout << "Unknown command received: " << message << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error processing network messages: " << e.what() << std::endl;
  }
}
