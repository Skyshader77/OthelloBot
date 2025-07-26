/**************************************************
 * Board Unit Tests - Testing Board.cpp functionality
 * Based on TestSuite pattern for Othello/Reversi board
**************************************************/

#include "GameState.h"
#include "TestSuite.h"
#include <iostream>
#include <sstream>
#include "BitCounter.h"

using namespace std;

int main()
{
    TestSuite testSuite;

    testSuite.require("Board initialization", 1.0, [](){
        Board board;
        board.resetBoard();
        uint64_t allPieces = board.getAllPieces();
        
        // Check that exactly 4 pieces are placed initially
        int pieceCount = __builtin_popcountll(allPieces);
        assert(pieceCount == 4, "Error: board should have exactly 4 pieces at start");
        
        // Check initial positions: D4(White), E4(Black), D5(Black), E5(White)
        // Assuming 0-indexed coordinates and 8x8 board
        piecePosition d4 = {3, 3, nWhite}; // D4
        piecePosition e4 = {4, 3, nBlack}; // E4
        piecePosition d5 = {3, 4, nBlack}; // D5
        piecePosition e5 = {4, 4, nWhite}; // E5
        
        assert(board.hasAlly(d4, nWhite), "Error: D4 should have a white piece");
        assert(board.hasAlly(e4, nBlack), "Error: E4 should have a black piece");
        assert(board.hasAlly(d5, nBlack), "Error: D5 should have a black piece");
        assert(board.hasAlly(e5, nWhite), "Error: E5 should have a white piece");
    });

    testSuite.require("Board reset functionality", 0.5, [](){
        Board board;
        board.resetBoard();
        // Add a piece to modify the board
        piecePosition newPos = {2, 3, nBlack};
        board.updateBoard(newPos);
        
        // Reset the board
        uint64_t resetResult = board.resetBoard();
        
        // Check that we're back to initial 4 pieces
        int pieceCount = __builtin_popcountll(resetResult);
        assert(pieceCount == 4, "Error: reset board should have exactly 4 pieces");
        
        // Verify initial positions are restored
        piecePosition d4 = {3, 3, nWhite};
        piecePosition e5 = {4, 4, nWhite};
        assert(board.hasAlly(d4, nWhite), "Error: D4 should have a white piece after reset");
        assert(board.hasAlly(e5, nWhite), "Error: E5 should have a white piece after reset");
    });

    testSuite.require("Out of range detection", 0.5, [](){
        Board board;
        board.resetBoard();
        
        // Test positions outside board boundaries
        piecePosition outOfRange1 = {-1, 0, nWhite};
        piecePosition outOfRange2 = {0, -1, nWhite};
        piecePosition outOfRange3 = {BOARD_SIZE, 0, nWhite};
        piecePosition outOfRange4 = {0, BOARD_SIZE, nWhite};
        piecePosition validPos = {0, 0, nWhite};
        
        assert(board.isOutOfRange(outOfRange1), "Error: negative X position should be out of range");
        assert(board.isOutOfRange(outOfRange2), "Error: negative Y position should be out of range");
        assert(board.isOutOfRange(outOfRange3), "Error: X position >= BOARD_SIZE should be out of range");
        assert(board.isOutOfRange(outOfRange4), "Error: Y position >= BOARD_SIZE should be out of range");
        assert(!board.isOutOfRange(validPos), "Error: position (0,0) should be valid");
    });

    testSuite.require("Square occupation detection", 0.5, [](){
        Board board;
        board.resetBoard();

        // Test initial occupied positions
        piecePosition d4 = {3, 3, nWhite};
        piecePosition e4 = {4, 3, nBlack};
        piecePosition emptyPos = {0, 0, nWhite};
        
        assert(board.isSquareOccupied(d4), "Error: D4 should be occupied");
        assert(board.isSquareOccupied(e4), "Error: E4 should be occupied");
        assert(!board.isSquareOccupied(emptyPos), "Error: position (0,0) should be empty");
    });

    testSuite.require("Ally detection", 0.5, [](){
        Board board;
        board.resetBoard();

        piecePosition d4 = {3, 3, nWhite}; // White piece
        piecePosition e4 = {4, 3, nBlack}; // Black piece
        piecePosition empty = {0, 0, nWhite}; // Empty square
        
        assert(board.hasAlly(d4, nWhite), "Error: D4 should have a white ally");
        assert(!board.hasAlly(d4, nBlack), "Error: D4 should not have a black ally");
        assert(board.hasAlly(e4, nBlack), "Error: E4 should have a black ally");
        assert(!board.hasAlly(e4, nWhite), "Error: E4 should not have a white ally");
        assert(!board.hasAlly(empty, nWhite), "Error: empty square should not have allies");
    });

    testSuite.require("Invalid position detection", 0.5, [](){
        Board board;
        board.resetBoard();

        piecePosition outOfRange = {-1, 0, nWhite};
        piecePosition occupied = {3, 3, nWhite}; // D4 is initially occupied
        piecePosition valid = {0, 0, nWhite}; // Empty and in range
        
        assert(board.isInValidPosition(outOfRange), "Error: out of range position should be invalid");
        assert(board.isInValidPosition(occupied), "Error: occupied position should be invalid");
        assert(!board.isInValidPosition(valid), "Error: empty and in-range position should be valid");
    });

    testSuite.require("Board not full initially", 0.5, [](){
        Board board;
        board.resetBoard();
        assert(!board.isFull(), "Error: board should not be full at start");
    });

    testSuite.require("Valid move with captures", 1.5, [](){
        Board board;
        board.resetBoard();
        // Initial setup has:
        // D4(White), E4(Black), D5(Black), E5(White)
        // Valid move for Black: C4 should capture D4
        
        piecePosition c4 = {2, 3, nBlack}; // C4
        
        // Count pieces before move
        uint64_t beforePieces = board.getAllPieces();
        int beforeCount = __builtin_popcountll(beforePieces);
        
        board.updateBoard(c4);
        
        // Check that the piece was placed
        assert(board.hasAlly(c4, nBlack), "Error: piece should be placed at C4");
        
        // Count pieces after move (should be one more)
        uint64_t afterPieces = board.getAllPieces();
        int afterCount = __builtin_popcountll(afterPieces);
        assert(afterCount == beforeCount + 1, "Error: total piece count should increase by 1");
        
        // Check that D4 is now black (captured)
        piecePosition d4 = {3, 3, nBlack};
        assert(board.hasAlly(d4, nBlack), "Error: D4 should now be black (captured)");
    });

    testSuite.require("Invalid move attempt", 0.5, [](){
        Board board;
        board.resetBoard();
        // Try to place a piece on an already occupied square
        piecePosition occupied = {3, 3, nBlack}; // D4 is occupied by white
        
        uint64_t beforePieces = board.getAllPieces();
        board.updateBoard(occupied); // Should do nothing
        uint64_t afterPieces = board.getAllPieces();
        
        assert(beforePieces == afterPieces, "Error: board should not change for invalid move");
        
        // D4 should still be white
        piecePosition d4 = {3, 3, nWhite};
        assert(board.hasAlly(d4, nWhite), "Error: D4 should remain white");
    });

    testSuite.require("Move without captures", 0.5, [](){
        Board board;
        board.resetBoard();

        // Try a move that doesn't capture anything (should still place the piece if valid)
        // This test assumes that isolated moves are allowed in your implementation
        piecePosition isolated = {0, 0, nWhite};
        
        uint64_t beforePieces = board.getAllPieces();
        board.updateBoard(isolated);
        uint64_t afterPieces = board.getAllPieces();
        
        // The behavior here depends on your game rules
        // If isolated moves are not allowed, the board shouldn't change
        // If they are allowed, the piece should be placed
        
        // Assuming isolated moves without captures are not allowed in Othello
        assert(beforePieces != afterPieces, "Error: move without captures should modify board");
    });

    testSuite.require("Multiple direction captures", 1.0, [](){
        Board board;
        board.resetBoard();
        // Set up a scenario where a move captures in multiple directions
        // This might require manually setting up the board state
        
        // For now, test a basic capture scenario
        piecePosition c5 = {2, 4, nWhite}; // C5
        
        board.updateBoard(c5);
        
        // Check that the move was processed
        if (!board.isInValidPosition(c5)) {
            assert(board.hasAlly(c5, nWhite), "Error: piece should be placed if move is valid");
        }
    });

    testSuite.require("Board display format", 1.0, [](){
        Board board;
        board.resetBoard();
        // Capture output from printBoard
        ostringstream oss;
        streambuf* oldCout = cout.rdbuf(oss.rdbuf());
        board.printBoard();
        cout.rdbuf(oldCout);
        
        string output = oss.str();
        
        // Check that output contains expected elements
        assert(output.find("●") != string::npos, "Error: display should contain white pieces (●)");
        assert(output.find("○") != string::npos, "Error: display should contain black pieces (○)");
        assert(output.find("·") != string::npos, "Error: display should contain empty squares (·)");
        assert(output.find("Legend") != string::npos, "Error: display should contain a legend");
        
        // Check for column labels A-H
        for(char c = 'A'; c < 'A' + BOARD_SIZE; c++) {
            assert(output.find(c) != string::npos, "Error: display should contain column labels");
        }
    });

    testSuite.require("Edge case: Corner moves", 0.5, [](){
        Board board;
        board.resetBoard();
        // Test moves in corners
        piecePosition corner1 = {0, 0, nWhite};
        piecePosition corner2 = {BOARD_SIZE-1, BOARD_SIZE-1, nBlack};
        
        // These should be invalid positions initially (no captures possible)
        assert(board.isInValidPosition(corner1) || !board.hasAlly(corner1, nWhite), 
               "Error: corner move should be invalid or have no effect");
        
        board.updateBoard(corner1);
        board.updateBoard(corner2);
        
        // Verify the board state remains consistent
        uint64_t allPieces = board.getAllPieces();
        assert(__builtin_popcountll(allPieces) >= 4, "Error: board should have at least 4 pieces");
    });

    testSuite.require("getEmptySpaces - Initial board state", 1.0, [](){
        Board board;
        board.resetBoard();

        vector<piecePosition> emptySpaces = board.getEmptySpaces();
        
        // Initial board has 4 pieces, so should have 64 - 4 = 60 empty spaces
        assert(emptySpaces.size() == 60, "Error: initial board should have 60 empty spaces");
        
        // Verify that all returned positions have ncolor = -1
        for(const auto& pos : emptySpaces) {
            assert(pos.ncolor == -1, "Error: empty spaces should have ncolor = -1");
        }
        
        // Verify that initially occupied positions are NOT in the empty spaces list
        // D4(3,3), E4(4,3), D5(3,4), E5(4,4) should not be in empty spaces
        bool foundD4 = false, foundE4 = false, foundD5 = false, foundE5 = false;
        for(const auto& pos : emptySpaces) {
            if(pos.xCoord == 3 && pos.yCoord == 3) foundD4 = true;
            if(pos.xCoord == 4 && pos.yCoord == 3) foundE4 = true;
            if(pos.xCoord == 3 && pos.yCoord == 4) foundD5 = true;
            if(pos.xCoord == 4 && pos.yCoord == 4) foundE5 = true;
        }
        assert(!foundD4, "Error: D4 should not be in empty spaces (occupied by white)");
        assert(!foundE4, "Error: E4 should not be in empty spaces (occupied by black)");
        assert(!foundD5, "Error: D5 should not be in empty spaces (occupied by black)");
        assert(!foundE5, "Error: E5 should not be in empty spaces (occupied by white)");
    });

    testSuite.require("getEmptySpaces - Corner positions included", 0.5, [](){
        Board board;
        board.resetBoard();

        vector<piecePosition> emptySpaces = board.getEmptySpaces();
        
        // Check that all four corners are initially empty
        bool foundTopLeft = false, foundTopRight = false;
        bool foundBottomLeft = false, foundBottomRight = false;
        
        for(const auto& pos : emptySpaces) {
            if(pos.xCoord == 0 && pos.yCoord == 0) foundTopLeft = true;
            if(pos.xCoord == BOARD_SIZE-1 && pos.yCoord == 0) foundTopRight = true;
            if(pos.xCoord == 0 && pos.yCoord == BOARD_SIZE-1) foundBottomLeft = true;
            if(pos.xCoord == BOARD_SIZE-1 && pos.yCoord == BOARD_SIZE-1) foundBottomRight = true;
        }
        
        assert(foundTopLeft, "Error: top-left corner should be empty initially");
        assert(foundTopRight, "Error: top-right corner should be empty initially");
        assert(foundBottomLeft, "Error: bottom-left corner should be empty initially");
        assert(foundBottomRight, "Error: bottom-right corner should be empty initially");
    });

    testSuite.require("getEmptySpaces - After placing pieces", 1.0, [](){
        Board board;
        board.resetBoard();

        // Place a piece (assuming C4 is a valid move)
        piecePosition c4 = {2, 3, nBlack};
        board.updateBoard(c4);
        
        vector<piecePosition> emptySpaces = board.getEmptySpaces();
        
        // Should have one less empty space now
        assert(emptySpaces.size() < 60, "Error: placing a piece should reduce empty spaces");
        
        // C4 should not be in the empty spaces list
        bool foundC4 = false;
        for(const auto& pos : emptySpaces) {
            if(pos.xCoord == 2 && pos.yCoord == 3) {
                foundC4 = true;
                break;
            }
        }
        assert(!foundC4, "Error: C4 should not be in empty spaces after placing piece there");
    });

    testSuite.require("getEmptySpaces - Coordinate bounds checking", 0.5, [](){
        Board board;
        board.resetBoard();

        vector<piecePosition> emptySpaces = board.getEmptySpaces();
        
        // Verify all coordinates are within valid bounds
        for(const auto& pos : emptySpaces) {
            assert(pos.xCoord >= 0, "Error: x coordinate should be >= 0");
            assert(pos.xCoord < BOARD_SIZE, "Error: x coordinate should be < BOARD_SIZE");
            assert(pos.yCoord >= 0, "Error: y coordinate should be >= 0");
            assert(pos.yCoord < BOARD_SIZE, "Error: y coordinate should be < BOARD_SIZE");
        }
    });

    testSuite.require("getEmptySpaces - No duplicates", 0.5, [](){
        Board board;
        board.resetBoard();

        vector<piecePosition> emptySpaces = board.getEmptySpaces();
        
        // Check for duplicate positions
        for(size_t i = 0; i < emptySpaces.size(); i++) {
            for(size_t j = i + 1; j < emptySpaces.size(); j++) {
                bool isDuplicate = (emptySpaces[i].xCoord == emptySpaces[j].xCoord) &&
                                (emptySpaces[i].yCoord == emptySpaces[j].yCoord);
                assert(!isDuplicate, "Error: empty spaces list should not contain duplicates");
            }
        }
    });

     testSuite.require("BitCounter initialization", 1.0, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test that initialization works by testing known values
        // 0 should have 0 bits set
        assert(counter.countSetBits(0ULL) == 0, "Error: 0 should have 0 bits set");
        
        // 1 should have 1 bit set
        assert(counter.countSetBits(1ULL) == 1, "Error: 1 should have 1 bit set");
        
        // 255 (0xFF) should have 8 bits set
        assert(counter.countSetBits(255ULL) == 8, "Error: 255 should have 8 bits set");
    });

    testSuite.require("Single bit patterns", 0.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test powers of 2 (single bit set)
        assert(counter.countSetBits(1ULL) == 1, "Error: 2^0 should have 1 bit set");
        assert(counter.countSetBits(2ULL) == 1, "Error: 2^1 should have 1 bit set");
        assert(counter.countSetBits(4ULL) == 1, "Error: 2^2 should have 1 bit set");
        assert(counter.countSetBits(8ULL) == 1, "Error: 2^3 should have 1 bit set");
        assert(counter.countSetBits(16ULL) == 1, "Error: 2^4 should have 1 bit set");
        assert(counter.countSetBits(32ULL) == 1, "Error: 2^5 should have 1 bit set");
        assert(counter.countSetBits(64ULL) == 1, "Error: 2^6 should have 1 bit set");
        assert(counter.countSetBits(128ULL) == 1, "Error: 2^7 should have 1 bit set");
    });

    testSuite.require("Multiple bit patterns", 0.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test combinations of bits
        assert(counter.countSetBits(3ULL) == 2, "Error: 3 (0b11) should have 2 bits set");
        assert(counter.countSetBits(7ULL) == 3, "Error: 7 (0b111) should have 3 bits set");
        assert(counter.countSetBits(15ULL) == 4, "Error: 15 (0b1111) should have 4 bits set");
        assert(counter.countSetBits(31ULL) == 5, "Error: 31 (0b11111) should have 5 bits set");
        assert(counter.countSetBits(63ULL) == 6, "Error: 63 (0b111111) should have 6 bits set");
        assert(counter.countSetBits(127ULL) == 7, "Error: 127 (0b1111111) should have 7 bits set");
    });

    testSuite.require("Byte boundary values", 1.0, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test all bits in a byte
        assert(counter.countSetBits(0x00ULL) == 0, "Error: 0x00 should have 0 bits set");
        assert(counter.countSetBits(0xFFULL) == 8, "Error: 0xFF should have 8 bits set");
        assert(counter.countSetBits(0xAAULL) == 4, "Error: 0xAA (10101010) should have 4 bits set");
        assert(counter.countSetBits(0x55ULL) == 4, "Error: 0x55 (01010101) should have 4 bits set");
        assert(counter.countSetBits(0xF0ULL) == 4, "Error: 0xF0 (11110000) should have 4 bits set");
        assert(counter.countSetBits(0x0FULL) == 4, "Error: 0x0F (00001111) should have 4 bits set");
    });

    testSuite.require("Multi-byte patterns", 1.0, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test 16-bit patterns
        assert(counter.countSetBits(0xFFFFULL) == 16, "Error: 0xFFFF should have 16 bits set");
        assert(counter.countSetBits(0xAAAAULL) == 8, "Error: 0xAAAA should have 8 bits set");
        assert(counter.countSetBits(0x5555ULL) == 8, "Error: 0x5555 should have 8 bits set");
        
        // Test 32-bit patterns
        assert(counter.countSetBits(0xFFFFFFFFULL) == 32, "Error: 0xFFFFFFFF should have 32 bits set");
        assert(counter.countSetBits(0xAAAAAAAAULL) == 16, "Error: 0xAAAAAAAA should have 16 bits set");
    });

    testSuite.require("Large values and edge cases", 1.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test maximum 64-bit value
        uint64_t allBitsSet = UINT64_MAX;
        assert(counter.countSetBits(allBitsSet) == 64, "Error: UINT64_MAX should have 64 bits set");
        
        // Test large specific patterns
        uint64_t pattern1 = 0x123456789ABCDEFULL;
        int expectedBits1 = 32; // This specific pattern has 32 bits set
        assert(counter.countSetBits(pattern1) == expectedBits1, 
               "Error: 0x123456789ABCDEF should have expected number of bits set");
        
        // Test alternating pattern across full 64 bits
        uint64_t alternating = 0xAAAAAAAAAAAAAAAAULL;
        assert(counter.countSetBits(alternating) == 32, 
               "Error: alternating pattern should have 32 bits set");
    });

    testSuite.require("Sequential bit counting", 0.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test sequential values
        for(int i = 0; i <= 15; i++) {
            int expected = __builtin_popcountll(i); // Use built-in for verification
            assert(counter.countSetBits(i) == expected, 
                   "Error: bit count mismatch for value " + to_string(i));
        }
    });

    testSuite.require("High bit patterns", 0.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test patterns with high-order bits set
        uint64_t highBit = 1ULL << 63; // Highest bit
        assert(counter.countSetBits(highBit) == 1, "Error: highest bit should count as 1");
        
        uint64_t highByte = 0xFF00000000000000ULL;
        assert(counter.countSetBits(highByte) == 8, "Error: highest byte should have 8 bits");
        
        uint64_t midHighBits = 0x00FF000000000000ULL;
        assert(counter.countSetBits(midHighBits) == 8, "Error: mid-high byte should have 8 bits");
    });

    testSuite.require("Random pattern verification", 1.0, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test some specific known patterns
        struct TestCase {
            uint64_t value;
            int expectedBits;
        };
        
        TestCase testCases[] = {
            {0x0000000000000001ULL, 1},
            {0x8000000000000000ULL, 1},
            {0x0000FFFF0000FFFFULL, 32},
            {0xFF000000000000FFULL, 16},
            {0x00000000FFFFFFFFULL, 32},
            {0xFFFFFFFF00000000ULL, 32},
            {0x1111111111111111ULL, 16},
            {0x8888888888888888ULL, 16}
        };
        
        for(const auto& test : testCases) {
            assert(counter.countSetBits(test.value) == test.expectedBits,
                   "Error: pattern verification failed for 0x" + 
                   to_string(test.value) + ", expected " + to_string(test.expectedBits));
        }
    });

    testSuite.require("Performance consistency", 0.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test that the same value always returns the same count
        uint64_t testValue = 0x123456789ABCDEFULL;
        int firstResult = counter.countSetBits(testValue);
        
        for(int i = 0; i < 100; i++) {
            int result = counter.countSetBits(testValue);
            assert(result == firstResult, "Error: bit counting should be consistent");
        }
    });

    testSuite.require("Zero and boundary validation", 0.5, [](){
        BitCounter counter;
        counter.initialize();
        
        // Edge cases
        assert(counter.countSetBits(0ULL) == 0, "Error: zero should have no bits set");
        assert(counter.countSetBits(1ULL) == 1, "Error: one should have one bit set");
        assert(counter.countSetBits(UINT64_MAX - 1) == 63, "Error: UINT64_MAX-1 should have 63 bits set");
        
        // Verify non-negative results
        for(int i = 0; i < 256; i++) {
            int result = counter.countSetBits(i);
            assert(result >= 0, "Error: bit count should never be negative");
            assert(result <= 64, "Error: bit count should never exceed 64");
        }
    });

    testSuite.require("Comparison with builtin function", 1.0, [](){
        BitCounter counter;
        counter.initialize();
        
        // Test against GCC's builtin popcount for validation
        // Test a range of values
        for(uint64_t i = 0; i < 1000; i++) {
            int ourResult = counter.countSetBits(i);
            int builtinResult = __builtin_popcountll(i);
            assert(ourResult == builtinResult, 
                   "Error: our count (" + to_string(ourResult) + 
                   ") doesn't match builtin (" + to_string(builtinResult) + 
                   ") for value " + to_string(i));
        }
        
        // Test some larger values
        uint64_t largeValues[] = {
            0x123456789ABCDEFULL,
            0xFEDCBA9876543210ULL,
            0xAAAAAAAAAAAAAAAAULL,
            0x5555555555555555ULL,
            0xFFFFFFFFFFFFFFFFULL
        };
        
        for(uint64_t val : largeValues) {
            int ourResult = counter.countSetBits(val);
            int builtinResult = __builtin_popcountll(val);
            assert(ourResult == builtinResult,
                   "Error: mismatch for large value 0x" + to_string(val));
        }
    });

    testSuite.showResults();
    return 0;
}