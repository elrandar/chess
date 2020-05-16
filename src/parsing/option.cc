#include <cstring>
#include "option.hh"



Option::Option()
{
    help = false;
    pgn_path = "";
    perft_path = "";
    listeners_vector = std::vector<std::string>();
}

bool Option::parse_options(int argc, char **argv)
{
    int i = 1;
    while (i < argc)
    {
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
