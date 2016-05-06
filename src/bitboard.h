#pragma once
#include "types.h"

typedef uint64_t bitboard_t;

namespace bb {

void init();

/* Bitboard Accessors */

bitboard_t rank(Rank r);
bitboard_t file(int f);
bitboard_t relative_rank(Color c, Rank r);

// Leaper attacks
bitboard_t pattacks(Color c, int sq);
bitboard_t pawn_span(Color c, int sq);
bitboard_t nattacks(int sq);
bitboard_t kattacks(int sq);

// Slider attacks
bitboard_t battacks(int sq, bitboard_t occ);
bitboard_t rattacks(int sq, bitboard_t occ);
bitboard_t bpattacks(int sq);    // pseudo-attacks (empty board)
bitboard_t rpattacks(int sq);    // pseudo-attacks (empty board)

bitboard_t segment(int sq1, int sq2);
bitboard_t ray(int sq1, int sq2);

/* Bit manipulation */

bool test(bitboard_t b, int sq);
void clear(bitboard_t& b, int sq);
void set(bitboard_t& b, int sq);
bitboard_t shift(bitboard_t b, int i);

int lsb(bitboard_t b);
int msb(bitboard_t b);
int pop_lsb(bitboard_t& b);

bool several(bitboard_t b);
int count(bitboard_t b);

/* Debug */

void print(bitboard_t b);

}    // namespace bb
