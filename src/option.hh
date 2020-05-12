#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstring>


class Option
{
    public:
        Option()
         :help(false)
         ,pgn_path("")
         ,perft_path("")
         ,listeners_list(std::vector<std::string>())
         {}

        bool parse_options(int argc, char **argv);
        static void show_help();

        bool get_help() const
        {
            return help;
        }

        std::string get_pgn() const
        {
            return pgn_path;
        }
        std::vector<std::string> get_listeners();


    private:
        bool help = false;
        std::string pgn_path;
        std::string perft_path;
        std::vector<std::string> listeners_list;


};