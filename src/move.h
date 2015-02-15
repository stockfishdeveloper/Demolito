#pragma once
#include "types.h"

class Position;

struct PinInfo {
	bitboard_t pinned;		// pinned pieces
	bitboard_t discoCheckers;	// discovery checkers

	bitboard_t hidden_checkers(const Position& b, int attacker, int blocker) const;
	PinInfo(const Position& pos);
};

typedef uint16_t move_t;	// fsq:6, tsq:6, prom: 3 (NB_PIECE if none)

struct Move {
	int fsq, tsq, prom;

	bool ok() const;
	move_t encode() const;
	void decode(move_t em);

	bool gives_check(const Position& pos, const PinInfo& pi) const;
};
