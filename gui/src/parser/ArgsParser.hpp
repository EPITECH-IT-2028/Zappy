#pragma once

#include <string>

namespace parser {
  /**
   * @brief Configuration class for parsing and storing command-line arguments for the Zappy GUI.
   */
  class Config {
    public:
      /**
       * @brief Parses command-line arguments and stores the port and host options.
       * @param argc Number of command-line arguments.
       * @param argv Array of command-line argument strings.
       * @throws std::invalid_argument or std::out_of_range on invalid input.
       */
      void parse(int argc, char* argv[]);
      /**
       * @brief Displays help information for the Zappy GUI command-line arguments.
       */
      void displayHelp() const;
      /**
       * @brief Gets the port option value as a string.
       * @return The port value provided by the user.
       */
      std::string getOptionP() const;
      /**
       * @brief Gets the host option value as a string.
       * @return The host value provided by the user.
       */
      std::string getOptionH() const;

    private:
      std::string _optionP; ///< Port value provided by the user.
      std::string _optionH; ///< Host value provided by the user.
  };
}  // namespace parser
