#include <cstring>
#include <iostream>
#include "option.hh"



Option::Option()
{
    help = false;
    pgn_path = "";
    perft_path = "";
    listeners_vector = std::vector<std::string>();
    timeTest = false;
}

void Option::show_help()
{
    std::cout << "chessengine : A chess engine written in C++.\nRun with no arguments to enter IA mode.\n\n";
    std::cout << "Usage:" << std::endl;
    std::cout << "\tchessengine\n";
    std::cout << "\tchessengine --pgn <pgn-file>\n";
    std::cout << "\tchessengine --pgn <pgn-file> -l <listener-path>...\n";
    std::cout << "\tchessengine --perft <perft-file>\n";

    std::cout << "\nOptions:" << '\n';
    std::cout << "\t-h --help\t\tShow this screen." << std::endl;
    std::cout << "\t--pgn arg\t\tPath to a PGN file" << std::endl;
    std::cout << "\t-l [ --listeners ] args\tList of the paths of the listeners plugins" << std::endl;
    std::cout << "\t--perft arg\t\tPath to a perft file" << std::endl;
}

bool Option::parse_options(int argc, char **argv)
{
    int i = 1;
    while (i < argc)
    {
        if (!strcmp(argv[i], "--time-test"))
        {
           timeTest = true;
        }
        if (!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help"))
        {
            help = true;
        }
        else if (!strcmp(argv[i],"--pgn"))
        {
            i++;
            if (i == argc)
            {
                return false;
            }
            pgn_path = std::string(argv[i]);
        }
        else if (!strcmp(argv[i],"-l") || !strcmp(argv[i],"--listeners"))
        {
            i++;
            if (i == argc)
            {
                return false;
            }
            while (i < argc)
            {
                std::string tmp = argv[i];
                listeners_vector.push_back(tmp);
                i++;
            }
        }
        else if (!strcmp(argv[i],"--perft"))
        {
            i++;
            if (i == argc)
            {
                return false;
            }
            perft_path = std::string(argv[i]);
        }
        i++;
    }
    return true;
}

const std::vector<std::string> &Option::getListenersVector() const {
    return listeners_vector;
}

const std::string &Option::getPgnPath() const {
    return pgn_path;
}

const std::string &Option::getPerftPath() const {
    return perft_path;
}
