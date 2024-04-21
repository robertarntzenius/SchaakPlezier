#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "board.h"
#include "game.h"


namespace py = pybind11;

void bindEnums(py::module& m) {
    // Bind Color enum
    py::enum_<Color>(m, "Color")
        .value("White", Color::White)
        .value("Black", Color::Black)
        .value("NrColors", Color::NrColors);

    // Bind Square enum
    py::enum_<Square>(m, "Square")
        .value("a1", Square::a1)
        .value("b1", Square::b1)
        .value("c1", Square::c1)
        .value("d1", Square::d1)
        .value("e1", Square::e1)
        .value("f1", Square::f1)
        .value("g1", Square::g1)
        .value("h1", Square::h1)

        .value("a2", Square::a2)
        .value("b2", Square::b2)
        .value("c2", Square::c2)
        .value("d2", Square::d2)
        .value("e2", Square::e2)
        .value("f2", Square::f2)
        .value("g2", Square::g2)
        .value("h2", Square::h2)

        .value("a3", Square::a3)
        .value("b3", Square::b3)
        .value("c3", Square::c3)
        .value("d3", Square::d3)
        .value("e3", Square::e3)
        .value("f3", Square::f3)
        .value("g3", Square::g3)
        .value("h3", Square::h3)

        .value("a4", Square::a4)
        .value("b4", Square::b4)
        .value("c4", Square::c4)
        .value("d4", Square::d4)
        .value("e4", Square::e4)
        .value("f4", Square::f4)
        .value("g4", Square::g4)
        .value("h4", Square::h4)

        .value("a5", Square::a5)
        .value("b5", Square::b5)
        .value("c5", Square::c5)
        .value("d5", Square::d5)
        .value("e5", Square::e5)
        .value("f5", Square::f5)
        .value("g5", Square::g5)
        .value("h5", Square::h5)

        .value("a6", Square::a6)
        .value("b6", Square::b6)
        .value("c6", Square::c6)
        .value("d6", Square::d6)
        .value("e6", Square::e6)
        .value("f6", Square::f6)
        .value("g6", Square::g6)
        .value("h6", Square::h6)

        .value("a7", Square::a7)
        .value("b7", Square::b7)
        .value("c7", Square::c7)
        .value("d7", Square::d7)
        .value("e7", Square::e7)
        .value("f7", Square::f7)
        .value("g7", Square::g7)
        .value("h7", Square::h7)

        .value("a8", Square::a8)
        .value("b8", Square::b8)
        .value("c8", Square::c8)
        .value("d8", Square::d8)
        .value("e8", Square::e8)
        .value("f8", Square::f8)
        .value("g8", Square::g8)
        .value("h8", Square::h8)

        .value("NrSquares", Square::NrSquares)
        .value("NoSquare", Square::NoSquare);

    // Bind Piecetype enum
    py::enum_<Piecetype>(m, "Piecetype")
        .value("Pawn", Piecetype::Pawn)
        .value("Knight", Piecetype::Knight)
        .value("Bishop", Piecetype::Bishop)
        .value("Rook", Piecetype::Rook)
        .value("Queen", Piecetype::Queen)
        .value("King", Piecetype::King)
        .value("NrPiecetypes", Piecetype::NrPiecetypes)
        .value("NrPromotiontypes", Piecetype::NrPromotiontypes)
        .value("NoType", Piecetype::NoType);
    


    // Bind File enum
    py::enum_<File>(m, "File")
        .value("FileA", File::FileA)
        .value("FileB", File::FileB)
        .value("FileC", File::FileC)
        .value("FileD", File::FileD)
        .value("FileE", File::FileE)
        .value("FileF", File::FileF)
        .value("FileG", File::FileG)
        .value("FileH", File::FileH)
        .value("NrFiles", File::NrFiles);

    // Bind Rank enum
    py::enum_<Rank>(m, "Rank")
        .value("Rank8", Rank::Rank8)
        .value("Rank7", Rank::Rank7)
        .value("Rank6", Rank::Rank6)
        .value("Rank5", Rank::Rank5)
        .value("Rank4", Rank::Rank4)
        .value("Rank3", Rank::Rank3)
        .value("Rank2", Rank::Rank2)
        .value("Rank1", Rank::Rank1)
        .value("NrRanks", Rank::NrRanks);

    // Bind Direction enum
    py::enum_<Direction>(m, "Direction")
        .value("North", Direction::North)
        .value("South", Direction::South)
        .value("West", Direction::West)
        .value("East", Direction::East)
        .value("NorthEast", Direction::NorthEast)
        .value("NorthWest", Direction::NorthWest)
        .value("SouthEast", Direction::SouthEast)
        .value("SouthWest", Direction::SouthWest)
        .value("FirstOrthogonal", Direction::FirstOrthogonal)
        .value("LastOrthogonal", Direction::LastOrthogonal)
        .value("FirstDiagonal", Direction::FirstDiagonal)
        .value("LastDiagonal", Direction::LastDiagonal)
        .value("NrDirections", Direction::NrDirections);

    // Bind GameResult enum
    py::enum_<GameResult>(m, "GameResult")
        .value("NOT_OVER", NOT_OVER)
        .value("WHITE_WIN_BY_CHECKMATE", WHITE_WIN_BY_CHECKMATE)
        .value("BLACK_WIN_BY_CHECKMATE", BLACK_WIN_BY_CHECKMATE)
        .value("WHITE_WIN_BY_TIME_OUT", WHITE_WIN_BY_TIME_OUT)
        .value("BLACK_WIN_BY_TIME_OUT", BLACK_WIN_BY_TIME_OUT)
        .value("WHITE_WIN_BY_FORFEIT", WHITE_WIN_BY_FORFEIT)
        .value("BLACK_WIN_BY_FORFEIT", BLACK_WIN_BY_FORFEIT)
        .value("DRAW_BY_STALEMATE", DRAW_BY_STALEMATE)
        .value("DRAW_BY_INSUFFICIENT_MATERIAL", DRAW_BY_INSUFFICIENT_MATERIAL)
        .value("DRAW_BY_REPETITION", DRAW_BY_REPETITION)
        .value("DRAW_BY_50_MOVES", DRAW_BY_50_MOVES);
}

