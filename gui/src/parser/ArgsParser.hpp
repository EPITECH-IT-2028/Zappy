#pragma once

#include <string>

namespace parser {
  class Config {
    public:
      void parse(int argc, char* argv[]);
      void displayHelp() const;

      std::string getOptionP() const;
      std::string getOptionH() const;

    private:
      std::string _optionP;
      std::string _optionH;
  };
}  // namespace parser
