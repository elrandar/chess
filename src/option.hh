#pragma once

#include <string>
#include <vector>

class Option{
    public:
        bool parse_options(int argc, char **argv);
        void show_help();
        std::string get_pgn();


    private:
        std::string pgn_path;
        std::stirng pertf_path;
        std::vector<std::string> listeners_vector;
        bool help;


}