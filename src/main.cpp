#include <string>
#include <vector>
#include <iostream>
#include <string.h>

enum TokenType {
    TOKEN_SYMBOL,
    TOKEN_IDENTIFIER
};

struct Token {
    std::string label;
    TokenType type;
    int line;
};

bool
tokenize(std::string code, std::vector<Token> &tokens)
{
    std::string storage;
    
    int i, line = 1;
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
                    token.line = line;

                    tokens.push_back(token);

                    storage = std::string("");
                }

                if (c != ' ' && c != '\n') {
                    Token token;

                    token.label = std::string(1, c);
                    token.type = TOKEN_SYMBOL;
                    token.line = line;

                    tokens.push_back(token);
                }

                if (c == '\n') {
                    line++;
                }

                break;
            default:
                storage.push_back(c);
                break;
        }
    }
    
    return true;
}

struct SourceFile {
    std::string path;
    std::vector<Token> tokens;
};

bool
load_files(std::vector<std::string> &files, std::vector<SourceFile> &sources)
{
    std::vector<std::string> imports;

    for (std::string path : files) {
        FILE* f = fopen(path.c_str(), "r");

        if (!f) {
            std::cout << "error: file not found '" << path << "'" << std::endl;
            return false;
        }

        // Read file
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);

        char* buffer = new char[size];

        rewind(f);
        fread(buffer, sizeof(char), size, f);

        fclose(f);

        // Push source
        SourceFile source;

        source.path = path;
        tokenize(buffer, source.tokens);

        // Handle imports
        int i;
        for (i = 0; i < source.tokens.size(); i++) {
            Token token = source.tokens[i];

            if (token.label.compare("import") == 0) {
                Token t = source.tokens[i + 1];

                if (t.type == TOKEN_IDENTIFIER) {
                    imports.push_back(t.label + ".jet");
                    i++;
                } else {
                    std::cout << "error: invalid file name '" << t.label << "'" << std::endl;
                    return false;
                }
            }
        }

        sources.push_back(source);

        // Delete file buffer
        delete[] buffer;
    }

    if (imports.size() > 0) {
        return load_files(imports, sources);
    }

    return true;
}

enum DataType {
    TYPE_NONE,
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_FLOAT32,
    TYPE_FLOAT64,
    TYPE_STRING
};

DataType
parse_data_type(std::string type)
{
    if (type.compare("int8") == 0) {
        return TYPE_INT8;
    } else if (type.compare("int16") == 0) {
        return TYPE_INT16;
    } else if (type.compare("int32") == 0) {
        return TYPE_INT32;
    } else if (type.compare("int64") == 0) {
        return TYPE_INT64;
    } else if (type.compare("uint8") == 0) {
        return TYPE_UINT8;
    } else if (type.compare("uint16") == 0) {
        return TYPE_UINT16;
    } else if (type.compare("uint32") == 0) {
        return TYPE_UINT32;
    } else if (type.compare("uint64") == 0) {
        return TYPE_UINT64;
    } else if (type.compare("float32") == 0) {
        return TYPE_FLOAT32;
    } else if (type.compare("float64") == 0) {
        return TYPE_FLOAT64;
    } else if (type.compare("string") == 0) {
        return TYPE_STRING;
    }
    // Simple types
    else if (type.compare("int") == 0) {
        return TYPE_INT32;
    } else if (type.compare("uint") == 0) {
        return TYPE_UINT32;
    } else if (type.compare("float") == 0) {
        return TYPE_FLOAT32;
    } else if (type.compare("byte") == 0) {
        return TYPE_UINT8;
    } else if (type.compare("char") == 0) {
        return TYPE_UINT16;
    } else {
        return TYPE_NONE;
    }
}

struct Value {
    DataType type;
    
    char int8_value;
    short int16_value;
    int int32_value;
    long int64_value;
    unsigned char uint8_value;
    unsigned short uint16_value;
    unsigned int uint32_value;
    unsigned long uint64_value;
    float float32_value;
    double float64_value;
    std::string string_value;
};

enum BlockType {
    BLOCK_NONE,
    BLOCK_MODULE,
    BLOCK_VARIABLE,
    BLOCK_STATEMENT,
    BLOCK_FUNCTION,
    BLOCK_STRUCTURE,
    BLOCK_ENUM,
    BLOCK_FOR,
    BLOCK_WHILE
};

class Block {
public:
    BlockType type;
    std::vector<Block*> children;
    
    void free() {
        for (Block* block : children) {
            block->free();
            delete block;
        }
    }
};

class Module : public Block {
public:
    std::string label;
};

class Variable : public Block {
public:
    std::string label;
    Value value;
};

class Statement : public Block {
public:
    
};

class Function : public Block {
public:
    std::string label;
    DataType return_type;
    std::vector<Variable> parameters;
};

class Structure : public Block {
public:
    std::string label;
};

class Enum : public Block {
public:
    std::string label;
    std::vector<Variable> values;
};

