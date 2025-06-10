#include <string>

#ifndef PARSINGARGS_HPP_
  #define PARSINGARGS_HPP_

namespace gui {
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
}  // namespace gui

#endif /* PARSINGARGS_HPP_ */
