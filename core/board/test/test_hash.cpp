#include <iostream>
#include <unordered_set>
#include <vector>
#include <random>
#include <chrono>
#include "board.h"
#include <algorithm>
#include <cassert>


Board generateRandomBoard(int maxPiecesPerSide = 5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> pieceDist(0, 12);
    std::uniform_int_distribution<int> countDist(1, std::max(1, maxPiecesPerSide));
    std::uniform_int_distribution<int> rankDist(0, 7);
    std::uniform_int_distribution<int> fileDist(0, 7);
    std::uniform_int_distribution<int> playerDist(0, 1);
    std::uniform_int_distribution<int> castlingDist(0, 15);
    std::uniform_int_distribution<int> halfmoveDist(0, 50);
    std::uniform_int_distribution<int> fullmoveDist(1, 100);

    std::vector<char> pieceChars = {'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q'};
    std::vector<std::string> ranks(8, std::string(8, '1'));


    for (int color = 0; color < 2; ++color) {
        int kingRank = rankDist(gen);
        int Kingfile = fileDist(gen);
        if (ranks[kingRank][Kingfile] == '1') {
            ranks[kingRank][Kingfile] = (color == 0)? 'K': 'k';
        }
        for (const char pieceChar : pieceChars) {
            int count = countDist(gen);
            for (int i = 0; i < count; ++i) {
                int rank = rankDist(gen);
                int file = fileDist(gen);
                if (ranks[rank][file] == '1') {
                    ranks[rank][file] = pieceChar;
                }
            }
        }
    }

    std::string boardString;
    for (const std::string& rank : ranks) {
        int emptyCount = 0;
        for (char square : rank) {
            if (square == '1') {
                ++emptyCount;
            } else {
                if (emptyCount > 0) {
                    boardString += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                boardString += square;
            }
        }
        if (emptyCount > 0) {
            boardString += std::to_string(emptyCount);
        }
        boardString += '/';
    }
    boardString.pop_back();  // Remove the trailing slash

    char activePlayer = playerDist(gen) == 0 ? 'w' : 'b';

    std::string castlingString = "-";
    // int castlingRights = castlingDist(gen);
    // if (castlingRights & 1) castlingString += 'K';
    // if (castlingRights & 2) castlingString += 'Q';
    // if (castlingRights & 4) castlingString += 'k';
    // if (castlingRights & 8) castlingString += 'q';
    // if (castlingString.empty()) castlingString = '-';

    char enPassant = '-';
    if (pieceDist(gen) == 0) {
        char file = 'a' + fileDist(gen);
        char rank = activePlayer == 'w' ? '6' : '3';
        enPassant = *(std::string(1, file) + rank).c_str();
    }

    int halfmoveClock = halfmoveDist(gen);
    int fullmoveNumber = fullmoveDist(gen);
    
    std::string _FEN = boardString + ' ' + activePlayer + ' ' + castlingString + ' ' + enPassant + ' ' + std::to_string(halfmoveClock) + ' ' + std::to_string(fullmoveNumber);
    
    try {
        Board board(_FEN.c_str());
        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        if (moves.size() == 0) {
            return generateRandomBoard();
        }
        return board;
    } catch (const std::exception &e) {
        return generateRandomBoard();
    }
}


void test_incremental_hash_update(size_t numTests) {
    std::cout << "test_incremental_hash_update" << std::endl;
    std::cout << "Number of Tests: " << numTests << std::endl;
    
    std::random_device rd;
    std::mt19937_64 gen(rd());
    size_t fails = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numTests; ++i) {
        Board board = generateRandomBoard();

        std::vector<Move> moves;
        uint64_t hashBeforeFromStruct = board.getBoardState().hash;
        board.getPossibleMoves(moves);
        uint64_t hashAfterMovegen = board.getBoardState().hash;
        assert(hashBeforeFromStruct == hashAfterMovegen);

        std::cout << '(' << i << '/' << numTests << " | move: " << moves[i] << std::endl;

        uint64_t hashBeforeFromFunc = board.hash();
        board.doMove(moves[0]);
        uint64_t hashAfterFromStruct = board.getBoardState().hash;
        uint64_t hashAfterFromFunc = board.hash();

        if ((hashBeforeFromStruct != hashBeforeFromFunc) || (hashAfterFromStruct != hashAfterFromFunc)) {
            fails++;
            std::cout << "Hash mismatch after move: " << moves[0] << std::endl;
            std::cout << "hashBefore" << " | " << "hashAfter" << std::endl;
            std::cout << hashBeforeFromStruct << " | " << hashAfterFromStruct << std::endl;
            std::cout << hashBeforeFromFunc << " | " << hashAfterFromFunc << std::endl;
        }

        assert(hashBeforeFromStruct == hashBeforeFromFunc);
        assert(hashAfterFromStruct == hashAfterFromFunc);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "Number of fails: " << fails << std::endl;
    std::cout << "Failure rate: " << (static_cast<double>(fails) / numTests) * 100 << "%" << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

}


void test_board_hash_collisions(size_t numTests) {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::unordered_set<uint64_t> hashSet;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    
    size_t maxPieces = 7;
    size_t collisionCount = 0;

    std::cout << "Number of Tests: " << numTests * maxPieces << std::endl;
    for (size_t nPieces = 1; nPieces < maxPieces; ++nPieces) {
        for (size_t i = 0; i < numTests; ++i) {
            Board board = generateRandomBoard(nPieces);

            uint64_t hashValue = std::hash<Board>{}(board);

            if (hashSet.find(hashValue) != hashSet.end()) {
                collisionCount++;
                std::cout << board << std::endl;
            } else {
                hashSet.insert(hashValue);
            }
        }
    }

    std::cout << "Number of collisions: " << collisionCount << std::endl;
    std::cout << "Collision rate: " << (static_cast<double>(collisionCount) / (numTests * maxPieces)) * 100 << "%" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}


void test_board_hash_consistency(size_t numTests) {
    std::cout << "test_board_hash_consistency" << numTests << std::endl;
    std::cout << "Number of Tests: " << numTests << std::endl;
    
    std::random_device rd;
    std::mt19937_64 gen(rd());
    size_t collisionCount = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numTests; ++i) {
        Board board = generateRandomBoard();

        uint64_t hashValue1 = std::hash<Board>{}(board);
        uint64_t hashValue2 = std::hash<Board>{}(board);

        if (hashValue1 != hashValue2) {
            collisionCount++;
            std::cout << "Hash mismatch for position:" << std::endl;
            std::cout << board << std::endl;
            std::cout << "Hash 1: " << hashValue1 << std::endl;
            std::cout << "Hash 2: " << hashValue2 << std::endl;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Number of collisions: " << collisionCount << std::endl;
    std::cout << "Collision rate: " << (static_cast<double>(collisionCount) / numTests) * 100 << "%" << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}


int main() {
    test_incremental_hash_update(10000);
    // test_board_hash_collisions(100000);
    // test_board_hash_consistency(10000);
}
