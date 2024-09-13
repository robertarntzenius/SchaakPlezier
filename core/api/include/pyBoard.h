#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "board.h"

namespace py = pybind11;

void bindBoard(py::module& m) {
    py::class_<Board> pyBoard(m, "Board");
    pyBoard.def(
        py::init<const char *, std::string &>(),
        py::arg("fen_string"),
        py::arg("log_file")
    );

    pyBoard.def("getPieceMap", [](Board& board, Color color) -> py::dict {
        auto convertPieceMapToDict = [](const std::unordered_map<Square, Piecetype>& pieceMap) {
            py::dict result;
            for (const auto& entry : pieceMap) {
                py::int_ squareValue = static_cast<int>(entry.first);  // Convert Square to Python int type
                result[squareValue] = entry.second;
            }
            return result;
        };
        switch (color) {
            case Color::White:
            case Color::Black:
                return convertPieceMapToDict(board.getPieceMap(color));
            default:
                throw std::invalid_argument("Invalid color: " + std::to_string(color) + ". Choose White or Black");
        }
    }), py::arg("color");

    pyBoard.def("getGameResult", &Board::getGameResult, py::arg("noLegalMoves"));
    pyBoard.def("getBoardState", &Board::getBoardState);
    pyBoard.def("clearBoard", &Board::clearBoard);
    pyBoard.def("initFromFEN", &Board::initFromFEN, py::arg("fen_string"));

    pyBoard.def("getPossibleMoves", [](Board& board) {
        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        return moves;
    }, "getPossibleMoves");

    pyBoard.def("doMove", [](Board& board, Square fromSquare, Square targetSquare, Piecetype promotionPiece = Piecetype::NoType) {
        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        for (const auto& move : moves) {
            if ((move.fromSquare == fromSquare) && (move.targetSquare == targetSquare) && (move.promotionPiece == promotionPiece)) {
                board.doMove(move);
                break;
            }
        }
    }, py::arg("from_square"), py::arg("target_square"), py::arg("promotion_piece") = Piecetype::NoType, "doMove");

    pyBoard.def("findPiece", [](Board& board, Square square) {
        return board.pieceAtSquare(square) != PieceInfo();
    }, py::arg("square"), "addPiece");

    pyBoard.def("getPiece", [](Board& board, Square square) {
        PieceInfo piece = board.pieceAtSquare(square);
        if (piece == PieceInfo()) {
            throw std::invalid_argument("No piece found at square: " + std::to_string(piece.square));
        }
        return py::make_tuple(piece.color, piece.type, piece.square);
    }, py::arg("square"), "addPiece");

    pyBoard.def("addPiece", [](Board& board, Color color, Piecetype type, Square square) {
        board.addPiece(color, type, square);
    }, py::arg("color"), py::arg("piecetype"), py::arg("square"), "addPiece");

    pyBoard.def("removePiece", [](Board& board, Color color, Piecetype type, Square square) {
        board.removePiece(color, type, square);
    }, py::arg("color"), py::arg("piecetype"), py::arg("square"), "removePiece");

    pyBoard.def("validate", [](Board& board) {
        return board.try_validate();
    }, "validate");

    pyBoard.def("undoMove", [](Board& board) {
        board.undoMove();
    }, "undoMove");

    pyBoard.def("inCheck", [](Board& board) {
        return board.inCheck();
    }, "inCheck");

    pyBoard.def("getHistory", [](Board& board) {
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
