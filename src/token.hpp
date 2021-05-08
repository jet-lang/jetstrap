#pragma once

#include <string>
#include <vector>

// Tokens
enum TokenType {
    TOKEN_SYMBOL,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER
};

struct Token {
    TokenType type;
    std::string label;
};

// Source file
struct SourceFile {
    std::string path;
    std::vector<Token> tokens;
};

// AST classes
enum ValueType {
    PRIMITIVE,
    STRUCT,
    NONE
};

enum BlockType {
    STATEMENT,
    FUNCTION,
    VARIABLE,
    STRUCT
};

class Block {
public:
    BlockType block_type;

    std::string label;
    std::vector<Block> children;
};

class Variable : public Block {
public:
    ValueType type;
    std::string value;
};

class Function : public Block {
public:
    ValueType return_type;
    std::vector<Variable> parameters;
};