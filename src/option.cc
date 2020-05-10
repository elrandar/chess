#include "option.hh"

using po = boost::program_options;

Option::Option()
{
    help = false;
    pgn_path = "";
    perft_path = "";
    listeners_list = std::vector<std::string>();
}

bool Option::parse_options(int argc, char **argv)
{
    int i = 1;

    while (i < argc)
    {
        if (!strcmp(argv[i]),"-h" || !strcmp(argv[i]),"--help")
        {
            help = true;
        }
        else if (!strcmp(argv[i]),"--pgn")
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
                listeners_list.push_back(tmp);
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