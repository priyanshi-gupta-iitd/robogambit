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
        uint64_t top_left_captures = ((state.w_pawns & NOT_A_FILE)<<5) & state.b_occ;
        uint64_t tl_captures_copy = top_left_captures;
        while (tl_captures_copy) {
            int dst = __builtin_ctzll(tl_captures_copy);
            int src = dst - 5; // Reverse the shift to find the origin
            moves.push_back(encode_move(src, dst, 1)); //1 for capture
            tl_captures_copy &= (tl_captures_copy - 1); //this is NOT address wala &, it is boolean, this erases the lowermost set bit in a vector. 
        }
        // TODO: Implement (state.w_pawns << 7) & state.b_occ & NOT_F_FILE
        uint64_t top_right_captures = ((state.w_pawns & NOT_F_FILE)<<7) & state.b_occ;
        uint64_t tr_captures_copy = top_right_captures;
        while (tr_captures_copy) {
            int dst = __builtin_ctzll(tr_captures_copy);
            int src = dst - 7; // Reverse the shift to find the origin
            moves.push_back(encode_move(src, dst, 1));
            tr_captures_copy &= (tr_captures_copy - 1); //this is NOT address wala &, it is boolean, this erases the lowermost set bit in a vector. 
        }

        // --- KNIGHT MOVES ---
        uint64_t knights_copy = state.w_knights;
        uint64_t valid_squares = ~state.w_occ & BOARD_MASK; // Phantom bounds sealed

        // Iterate through each knight one by one
        while (knights_copy) {
            // 1. Isolate the current knight's source index
            int src = __builtin_ctzll(knights_copy);
            
            // 2. Create a bitboard containing ONLY this single knight
            uint64_t single_knight = (1ULL << src);
            
            // 3. Generate all 8 targets for THIS knight only
            uint64_t targets = 0;
            
            targets |= ((single_knight & NOT_A_FILE)  << 11);
            targets |= ((single_knight & NOT_F_FILE)  << 13);
            targets |= ((single_knight & NOT_AB_FILE) << 4);
            targets |= ((single_knight & NOT_EF_FILE) << 8);
            targets |= ((single_knight & NOT_A_FILE)  >> 11);
            targets |= ((single_knight & NOT_F_FILE)  >> 13);
            targets |= ((single_knight & NOT_AB_FILE) >> 4);
            targets |= ((single_knight & NOT_EF_FILE) >> 8);
            
            // 4. Mask the targets with valid_squares to filter out friendly fire and phantom bounds
            targets &= valid_squares;
            
            // 5. Extract the valid destinations for this specific knight
            while (targets) {
                int dst = __builtin_ctzll(targets);
                
                // Check if the destination contains a black piece to set the capture flag
                int flag = (state.b_occ & (1ULL << dst)) ? 1 : 0;
                
                moves.push_back(encode_move(src, dst, flag));
                
                targets &= (targets - 1); // Clear the processed target
            }
            
            // 6. Clear the processed knight and move to the next one
            knights_copy &= (knights_copy - 1); 
        }

        //BIshops moves: RAY CASTING METHODS:
        uint64_t bishops_copy = state.w_bishops;
        // Process each bishop one by one to preserve the exact 'src' square
        while (bishops_copy) {
            int src = __builtin_ctzll(bishops_copy);
            uint64_t single_bishop = (1ULL << src);
            uint64_t targets = 0;
            uint64_t ray;

            // 1. Up-Left Ray (Left Shift by 5)
            ray = (single_bishop & NOT_A_FILE) << 5;
            while (ray & BOARD_MASK) {        // Enforce the 36-bit boundary!
                if (ray & state.w_occ) break; // Blocked by friendly piece -> stop ray
                
                targets |= ray;               // It's empty or an enemy -> valid square
                
                if (ray & state.b_occ) break; // Captured an enemy piece -> stop ray
                
                ray = (ray & NOT_A_FILE) << 5; // Step the ray one more square Up-Left
            }

            // 2. Up-Right Ray (Left Shift by 7)
            ray = (single_bishop & NOT_F_FILE) << 7;
            while (ray & BOARD_MASK) {
                if (ray & state.w_occ) break;
                targets |= ray;
                if (ray & state.b_occ) break;
                ray = (ray & NOT_F_FILE) << 7;
            }

            // 3. Down-Right Ray (Right Shift by 5)
            // Note: Right shifts drop bits into oblivion, so they don't need BOARD_MASK
            ray = (single_bishop & NOT_F_FILE) >> 5; 
            while (ray) { 
                if (ray & state.w_occ) break;
                targets |= ray;
                if (ray & state.b_occ) break;
                ray = (ray & NOT_F_FILE) >> 5;
            }

            // 4. Down-Left Ray (Right Shift by 7)
            ray = (single_bishop & NOT_A_FILE) >> 7;
            while (ray) {
                if (ray & state.w_occ) break;
                targets |= ray;
                if (ray & state.b_occ) break;
                ray = (ray & NOT_A_FILE) >> 7;
            }

            // Extract all valid destinations for THIS specific bishop
            while (targets) {
                int dst = __builtin_ctzll(targets);
                
                // Flag is 1 if destination contains a black piece, 0 otherwise
                int flag = (state.b_occ & (1ULL << dst)) ? 1 : 0; 
                
                moves.push_back(encode_move(src, dst, flag));
                
                targets &= (targets - 1); // Erase the processed target
            }

            bishops_copy &= (bishops_copy - 1); // Erase the processed bishop, move to next
        }
    } else {
        // TODO: Implement Black's move generation (shifting DOWN, e.g., >> 6)
    }

    return moves;
}