void
syntax_error(SourceFile &source, int line, std::string msg)
{
    std::cout << source.path << ":" << line << ": error: " << msg << std::endl;
}

bool
parse(SourceFile &source, std::vector<Block*> &ast)
{
    std::vector<Block*> current;
    
    Block block;
    block.type = BLOCK_NONE;
    
    current.push_back(&block);
    
    int i;
    for (i = 0; i < source.tokens.size(); i++) {
        Token token = source.tokens[i];
        
        if (token.label.compare("func") == 0) { // Parse function
            Function* function = new Function();
            i++;
            
            function->type = BLOCK_FUNCTION;
            
            if (source.tokens[i].type != TOKEN_IDENTIFIER) {
                syntax_error(source, source.tokens[i].line, "expected function name, got '" + source.tokens[i].label + "'");
                return false;
            }
            
            function->label = source.tokens[i].label;
            i++;
            
            if (source.tokens[i].label.compare("(") != 0) {
                syntax_error(source, source.tokens[i].line, "expected '(', got '" + source.tokens[i].label + "'");
                return false;
            }
            
            i++;
            
            while (source.tokens[i].label.compare(")") != 0) {
                if (source.tokens[i].label.compare(",") == 0) {
                    i++;
                    continue;
                }
                
                if (source.tokens[i].type != TOKEN_IDENTIFIER) {
                    syntax_error(source, source.tokens[i].line, "expected identifier, got '" + source.tokens[i].label + "'");
                    return false;
                }
                
                for (Variable v : function->parameters) {
                    if (v.label.compare(source.tokens[i].label) == 0) {
                        syntax_error(source, source.tokens[i].line, "redefinition of parameter '" + source.tokens[i].label + "'");
                        return false;
                    }
                }
                
                Variable variable;
                
                variable.label = source.tokens[i].label;
                i++;
                
                if (source.tokens[i].label.compare(":") != 0) {
                    syntax_error(source, source.tokens[i].line, "expected ':', got '" + source.tokens[i].label + "'");
                    return false;
                }
                
                i++;
                
                DataType type = parse_data_type(source.tokens[i].label);
                
                if (type == TYPE_NONE) {
                    syntax_error(source, source.tokens[i].line, "expected type, got '" + source.tokens[i].label + "'");
                    return false;
                }
                
                variable.value.type = type;
                
                function->parameters.push_back(variable);
                
                i++;
                
                if (source.tokens[i].label.compare(",") != 0 && source.tokens[i].label.compare(")") != 0) {
                    syntax_error(source, source.tokens[i].line, "expected ',', got '" + source.tokens[i].label + "'");
                    return false;
                }
            }
            
            i++;
            
            if (source.tokens[i].label.compare("-") == 0 && source.tokens[i + 1].label.compare(">") == 0) {
                i += 2;
                
                DataType type = parse_data_type(source.tokens[i].label);
                
                if (type == TYPE_NONE) {
                    syntax_error(source, source.tokens[i].line, "expected type, got '" + source.tokens[i].label + "'");
                    return false;
                }
                
                function->return_type = type;
                
                i++;
                
                if (source.tokens[i].label.compare("{") != 0) {
                    syntax_error(source, source.tokens[i].line, "expected '{', got '" + source.tokens[i].label + "'");
                    return false;
                }
            } else {
                if (source.tokens[i].label.compare("{") != 0) {
                    syntax_error(source, source.tokens[i].line, "expected '{', got '" + source.tokens[i].label + "'");
                    return false;
                }
            }

            current[current.size() - 1]->children.push_back(function);
            current.push_back(current[current.size() - 1]->children[current[current.size() - 1]->children.size() - 1]);
        } else if (token.label.compare("var") == 0) {
            // TODO
        } else if (token.label.compare("}") == 0) {
            current.pop_back();
        }
    }
    
    for (i = 0; i < block.children.size(); i++) {
        ast.push_back(block.children[i]);
    }
    
    return true;
}

int
main(int argc, char **argv)
{
    std::vector<std::string> files;
    
    int i;
    for (i = 1; i < argc; i++) {
        files.push_back(std::string(argv[i]));
    }
    
    // Lexed source code
    std::vector<SourceFile> sources;

    // Load all files
    if (!load_files(files, sources)) {
        return 1;
    }

    std::cout << "info: found " << sources.size() << " source(s)" << std::endl;

    // Parse tokens
    std::vector<Block*> blocks;

    for (SourceFile file : sources) {
        std::vector<Block*> b;
        
        if (!parse(file, b)) {
            return 1;
        }

        for (Block* block : b) {
            blocks.push_back(block);
        }
    }
    
    for (Block* block : blocks) {
        if (block->type == BLOCK_FUNCTION) {
            Function* function = (Function*)block;
            
            std::cout << function->label << " (";
            
            for (Variable v : function->parameters) {
                std::cout << v.label << ": " << v.value.type << ", ";
            }
            
            std::cout << ") -> " << function->return_type << std::endl;
        }
    }
    
    for (Block* block : blocks) {
        block->free();
        delete block;
    }
    
    return 0;
}
