// ---------------------------------------------------------------------------
// 5. MINIMAX SEARCH
// ---------------------------------------------------------------------------

#include "minimax.h"
#include "bitboard_eval.h"
#include "move_application.h"
#include "move_generation.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// 5. MINIMAX SEARCH WITH ALPHA-BETA
// ---------------------------------------------------------------------------

int V(BitBoardState state, int depth, int alpha, int beta, int side) {
    if (depth == 0) {
        return evaluate(state); // Static evaluation at leaf nodes 
    }

    std::vector<uint16_t> moves = generate_moves(state, side);
    
    // Terminal State Handling
    if (moves.empty()) {
        if (is_in_check(state, side)) {
            // Checkmate: Returning values relative to depth encourages the AI
            // to find the fastest mate or delay being mated.
            if (side == 1) {
                return -200000 - depth;
            } else {
                return 200000 + depth;
            }
        }
        return 0; // Stalemate
    }

    if (side == 1) { // Maximizing Player (White)
        int max_eval = -999999;
        for (uint16_t move : moves) {
            BitBoardState next_state = state; 
            apply_move(next_state, move, side);

            // 'eval' here is the dynamic value from future boards 
            int eval = V(next_state, depth - 1, alpha, beta, 0);
            max_eval = std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cut-off
            }
        }
        return max_eval;
    } 
    else { // Minimizing Player (Black)
        int min_eval = 999999;
        for (uint16_t move : moves) {
            BitBoardState next_state = state;
            apply_move(next_state, move, side);

            int eval = V(next_state, depth - 1, alpha, beta, 1);
            min_eval = std::min(min_eval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cut-off
            }
        }
        return min_eval;
    }
}

// ---------------------------------------------------------------------------
// 6. ROOT CALLER
// ---------------------------------------------------------------------------

uint16_t get_best_move(BitBoardState state, int depth, int side) {
    std::vector<uint16_t> moves = generate_moves(state, side);
    if (moves.empty()) {
        return 0; 
    }

    uint16_t best_move = moves[0];
    int alpha = -999999;
    int beta = 999999;
    int best_score;

    if (side == 1) {
        best_score = -999999;
    } else {
        best_score = 999999;
    }

    for (uint16_t move : moves) {
        BitBoardState next_state = state;
        apply_move(next_state, move, side);
        
        int eval = V(next_state, depth - 1, alpha, beta, 1 - side);

        if (side == 1) { // White (Maximizer)
            if (eval > best_score) {
                best_score = eval;
                best_move = move;
            }
            alpha = std::max(alpha, best_score);
        } else { // Black (Minimizer)
            if (eval < best_score) {
                best_score = eval;
                best_move = move;
            }
            beta = std::min(beta, best_score);
        }
    }
    return best_move;
}