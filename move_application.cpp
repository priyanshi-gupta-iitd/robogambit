#include "bitboard_eval.hpp"

// ---------------------------------------------------------------------------
// 3. MOVE APPLICATION
// ---------------------------------------------------------------------------

void apply_move(BitboardState& state, uint16_t move, int side) {
    int src = get_src(move);
    int dst = get_dst(move);
    
    uint64_t src_mask = (1ULL << src);
    uint64_t dst_mask = (1ULL << dst);
    uint64_t toggle_mask = src_mask | dst_mask; // Has 1s at both src and dst

    if (side == 1) { // White is moving
        // 1. Move the piece (Example assumes it's a knight for simplicity)
        // TODO: You must determine WHICH piece is moving to apply it to the right bitboard.
        // For now, if it was a knight:
        state.w_knights ^= toggle_mask; // Removes from src, places on dst
        state.w_occ ^= toggle_mask;

        // 2. Handle Captures
        // If the destination square was occupied by black, remove black's piece
        if (state.b_occ & dst_mask) {
            // TODO: Figure out which black piece was captured and clear its bit
            // state.b_pawns &= ~dst_mask;
            state.b_occ &= ~dst_mask;
        }
    } else {
        // TODO: Implement Black's move application
    }

    // Update global empty squares
    state.empty = ~(state.w_occ | state.b_occ);
}