void bindStructs(py::module& m) {
    // Binding for Move struct
    py::class_<Move>(m, "Move")
        .def_readwrite("playerPiece", &Move::playerPiece)
        .def_readwrite("fromSquare", &Move::fromSquare)
        .def_readwrite("targetSquare", &Move::targetSquare)
        .def_readwrite("isCastling", &Move::isCastling)
        .def_readwrite("isCapture", &Move::isCapture)
        .def_readwrite("isPromotion", &Move::isPromotion)
        .def_readwrite("promotionPiece", &Move::promotionPiece)
        .def_readwrite("capturePiece", &Move::capturePiece)
        .def_readwrite("captureSquare", &Move::captureSquare)
        .def_readwrite("newEnPassant", &Move::newEnPassant)
        .def(py::pickle(
            [](const Move &m) {  // __reduce__
                return py::make_tuple(
                    m.playerPiece, m.fromSquare, m.targetSquare,
                    m.isCastling, m.isCapture, m.isPromotion,
                    m.promotionPiece, m.capturePiece, m.captureSquare,
                    m.newEnPassant
                );
            },
            [](py::tuple t) {  // __setstate__
                Move m;
                m.playerPiece = t[0].cast<Piecetype>();
                m.fromSquare = t[1].cast<Square>();
                m.targetSquare = t[2].cast<Square>();
                m.isCastling = t[3].cast<bool>();
                m.isCapture = t[4].cast<bool>();
                m.isPromotion = t[5].cast<bool>();
                m.promotionPiece = t[6].cast<Piecetype>();
                m.capturePiece = t[7].cast<Piecetype>();
                m.captureSquare = t[8].cast<Square>();
                m.newEnPassant = t[9].cast<Square>();
                return m;
            }
        ));

    // Binding for BoardState struct
    py::class_<BoardState>(m, "BoardState")
        .def_readwrite("activePlayer", &BoardState::activePlayer)
        .def_readwrite("wKC", &BoardState::wKC)
        .def_readwrite("wQC", &BoardState::wQC)
        .def_readwrite("bKC", &BoardState::bKC)
        .def_readwrite("bQC", &BoardState::bQC)
        .def_readwrite("enPassantSquare", &BoardState::enPassantSquare)
        .def_readwrite("fullMoveNumber", &BoardState::fullMoveNumber)
        .def_readwrite("halfMoveClock", &BoardState::halfMoveClock);
}

