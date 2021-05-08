#include <iostream>
#include "parser.hpp"

std::vector<Token>
identify_tokens(std::string code)
{
    std::vector<Token> tokens;
    std::string storage("");

    int i;
    for (i = 0; i < code.length(); i++) {
        char c = code[i];

        switch (c) {
            case '`':
            case '~':
            case '!':
            case '@':
            case '#':
            case '$':
            case '%':
            case '^':
            case '&':
            case '*':
            case '(':
            case ')':
            case '-':
            case '=':
            case '+':
            case '[':
            case '{':
            case ']':
            case '}':
            case '\\':
            case '|':
            case ';':
            case ':':
            case '\'':
            case '\"':
            case ',':
            case '<':
            case '.':
            case '>':
            case '/':
            case '?':
            case ' ':
            case '\t':
            case '\n':
                if (storage.length() > 0) {
                    Token token;

                    token.label = storage;
                    token.type = TOKEN_IDENTIFIER;

                    tokens.push_back(token);

                    storage = std::string("");
                }

                if (c != ' ' && c != '\n') {
                    Token token;

                    token.label = std::string(1, c);
                    token.type = TOKEN_SYMBOL;

                    tokens.push_back(token);
                }

                break;
            default:
                storage.push_back(c);
                break;
        }
    }

    return tokens;
}

bool
parse_tokens(SourceFile &source, std::vector<Block> &blocks)
{
    Block block;
    block.label = "";
    
    std::vector<Block*> current;
    
    current.push_back(&block);

    int i;
    for (i = 0; i < source.tokens.size(); i++) {
        Token token = source.tokens[i];
    }

    return true;
}