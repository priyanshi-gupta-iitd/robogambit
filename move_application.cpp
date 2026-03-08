#include "move_application.h"

// ---------------------------------------------------------------------------
// 3. MOVE APPLICATION
// ---------------------------------------------------------------------------

//IMPORTANT!
//YET TO ACCOMODATE PAWN PROMOTION HERE!
void apply_move (BitBoardState& state, uint16_t move, int side) {
    int src = get_src(move);
    int dst = get_dst(move);
    
    uint64_t src_mask = (1ULL << src);
    uint64_t dst_mask = (1ULL << dst);
    uint64_t toggle_mask = src_mask | dst_mask; // Has 1s at both src and dst

    if (side == 1) { // White is moving
        // 1. Move the piece (Example assumes it's a knight for simplicity)
        // TODO: You must determine WHICH piece is moving to apply it to the right bitboard.
        // For now, if it was a knight:
        if(state.w_knights & src_mask)
        state.w_knights ^= toggle_mask; // Removes from src, places on dst
        else if(state.w_pawns & src_mask)
        state.w_pawns ^= toggle_mask; // Removes from src, places on dst
        else if(state.w_bishops & src_mask)
        state.w_bishops ^= toggle_mask;
        else if(state.w_king & src_mask)
        state.w_king ^= toggle_mask;
        else
        state.w_queen ^= toggle_mask;

        state.w_occ ^= toggle_mask;
        
        // 2. Handle Captures
        // If the destination square was occupied by black, remove black's piece
        if (state.b_occ & dst_mask) {
            //Figure out which black piece was captured and clear its bit
            state.b_occ &= ~dst_mask;

            if(state.b_knights & dst_mask)
            state.b_knights &= ~dst_mask; // Removes from src, places on dst
            else if(state.b_pawns & dst_mask)
            state.b_pawns &= ~dst_mask; // Removes from src, places on dst
            else if(state.b_bishops & dst_mask)
            state.b_bishops &= ~dst_mask;
            else if(state.b_king & dst_mask)
            state.b_king &= ~dst_mask;
            else
            state.b_queen &= ~dst_mask;
        }
    } else {
        //Implement Black's move application
        if(state.b_knights & src_mask)
        state.b_knights ^= toggle_mask; // Removes from src, places on dst
        else if(state.b_pawns & src_mask)
        state.b_pawns ^= toggle_mask; // Removes from src, places on dst
        else if(state.b_bishops & src_mask)
        state.b_bishops ^= toggle_mask;
        else if(state.b_king & src_mask)
        state.b_king ^= toggle_mask;
        else
        state.b_queen ^= toggle_mask;

        state.b_occ ^= toggle_mask;
        
        // 2. Handle Captures
        // If the destination square was occupied by black, remove black's piece
        if (state.w_occ & dst_mask) {
            //Figure out which black piece was captured and clear its bit
            state.w_occ &= ~dst_mask;

            if(state.w_knights & dst_mask)
            state.w_knights &= ~dst_mask; // Removes from src, places on dst
            else if(state.w_pawns & dst_mask)
            state.w_pawns &= ~dst_mask; // Removes from src, places on dst
            else if(state.w_bishops & dst_mask)
            state.w_bishops &= ~dst_mask;
            else if(state.w_king & dst_mask)
            state.w_king &= ~dst_mask;
            else
            state.w_queen &= ~dst_mask;
        }
    }
    // 36 bits of 1s. Masks out the phantom bits 36-63.
    const uint64_t BOARD_MASK = 0xFFFFFFFFFULL;

    // Update global empty squares
    state.empty = ~(state.w_occ | state.b_occ) & BOARD_MASK;
}