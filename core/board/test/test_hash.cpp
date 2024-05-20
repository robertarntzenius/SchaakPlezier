#include <iostream>
#include <unordered_set>
#include <vector>
#include <random>
#include <chrono>
#include "board.h"
#include <algorithm>
#include <cassert>


std::string generateRandomValidFEN(int attemptsLeft, int maxPiecesPerType) {
    if (attemptsLeft == 0) {
        return "NOT SUCCESFUL";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> countDist(0, std::max(1, maxPiecesPerType));
    std::uniform_int_distribution<int> rankDist(0, 7);
    std::uniform_int_distribution<int> fileDist(0, 7);
    std::uniform_int_distribution<int> playerDist(0, 1);
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
    boardString.pop_back();

    char activePlayer = playerDist(gen) == 0 ? 'w' : 'b';

    char enPassant = '-';
    int halfmoveClock = halfmoveDist(gen);
    int fullmoveNumber = fullmoveDist(gen);

    
    int countK = std::count(boardString.begin(), boardString.end(), 'K');
    int countk = std::count(boardString.begin(), boardString.end(), 'k');
    if ( (countK != 1) || (countk != 1) ) {
        return generateRandomValidFEN(attemptsLeft - 1, maxPiecesPerType);
    }

    std::string BlackStartingRank = boardString.substr(0, boardString.find('/'));
    std::string WhiteStartingRank = boardString.substr(boardString.rfind('/') + 1);
    
    int countP = 0;
    int countp = 0;

    countP += std::count(BlackStartingRank.begin(), BlackStartingRank.end(), 'P');
    countP += std::count(WhiteStartingRank.begin(), WhiteStartingRank.end(), 'P');

    countp += std::count(BlackStartingRank.begin(), BlackStartingRank.end(), 'p');
    countp += std::count(WhiteStartingRank.begin(), WhiteStartingRank.end(), 'p');
    
    if (countP + countp != 0) {
        return generateRandomValidFEN(attemptsLeft - 1, maxPiecesPerType);
    }
    
    auto isPieceAt = [](const std::string& rankString, char pieceChar, File file) -> bool {
        int _file = 0;
        for (const char c : rankString) {
            if (std::isdigit(c)) {
                _file += c - '0';
            } else {
                if ((c == pieceChar) && (_file == file)) {
                    return true;
                }
                _file++;
            }
        }
        return false;
    };
    
    std::string castlingString = "KQkq";
    auto removeCastlingRight = [](std::string& castlingString, char right) {
        castlingString.erase(std::remove(castlingString.begin(), castlingString.end(), right), castlingString.end());
        if (castlingString.empty()) {castlingString = '-';}
    };

    if ( (!isPieceAt(WhiteStartingRank, 'K', FileE)) || (!isPieceAt(WhiteStartingRank, 'R', FileH))) {
        removeCastlingRight(castlingString, 'K');
    }
    if ( (!isPieceAt(WhiteStartingRank, 'K', FileE)) || (!isPieceAt(WhiteStartingRank, 'R', FileA))) {
        removeCastlingRight(castlingString, 'Q');
    }
    if ( (!isPieceAt(BlackStartingRank, 'k', FileE)) || (!isPieceAt(BlackStartingRank, 'r', FileH))) {
        removeCastlingRight(castlingString, 'k');
    }
    if ( (!isPieceAt(BlackStartingRank, 'k', FileE)) || (!isPieceAt(BlackStartingRank, 'r', FileA))) {
        removeCastlingRight(castlingString, 'q');
    }

    std::string _FEN = boardString + ' ' + activePlayer + ' ' + castlingString + ' ' + enPassant + ' ' + std::to_string(halfmoveClock) + ' ' + std::to_string(fullmoveNumber);
    
    try {
        Board board(_FEN.c_str());
        std::vector<Move> moves;
        board.getPossibleMoves(moves);

        if ((moves.size() != 0) && (!board.inCheck(~board.getBoardState().activePlayer) )) {
            return _FEN;
        }
        return generateRandomValidFEN(attemptsLeft - 1, maxPiecesPerType);
    } catch (const std::exception &e) {
        return generateRandomValidFEN(attemptsLeft - 1, maxPiecesPerType);
    }
}

void generateTestFENs(size_t amount, const std::string& filename) {
    int maxRetries = 100;
    int piecesPerType = 2;
    size_t count = 0;

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Could not open the file for writing!" << std::endl;
        throw std::runtime_error("Unable to open file: " + filename);
    }

    while(count < amount) {
        std::string FEN = generateRandomValidFEN(maxRetries, piecesPerType);
        if (FEN != "NOT SUCCESFUL") {
            count++;
            outFile << FEN << std::endl;
        }
    }
    outFile.close();
    std::cout << "Generated " << amount << " valid FEN strings in " << filename << std::endl;
}

