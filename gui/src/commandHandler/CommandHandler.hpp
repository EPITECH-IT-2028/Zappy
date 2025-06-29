#pragma once

#include <string>
#include "entities/GameState.hpp"

namespace handlecommand {
  /**
   * @brief Handles server commands and updates the game state accordingly.
   *
   * Provides methods to process each type of server command and apply changes to the game state.
   */
  class CommandHandler {
    public:
      /**
       * @brief Constructs a CommandHandler with a reference to the game state.
       * @param gameState Reference to the GameState to be updated.
       */
      CommandHandler(gui::GameState& gameState) : _gameState(gameState) {};

      /**
       * @brief Handles the 'msz' command (map size update).
       * @param command The command string received from the server.
       */
      void handleMsz(const std::string& command);
      /**
       * @brief Handles the 'sgt' command (time unit update).
       * @param command The command string received from the server.
       */
      void handleSgt(const std::string& command);
      /**
       * @brief Handles the 'tna' command (team names update).
       * @param command The command string received from the server.
       */
      void handleTna(const std::string& command);
      /**
       * @brief Handles the 'bct' command (tile resource update).
       * @param command The command string received from the server.
       */
      void handleBct(const std::string& command);
      /**
       * @brief Handles the 'pnw' command (new player).
       * @param command The command string received from the server.
       */
      void handlePnw(const std::string& command);
      /**
       * @brief Handles the 'ppo' command (player position update).
       * @param command The command string received from the server.
       */
      void handlePpo(const std::string& command);
      /**
       * @brief Handles the 'plv' command (player level update).
       * @param command The command string received from the server.
       */
      void handlePlv(const std::string& command);
      /**
       * @brief Handles the 'pin' command (player inventory update).
       * @param command The command string received from the server.
       */
      void handlePin(const std::string& command);
      /**
       * @brief Handles the 'enw' command (egg laid event).
       * @param command The command string received from the server.
       */
      void handleEnw(const std::string& command);
      /**
       * @brief Handles the 'ebo' command (egg hatch event).
       * @param command The command string received from the server.
       */
      void handleEbo(const std::string& command);
      /**
       * @brief Handles the 'edi' command (egg death event).
       * @param command The command string received from the server.
       */
      void handleEdi(const std::string& command);
      /**
       * @brief Handles the 'pdi' command (player death event).
       * @param command The command string received from the server.
       */
      void handlePdi(const std::string& command);
      /**
       * @brief Handles the 'pic' command (incantation start event).
       * @param command The command string received from the server.
       */
      void handlePic(const std::string& command);
      /**
       * @brief Handles the 'pie' command (incantation end event).
       * @param command The command string received from the server.
       */
      void handlePie(const std::string& command);
      /**
       * @brief Handles the 'pfk' command (fork event).
       * @param command The command string received from the server.
       */
      void handlePfk(const std::string& command);
      /**
       * @brief Handles the 'pdr' command (drop resource event).
       * @param command The command string received from the server.
       */
      void handlePdr(const std::string& command);
      /**
       * @brief Handles the 'pgt' command (collect resource event).
       * @param command The command string received from the server.
       */
      void handlePgt(const std::string& command);
      /**
       * @brief Handles the 'pex' command (player expulsion event).
       * @param command The command string received from the server.
       */
      void handlePex(const std::string& command);
      /**
       * @brief Handles the 'pbc' command (broadcast event).
       * @param command The command string received from the server.
       */
      void handlePbc(const std::string& command);
      /**
       * @brief Handles the 'smg' command (server message event).
       * @param command The command string received from the server.
       */
      void handleSmg(const std::string& command);
      /**
       * @brief Handles the 'suc' command (success event).
       * @param command The command string received from the server.
       */
      void handleSuc(const std::string& command);
      /**
       * @brief Handles the 'sbp' command (failure event).
       * @param command The command string received from the server.
       */
      void handleSbp(const std::string& command);
      /**
       * @brief Handles the 'seg' command (game over event).
       * @param command The command string received from the server.
       */
      void handleSeg(const std::string& command);

    private:
      /**
       * @brief Removes an egg from the game state by its ID.
       * @param eggId The ID of the egg to remove.
       */
      void removeEgg(int eggId);
      /**
       * @brief Reference to the game state to be updated.
       */
      gui::GameState& _gameState;
  };
}  // namespace handlecommand
