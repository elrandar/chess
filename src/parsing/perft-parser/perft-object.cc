
#include "perft-object.hh"

#include <utility>

namespace perft_parser
{
    PerftObject::PerftObject(FenObject fenObject, int depth)
        : fen_(std::move(fenObject))
        , depth_(depth)
    {}

    FenObject PerftObject::fen_get()
    {
        return FenObject(fen_);
    }

    int PerftObject::depth_get() const
    {
        return depth_;
    }

    PerftObject parse_perft(const std::string& input)
    {
        auto lastSpaceIndex = input.find_last_of(' ');

        auto depth = input.substr(lastSpaceIndex + 1);
        int depthInteger = std::stoi(depth);

        std::string fenString = input.substr(0, lastSpaceIndex);

        return PerftObject(parse_fen(fenString), depthInteger);
    }

    FenObject parse_fen(std::string fenString)
    {
        return FenObject(std::move(fenString));
    }
} // namespace perft_parser