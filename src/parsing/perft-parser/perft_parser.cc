#include "perft_parser.hh"
//Function from class FenRank

std::optional<std::pair<board::PieceType, board::Color>> FenRank::operator[](board::File file)
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
    for (rank : ranks_)
    {
        if (static_cast<int>(position.rank_get()) == i)
        {
            return pieces_[position.file_get()];
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

board::Position FenObject::en_passant_target_get()
{
    return en_passant_target_;
}

//Functions from class PerftObject

FenObject PerftObject::fen_get()
{
    return fen_;
}

int PerftObject::depth_get()
{
    return depth_;
}

///////////////////////////

//recupere chacun des rank sous forme de string
void parse_ranks(std::string input, std::vector<std::string> vect)
{
    std::stringstream ss(input);
    ss::string tmp;
    while (getline(ss,tmp,'/'))
    {
        vect.push_back(tmp);
    }
}

//transforme le char en pair<piecetype,color>
std::pair<board::PieceType,board::Color> to_piece(char c)
{
        std::pair<board::PieceType,board::Color> rtn;
        switch (c)
        {
        case 'P':
            return rtn(board::PieceType::PAWN,board::Color::WHITE);
        case 'N':
            return rtn(board::PieceType::KNIGHT,board::Color::WHITE);
        case 'B':
            return rtn(board::PieceType::BISHOP,board::Color::WHITE);
        case 'R':
            return rtn(board::PieceType::ROOK,board::Color::WHITE);
        case 'Q':
            return rtn(board::PieceType::QUEEN,board::Color::WHITE);
        case 'K':
            return rtn(board::PieceType::KING,board::Color::WHITE);
        case 'p':
            return rtn(board::PieceType::PAWN,board::Color::BLACK);
        case 'n':
            return rtn(board::PieceType::KNIGHT,board::Color::BLACK);
        case 'b':
            return rtn(board::PieceType::BISHOP,board::Color::BLACK);
        case 'r':
            return rtn(board::PieceType::ROOK,board::Color::BLACK);
        case 'q':
            return rtn(board::PieceType::QUEEN,board::Color::BLACK);
        case 'k':
            return rtn(board::PieceType::KING,board::Color::BLACK);
        default:
            throw std::invalid_argument("Unknown piece");
        }
}

//transforme la string en object fenrank
FenRank fromStr_to_FenRank(std::string vect)
{
    int i = 0;
    FenRank fenrank;
    std::vector<std::optional<std::pair<board::PieceType,board::Color>>> rtn;
    while (i < vect.size())
    {
        while (j < vect.at(i).size())
        {
            char check = vect.at(i)[j];
            if (!isdigit(check))
            {
                rtn.push_back(char_to_piece(check));
            }
            else
            {
                int k = static_cast<int>(check);
                int l = 0;
                while (l < k)
                {
                    rtn.push_back(std::nullopt);
                    l++;
                }
                l = 0;

            }
            j++;
        }
        j = 0;
        i++;
    }
    return fenrank(rtn);
}

//obtient la position du en_passant_target
board::Position get_position(char file, char rank)
{
    board::File file_;
    board::Rank rank_;
    if (rank == 'a')
    {
        rank_ = board::File::A;
    }
    else if (rank == 'b')
    {
        rank_ = board::File::B;
    }
    else if (rank == 'c')
    {
        rank_ = board::File::C;
    }
    else if (rank == 'd')
    {
        rank_ = board::File::D;
    }
    else if (rank == 'e')
    {
        rank_ = board::File::E;
    }
    else if (rank == 'f')
    {
        rank_ = board::File::F;
    }
    else if (rank == 'g')
    {
        rank_ = board::File::G;
    }
    else if (rank == 'h')
    {
        rank_ = board::File::H;
    }

    if (file == '3')
    {
        file_ = board::Rank::THREE;
    }
    else if (file == '6')
    {
        file_ = board::Rank::SIX;
    }
    return board::Position(file_,rank_);
}


PerftObject parse_perft(std::string input)
{
    PerftObject perft;
    stringstream ss(chaine);
    std::vector<std::string> vect;
    vect.reserve(7);
    string sousChaine;
    while (getline(ss, sousChaine, ' '))
    {
        vect.push_back(sousChaine);
    }
    int i = 0;
    while (i < 7)
    {
        if (i == 6)
        {
            perft.depth_ = static_cast<int>(vect.at(i));
            break;
        }
        i++;
    }
    perft.fen_ = parse_fen(vect);
    return perft;
}

FenObject parse_fen(std::vector<std::string> splited_input)
{
    Color side_to_move;
    int depth;
    std::vector<char> castling;
    std::optional<board::Position> en_passant_target;
    std::vector<std::string> keep;
    std::vector<FenRank> ranks;
    ranks.reserve(7);

    int i = 0;
    while (i < 7)
    {
        if (i == 0)
        {
            for (auto rank : parse_ranks(splited_input.at(i), keep))
            {
                ranks.push_back(fromStr_to_FenRank(rank));
            }
        }
        if (i == 1)
        {
            if (splited_input.at(i).compare("w") == 0)
            {
                side_to_move = board::Color::WHITE;
            }
            else if (splited_input.at(i).compare("b") == 0)
            {
                side_to_move = board::Color::BLACK;
            }
        }
        if (i == 2)
        {
            int size = splited_input.at(i).size();
            int j = 0;
            castling.reserve(size);
            while (j < size)
            {
                castling.push_back(to_parse.at(i)[j]);
                j++;
            }
        }
        if (i == 3)
        {
            if (splited_input.at(i).compare("-") != 0)
            {
                en_passant_target = get_position(splited_input.at(i)[0],splited_input.at(i)[1]);
            }
            
        }
        if (i == 6)
        {
            depth = static_cast<int>(splited_input.at(i));
        }
        i++;
    }
    fen_.rank = ranks;
    fen_.side_to_move = side_to_move;
    fen_.castling = castling;
    fen_.en_passant_target = en_passant_target;
    return fen_;
}
