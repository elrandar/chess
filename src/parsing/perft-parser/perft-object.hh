#pragma once

#include <string>
#include "fen-object.hh"

namespace perft_parser
{
    class PerftObject
    {
    private:
        FenObject fen_;
        int depth_;
    public:
        PerftObject(FenObject fenObject, int depth);
        FenObject fen_get();
        int depth_get() const;
    };

    PerftObject parse_perft(const std::string& input);
    FenObject parse_fen(std::string fenString);
}


