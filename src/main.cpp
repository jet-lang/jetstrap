#include <iostream>
#include <string.h>
#include "parser.hpp"

const int MIN_VER = 0;
const int MAX_VER = 1;

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
        source.tokens = identify_tokens(buffer);

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

int
main(int argc, char **argv)
{
    std::vector<std::string> files;

    // Get arguments
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0) {
            std::cout << "jetstrap v" << MAX_VER << "." << MIN_VER << std::endl;
            return 0;
        } else {
            files.push_back(std::string(argv[i]));
        }
    }

    // Lexed source code
    std::vector<SourceFile> sources;

    // Load all files
    if (!load_files(files, sources)) {
        return 1;
    }

    std::cout << "info: found " << sources.size() << " source(s)" << std::endl;

    // Parse tokens
    std::vector<Block> blocks;

    for (SourceFile file : sources) {
        std::vector<Block> b;
        
        if (!parse_tokens(file, b)) {
            return 1;
        }

        for (Block block : b) {
            blocks.push_back(block);
        }
    }

    return 0;
}
