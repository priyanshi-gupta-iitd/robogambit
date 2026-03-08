// ---------------------------------------------------------------------------
// 4. MOVE GENERATION
// ---------------------------------------------------------------------------
#include "move_application.h"

std::vector<uint16_t> generate_moves(const BitBoardState& state, int side) {
    std::vector<uint16_t> moves;
    moves.reserve(40); // Pre-allocate to save memory reallocation time

    if (side == 1) { // White
        // --- PAWN PUSHES ---
        // Shift pawns UP by 6, mask with empty squares
        uint64_t single_pushes = (state.w_pawns << 6) & state.empty;
        
        uint64_t pushes_copy = single_pushes;
        while (pushes_copy) {
            int dst = __builtin_ctzll(pushes_copy);
            int src = dst - 6; // Reverse the shift to find the origin
            moves.push_back(encode_move(src, dst));
            pushes_copy &= (pushes_copy - 1); //this is NOT address wala &, it is boolean, this erases the lowermost set bit in a vector. 
        }

        // --- PAWN CAPTURES ---
        // TODO: Implement (state.w_pawns << 5) & state.b_occ & NOT_A_FILE
        const uint64_t A_FILE = 0b000001'000001'000001'000001'000001'000001ULL;
        const uint64_t NOT_A_FILE = ~ A_FILE;
        uint64_t top_left_captures = ((state.w_pawns & NOT_A_FILE)<<5) & state.b_occ;
        uint64_t tl_captures_copy = top_left_captures;
        while (tl_captures_copy) {
            int dst = __builtin_ctzll(tl_captures_copy);
            int src = dst - 5; // Reverse the shift to find the origin
            moves.push_back(encode_move(src, dst, 1)); //1 for capture
            tl_captures_copy &= (tl_captures_copy - 1); //this is NOT address wala &, it is boolean, this erases the lowermost set bit in a vector. 
        }
        // TODO: Implement (state.w_pawns << 7) & state.b_occ & NOT_F_FILE
        const uint64_t F_FILE = 0b100000'100000'100000'100000'100000'100000ULL;
        const uint64_t NOT_F_FILE = ~ F_FILE;
        uint64_t top_right_captures = ((state.w_pawns & NOT_F_FILE)<<7) & state.b_occ;
        uint64_t tr_captures_copy = top_right_captures;
        while (tr_captures_copy) {
            int dst = __builtin_ctzll(tr_captures_copy);
            int src = dst - 7; // Reverse the shift to find the origin
            moves.push_back(encode_move(src, dst, 1));
            tr_captures_copy &= (tr_captures_copy - 1); //this is NOT address wala &, it is boolean, this erases the lowermost set bit in a vector. 
        }
        // --- KNIGHT MOVES ---
        // TODO: Implement Knight jumps using precalculated attack tables or shift logic.
    } else {
        // TODO: Implement Black's move generation (shifting DOWN, e.g., >> 6)
    }

    return moves;
}