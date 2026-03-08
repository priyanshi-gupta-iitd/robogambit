#include "bitboard_eval.h"

//EVALUATION FUNCTION: THE HEURISTICS THAT WE NEED TO WORK ON:
//WRITING THE BEST POSITION TABLES FOR EACH CHESS PIECE:
const int knight_pst[36] = {
    -10, -10, -10, -10, -10, -10,
    -10,   5,   0,   0,   5, -10,
    -10,   5,  20,  20,   5, -10,
    -10,   5,  20,  20,   5, -10,
    -10,   5,   0,   0,   5, -10,
    -10, -10, -10, -10, -10, -10
};

int evaluate(const BitBoardState& state){
    int score =0;

    uint64_t knights = state.w_knights;
    int i=0;
    while(knights){
        if(knights%2 ==1 ){
            score+=300*knight_pst[i];
        }
        i++;
        knights>>2;
    }

    
}