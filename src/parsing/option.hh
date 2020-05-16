#pragma once

#include <string>
#include <vector>

class Option {

private:
    std::string pgn_path;
    std::string perft_path;
    std::vector<std::string> listeners_vector;
    bool help;

public:
    Option();
    bool parse_options(int argc, char **argv);
    void show_help();
    const std::vector<std::string> &getListenersVector() const;
    const std::string &getPgnPath() const;
    const std::string &getPerftPath() const;


};