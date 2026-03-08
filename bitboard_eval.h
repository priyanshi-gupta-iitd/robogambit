#ifndef BITBOARD_EVAL_H
#define BITBOARD_EVAL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "files.h"
//data structures 

struct BitBoardState{
    uint64_t w_pawns, w_king, w_queen, w_bishops, w_knights;
    uint64_t b_pawns, b_king, b_queen, b_bishops, b_knights;

    uint64_t w_occ = 0;
    uint64_t b_occ = 0;
    uint64_t empty = ~0ULL;
};


// Move Encoding: 
// Bits 0-5: Source Square (0-35)
// Bits 6-11: Target Square (0-35)
// Bits 12-15: Flags (e.g., 0 = quiet, 1 = capture, 2 = promotion)
inline uint16_t encode_move(int src, int dst, int flag = 0) {
    return (src & 0x3F) | ((dst & 0x3F) << 6) | ((flag & 0xF) << 12);
}
//IMP, SEEE WHAT & IS FOR AND 0x3F IS FOR
//"PARSING THE MOVE NUMBER TO GET THE SOURCE (SRC) AND DESTINATION (DST) AND THE MOVE TYPE LIKE CAPTURE ETC"
inline int get_src(uint16_t move) { return move & 0x3F; }
inline int get_dst(uint16_t move) { return (move >> 6) & 0x3F; }
inline int get_flag(uint16_t move) { return (move >> 12) & 0xF; }

int evaluate(const BitBoardState& );
#endif