#pragma once

#include <vector>
#include "token.hpp"

// Breaks up source code into a list of tokens
std::vector<Token> identify_tokens(std::string code);
bool parse_tokens(SourceFile &source, std::vector<Block> &blocks);