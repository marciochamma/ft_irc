#include "ColorUtils.hpp"

std::string RED(bool isIRC)    { return isIRC ? "\x03""04" : "\e[1;31m"; }
std::string YEL(bool isIRC)    { return isIRC ? "\x03""08" : "\e[1;33m"; }
std::string GRE(bool isIRC)    { return isIRC ? "\x03""03" : "\e[1;32m"; }
std::string BLU(bool isIRC)    { return isIRC ? "\x03""12" : "\e[1;34m"; }
std::string WHI(bool isIRC)    { return isIRC ? "\x03""00" : "\e[0;37m"; }
std::string RESET(bool isIRC)  { return isIRC ? "\x03"     : "\e[0m"; }