std::vector<std::string> readFENStringsFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::vector<std::string> fenStrings;
    std::string line;
    while (std::getline(inFile, line)) {
        fenStrings.push_back(line);
    }

    inFile.close();
    return fenStrings;
}

void assertHashEqual(uint64_t hashBefore, uint64_t hashAfter, std::string msg) {
    if (hashBefore != hashAfter) {
        std::cout << "Hash mismatch: " << msg << std::endl;
        std::cout << hashBefore << " | " << hashAfter << std::endl;
    }
    assert(hashBefore == hashAfter);
}

void test_board_incremental_hash_update(std::vector<std::string>& TEST_FENS) {
    std::cout << "test_board_incremental_hash_update" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& FEN : TEST_FENS) {
        Board board(FEN.c_str());

        uint64_t hashBeforeMoveGen = board.computeHashFromScratch();
        uint64_t hashBeforeMoveGen_inc = board.getBoardState().hash;
        assertHashEqual(hashBeforeMoveGen, hashBeforeMoveGen_inc, "hashBeforeMoveGen != hashBeforeMoveGen_inc");
        
        std::vector<Move> moves;
        board.getPossibleMoves(moves);
        
        uint64_t hashAfterMovegen = board.computeHashFromScratch();
        uint64_t hashAfterMovegen_inc = board.getBoardState().hash;
        assertHashEqual(hashAfterMovegen, hashAfterMovegen_inc, "hashAfterMovegen != hashAfterMovegen_inc");
        assertHashEqual(hashBeforeMoveGen_inc, hashAfterMovegen_inc, "hashBeforeMoveGen_inc != hashAfterMovegen_inc"); 

        board.doMove(moves[0]);

        uint64_t hashAfterDoMove = board.computeHashFromScratch();
        uint64_t hashAfterDoMove_inc = board.getBoardState().hash;
        assertHashEqual(hashAfterDoMove_inc, hashAfterDoMove, "hashAfterDoMove_inc != hashAfterDoMove");
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void test_board_hash_collisions(std::vector<std::string>& TEST_FENS) {
    std::cout << "test_board_hash_collisions" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    std::unordered_set<uint64_t> hashSet;
    size_t collisionCount = 0;

    for (const auto& FEN : TEST_FENS) {
        Board board(FEN.c_str());
        uint64_t hashValue = std::hash<Board>{}(board);

        if (hashSet.find(hashValue) != hashSet.end()) {
            collisionCount++;
        } else {
            hashSet.insert(hashValue);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Number of collisions: " << collisionCount << std::endl;
    std::cout << "Collision rate: " << (static_cast<double>(collisionCount) / TEST_FENS.size()) * 100 << "%" << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void test_board_hash_consistency(std::vector<std::string>& TEST_FENS) {
    std::cout << "test_board_hash_consistency" << std::endl;
    
    size_t collisionCount = 0;

    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& FEN : TEST_FENS) {
        Board board(FEN.c_str());
        uint64_t hashValue1 = std::hash<Board>{}(board);
        uint64_t hashValue2 = std::hash<Board>{}(board);
        assertHashEqual(hashValue1, hashValue2, "");
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Number of collisions: " << collisionCount << std::endl;
    std::cout << "Collision rate: " << (static_cast<double>(collisionCount) / TEST_FENS.size()) * 100 << "%" << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

int main() {
    std::string fn = "testFENS.txt";
    int numFENS = 10000;
    
    generateTestFENs(numFENS, fn);
    std::vector<std::string> TEST_FENS = readFENStringsFromFile(fn);

    test_board_incremental_hash_update(TEST_FENS);
    test_board_hash_collisions(TEST_FENS);
    test_board_hash_consistency(TEST_FENS);
}
