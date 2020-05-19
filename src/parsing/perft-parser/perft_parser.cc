#include <sstream>
#include "perft_parser.hh"
//Function from class FenRank

std::optional<std::pair<board::PieceType, board::Color>> FenRank::operator[] (board::File file)
{
    int i = 0;
    for (auto current : pieces_)
    {
        if (i == static_cast<int>(file))
        {
            return current;
        }
        i++;
    }
    return std::nullopt;
}

//Functions from class FenObject

std::optional<std::pair<board::PieceType, board::Color>> FenObject::operator[](board::Position position)
{
    int i = 0;
    for (auto rank : rank_)
    {
        if (static_cast<int>(position.rank_get()) == i)
        {
            return rank.pieces_[position.file_get()];
        }
        i++;
    }
    return std::nullopt;
}

board::Color FenObject::side_to_move_get()
{
    return side_to_move_;
}

std::vector<char> FenObject::castling_get()
{
    return castling_;
}

std::optional<board::Position> FenObject::en_passant_target_get()
{
    return en_passant_target_;
}

//Functions from class PerftObject



///////////////////////////

//recupere chacun des rank sous forme de string
std::vector<std::string> parse_ranks(const std::string& input, std::vector<std::string> vect)
{
    std::stringstream ss(input);
    std::string tmp;
    while (getline(ss,tmp,'/'))
    {
        vect.push_back(tmp);
    }
    return vect;
}

//transforme le char en pair<piecetype,color>
std::pair<board::PieceType,board::Color> to_piece(char c)
{
        switch (c)
        {
        case 'P':
            return std::pair(board::PieceType::PAWN,board::Color::WHITE);
        case 'N':
            return std::pair(board::PieceType::KNIGHT,board::Color::WHITE);
        case 'B':
            return std::pair(board::PieceType::BISHOP,board::Color::WHITE);
        case 'R':
            return std::pair(board::PieceType::ROOK,board::Color::WHITE);
        case 'Q':
            return std::pair(board::PieceType::QUEEN,board::Color::WHITE);
        case 'K':
            return std::pair(board::PieceType::KING,board::Color::WHITE);
        case 'p':
            return std::pair(board::PieceType::PAWN,board::Color::BLACK);
        case 'n':
            return std::pair(board::PieceType::KNIGHT,board::Color::BLACK);
        case 'b':
            return std::pair(board::PieceType::BISHOP,board::Color::BLACK);
        case 'r':
            return std::pair(board::PieceType::ROOK,board::Color::BLACK);
        case 'q':
            return std::pair(board::PieceType::QUEEN,board::Color::BLACK);
        case 'k':
            return std::pair(board::PieceType::KING,board::Color::BLACK);
        default:
            throw std::invalid_argument("Unknown piece");
        }
}

//transforme la string en object fenrank
FenRank fromStr_to_FenRank(std::string vect)
{
    size_t i = 0;
    size_t j = 0;
    std::vector<std::optional<std::pair<board::PieceType,board::Color>>> rtn;
    while (i < vect.size())
    {
        char check = vect.at(i);
        if (!isdigit(check))
        {
            rtn.emplace_back(std::nullopt);
        }
        else{
            int k = static_cast<int>(check);
            int l = 0;
            while (l < k)
            {
                rtn.emplace_back(std::nullopt);
                l++;
            }
            l = 0;

        }
        j++;
    }
    j = 0;
    i++;
    return FenRank(rtn);
}

//obtient la position du en_passant_target
board::Position get_position(char file, char rank)
{
    board::File file_;
    board::Rank rank_;
    if (file == 'a')
    {
        file_ = board::File::A;
    }
    else if (file == 'b')
    {
        file_ = board::File::B;
    }
    else if (file == 'c')
    {
        file_ = board::File::C;
    }
    else if (file == 'd')
    {
        file_ = board::File::D;
    }
    else if (file == 'e')
    {
        file_ = board::File::E;
    }
    else if (file == 'f')
    {
        file_ = board::File::F;
    }
    else if (file == 'g')
    {
        file_ = board::File::G;
    }
    else if (file == 'h')
    {
        file_ = board::File::H;
    }

    if (rank == '3')
    {
        rank_ = board::Rank::THREE;
    }
    else if (rank == '6')
    {
        rank_ = board::Rank::SIX;
    }
    return board::Position(file_,rank_);
}


PerftObject parse_perft(std::string input)
{
    FenObject fen;
    int depth;
    std::stringstream ss(input);
    std::vector<std::string> vect;
    vect.reserve(7);
    std::string sousChaine;
    while (getline(ss, sousChaine, ' '))
    {
        vect.push_back(sousChaine);
    }
    int i = 0;
    while (i < 7)
    {
        if (i == 6)
        {
            depth = static_cast<int>(vect.at(i)[0]);
            break;
        }
        i++;
    }
    fen = parse_fen(vect);
    return PerftObject(fen,depth);
}

FenObject parse_fen(std::vector<std::string> splited_input)
{
    board::Color side_to_move;
    int depth;
    std::vector<char> castling;
    std::optional<board::Position> en_passant_target;
    std::vector<std::string> keep;
    std::vector<FenRank> ranks;
    ranks.reserve(7);

    int i = 0;
    while (i < 7) {
        if (i == 0) {
            for (auto rank : parse_ranks(splited_input.at(i), keep)) {
                ranks.push_back(fromStr_to_FenRank(rank));
            }
        }
        if (i == 1) {
            if (splited_input.at(i).compare("w") == 0) {
                side_to_move = board::Color::WHITE;
            } else if (splited_input.at(i).compare("b") == 0) {
                side_to_move = board::Color::BLACK;
            }
        }
        if (i == 2) {
            int size = splited_input.at(i).size();
            int j = 0;
            castling.reserve(size);
            while (j < size) {
                castling.push_back(splited_input.at(i)[j]);
                j++;
            }
        }
        if (i == 3) {
            if (splited_input.at(i).compare("-") != 0) {
                en_passant_target = get_position(splited_input.at(i)[0], splited_input.at(i)[1]);
            }

        }
        if (i == 6) {
            depth = static_cast<int>(splited_input.at(i)[0]);
        }
        i++;
    }
    return FenObject(ranks,side_to_move,castling,en_passant_target);
}