void bindMaps(py::module& m) {
    m.def("string_piecetype_map", []() {
        py::dict map;
        for (const auto& pair : stringPiecetypeMap) {
            map[py::str(pair.first)] = pair.second;
        }
        return map;
    });
}

void bindBoardGetters(py::class_<Board>& boardClass) {
    boardClass.def("getPieceMaps", [](Board& board) {
        auto whitePieceMap = board.getPieceMap(Color::White);
        auto blackPieceMap = board.getPieceMap(Color::Black);

        // Convert C++ unordered_map to Python list of tuples
        auto convertPieceMap = [](const std::unordered_map<Square, Piecetype>& pieceMap) {
            std::vector<std::pair<Square, Piecetype>> result;
            for (const auto& entry : pieceMap) {
                result.emplace_back(entry.first, entry.second);
            }
            return result;
        };
        // Convert white and black piece maps
        auto whiteList = convertPieceMap(whitePieceMap);
        auto blackList = convertPieceMap(blackPieceMap);
        
        // Return as a tuple of two lists
        return std::make_tuple(whiteList, blackList);
    }, "getPieceMaps");
    
    boardClass.def("getGameResult", &Board::getGameResult, py::arg("noLegalMoves"));
    boardClass.def("getBoardState", &Board::getBoardState);
    boardClass.def("clearBoard", &Board::clearBoard);
    boardClass.def("initializeFromFEN", &Board::initializeFromFEN, py::arg("fen_string"));

    boardClass.def("getPossibleMoves", [](Board& board) {
        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        return moves;
    }, "getPossibleMoves");
    
    boardClass.def("doMove", [](Board& board, Square fromSquare, Square targetSquare, Piecetype promotionPiece = Piecetype::NoType) {
        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        for (const auto& move : moves) {
            if ((move.fromSquare == fromSquare) && (move.targetSquare == targetSquare) && (move.promotionPiece == promotionPiece)) {
                board.doMove(move);
                break;
            }
        }
    }, "doMove");

    boardClass.def("undoMove", [](Board& board) {
        board.undoMove();
    }, "undoMove");

    boardClass.def("inCheck", [](Board& board) {
        return board.inCheck();
    }, "inCheck");

    boardClass.def("getHistory", [](Board& board) {
        std::stack<MoveCommand> originalHistory = board.getHistory();
        
        std::vector<Move> _history;
        while (!originalHistory.empty()) {
            _history.push_back(originalHistory.top().move);
            originalHistory.pop();
        }
        
        std::reverse(_history.begin(), _history.end());
        
        return _history;
    }, "getHistory");


}

void bindBoard(py::module& m) {
    py::class_<Board> myBoard(m, "Board");
    myBoard.def(py::init<const char *, std::string &>());
    bindBoardGetters(myBoard);
}

// void bindGameFunctions(py::class_<Game>& gameClass) {
//     gameClass.def("start", &Game::start);
//     gameClass.def("setFEN", &Game::setFEN, py::arg("FENString"));
//     gameClass.def("resetBoard", &Game::resetBoard);
//     gameClass.def("setPlayer", &Game::setPlayer, py::arg("color"), py::arg("player"));
// }

void bindGame(py::module& m) {
    py::class_<Game> myGame(m, "Game");
    myGame.def(py::init<const std::string&, const std::string&, const char*>(), py::arg("whitePlayer"), py::arg("blackPlayer"), py::arg("FENString"));
    // bindGameFunctions(myGame);
}

void bindPlayer(py::module& m) {
    py::class_<Player, std::unique_ptr<Player>> myPlayer(m, "Player");
    
    myPlayer.def("decideOnMove", &Player::decideOnMove);
    myPlayer.def("getPlayerType", &Player::getPlayerType);

    m.def("makePlayer", [](const std::string& playerTypeString) { return PlayerFactory::makePlayer(playerTypeString); });

    py::enum_<PlayerType>(m, "PlayerType")
        .value("Human", PlayerType::Human)
        .value("Random", PlayerType::Random)
        .value("MinMax", PlayerType::MinMax)
        .value("MonteCarlo", PlayerType::MonteCarlo)
        .value("AlphaBeta", PlayerType::AlphaBeta);

    // m.attr("player_type_map") = py::dict(stringPlayerTypeMap);
    // m.attr("player_type_str_map") = py::dict(playerTypeStringMap);
}
