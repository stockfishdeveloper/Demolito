/*
 * Demolito, a UCI chess engine.
 * Copyright 2015 lucasart.
 *
 * Demolito is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Demolito is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
*/
#include "bitboard.h"

namespace {

const int RShift[NB_SQUARE] = {
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 53, 53, 53, 53, 53
};

const bitboard_t RMagic[NB_SQUARE] = {
    0x0080001020400080ull, 0x0040001000200040ull, 0x0080081000200080ull, 0x0080040800100080ull,
    0x0080020400080080ull, 0x0080010200040080ull, 0x0080008001000200ull, 0x0080002040800100ull,
    0x0000800020400080ull, 0x0000400020005000ull, 0x0000801000200080ull, 0x0000800800100080ull,
    0x0000800400080080ull, 0x0000800200040080ull, 0x0000800100020080ull, 0x0000800040800100ull,
    0x0000208000400080ull, 0x0000404000201000ull, 0x0000808010002000ull, 0x0000808008001000ull,
    0x0000808004000800ull, 0x0000808002000400ull, 0x0000010100020004ull, 0x0000020000408104ull,
    0x0000208080004000ull, 0x0000200040005000ull, 0x0000100080200080ull, 0x0000080080100080ull,
    0x0000040080080080ull, 0x0000020080040080ull, 0x0000010080800200ull, 0x0000800080004100ull,
    0x0000204000800080ull, 0x0000200040401000ull, 0x0000100080802000ull, 0x0000080080801000ull,
    0x0000040080800800ull, 0x0000020080800400ull, 0x0000020001010004ull, 0x0000800040800100ull,
    0x0000204000808000ull, 0x0000200040008080ull, 0x0000100020008080ull, 0x0000080010008080ull,
    0x0000040008008080ull, 0x0000020004008080ull, 0x0000010002008080ull, 0x0000004081020004ull,
    0x0000204000800080ull, 0x0000200040008080ull, 0x0000100020008080ull, 0x0000080010008080ull,
    0x0000040008008080ull, 0x0000020004008080ull, 0x0000800100020080ull, 0x0000800041000080ull,
    0x00FFFCDDFCED714Aull, 0x007FFCDDFCED714Aull, 0x003FFFCDFFD88096ull, 0x0000040810002101ull,
    0x0001000204080011ull, 0x0001000204000801ull, 0x0001000082000401ull, 0x0001FFFAABFAD1A2ull
};

const bitboard_t RMask[NB_SQUARE] = {
    0x000101010101017Eull, 0x000202020202027Cull, 0x000404040404047Aull, 0x0008080808080876ull,
    0x001010101010106Eull, 0x002020202020205Eull, 0x004040404040403Eull, 0x008080808080807Eull,
    0x0001010101017E00ull, 0x0002020202027C00ull, 0x0004040404047A00ull, 0x0008080808087600ull,
    0x0010101010106E00ull, 0x0020202020205E00ull, 0x0040404040403E00ull, 0x0080808080807E00ull,
    0x00010101017E0100ull, 0x00020202027C0200ull, 0x00040404047A0400ull, 0x0008080808760800ull,
    0x00101010106E1000ull, 0x00202020205E2000ull, 0x00404040403E4000ull, 0x00808080807E8000ull,
    0x000101017E010100ull, 0x000202027C020200ull, 0x000404047A040400ull, 0x0008080876080800ull,
    0x001010106E101000ull, 0x002020205E202000ull, 0x004040403E404000ull, 0x008080807E808000ull,
    0x0001017E01010100ull, 0x0002027C02020200ull, 0x0004047A04040400ull, 0x0008087608080800ull,
    0x0010106E10101000ull, 0x0020205E20202000ull, 0x0040403E40404000ull, 0x0080807E80808000ull,
    0x00017E0101010100ull, 0x00027C0202020200ull, 0x00047A0404040400ull, 0x0008760808080800ull,
    0x00106E1010101000ull, 0x00205E2020202000ull, 0x00403E4040404000ull, 0x00807E8080808000ull,
    0x007E010101010100ull, 0x007C020202020200ull, 0x007A040404040400ull, 0x0076080808080800ull,
    0x006E101010101000ull, 0x005E202020202000ull, 0x003E404040404000ull, 0x007E808080808000ull,
    0x7E01010101010100ull, 0x7C02020202020200ull, 0x7A04040404040400ull, 0x7608080808080800ull,
    0x6E10101010101000ull, 0x5E20202020202000ull, 0x3E40404040404000ull, 0x7E80808080808000ull
};

const int BShift[NB_SQUARE] = {
    58, 59, 59, 59, 59, 59, 59, 58,
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59,
    58, 59, 59, 59, 59, 59, 59, 58
};

const bitboard_t BMagic[NB_SQUARE] = {
    0x0002020202020200ull, 0x0002020202020000ull, 0x0004010202000000ull, 0x0004040080000000ull,
    0x0001104000000000ull, 0x0000821040000000ull, 0x0000410410400000ull, 0x0000104104104000ull,
    0x0000040404040400ull, 0x0000020202020200ull, 0x0000040102020000ull, 0x0000040400800000ull,
    0x0000011040000000ull, 0x0000008210400000ull, 0x0000004104104000ull, 0x0000002082082000ull,
    0x0004000808080800ull, 0x0002000404040400ull, 0x0001000202020200ull, 0x0000800802004000ull,
    0x0000800400A00000ull, 0x0000200100884000ull, 0x0000400082082000ull, 0x0000200041041000ull,
    0x0002080010101000ull, 0x0001040008080800ull, 0x0000208004010400ull, 0x0000404004010200ull,
    0x0000840000802000ull, 0x0000404002011000ull, 0x0000808001041000ull, 0x0000404000820800ull,
    0x0001041000202000ull, 0x0000820800101000ull, 0x0000104400080800ull, 0x0000020080080080ull,
    0x0000404040040100ull, 0x0000808100020100ull, 0x0001010100020800ull, 0x0000808080010400ull,
    0x0000820820004000ull, 0x0000410410002000ull, 0x0000082088001000ull, 0x0000002011000800ull,
    0x0000080100400400ull, 0x0001010101000200ull, 0x0002020202000400ull, 0x0001010101000200ull,
    0x0000410410400000ull, 0x0000208208200000ull, 0x0000002084100000ull, 0x0000000020880000ull,
    0x0000001002020000ull, 0x0000040408020000ull, 0x0004040404040000ull, 0x0002020202020000ull,
    0x0000104104104000ull, 0x0000002082082000ull, 0x0000000020841000ull, 0x0000000000208800ull,
    0x0000000010020200ull, 0x0000000404080200ull, 0x0000040404040400ull, 0x0002020202020200ull
};

const bitboard_t BMask[NB_SQUARE] = {
    0x0040201008040200ull, 0x0000402010080400ull, 0x0000004020100A00ull, 0x0000000040221400ull,
    0x0000000002442800ull, 0x0000000204085000ull, 0x0000020408102000ull, 0x0002040810204000ull,
    0x0020100804020000ull, 0x0040201008040000ull, 0x00004020100A0000ull, 0x0000004022140000ull,
    0x0000000244280000ull, 0x0000020408500000ull, 0x0002040810200000ull, 0x0004081020400000ull,
    0x0010080402000200ull, 0x0020100804000400ull, 0x004020100A000A00ull, 0x0000402214001400ull,
    0x0000024428002800ull, 0x0002040850005000ull, 0x0004081020002000ull, 0x0008102040004000ull,
    0x0008040200020400ull, 0x0010080400040800ull, 0x0020100A000A1000ull, 0x0040221400142200ull,
    0x0002442800284400ull, 0x0004085000500800ull, 0x0008102000201000ull, 0x0010204000402000ull,
    0x0004020002040800ull, 0x0008040004081000ull, 0x00100A000A102000ull, 0x0022140014224000ull,
    0x0044280028440200ull, 0x0008500050080400ull, 0x0010200020100800ull, 0x0020400040201000ull,
    0x0002000204081000ull, 0x0004000408102000ull, 0x000A000A10204000ull, 0x0014001422400000ull,
    0x0028002844020000ull, 0x0050005008040200ull, 0x0020002010080400ull, 0x0040004020100800ull,
    0x0000020408102000ull, 0x0000040810204000ull, 0x00000A1020400000ull, 0x0000142240000000ull,
    0x0000284402000000ull, 0x0000500804020000ull, 0x0000201008040200ull, 0x0000402010080400ull,
    0x0002040810204000ull, 0x0004081020400000ull, 0x000A102040000000ull, 0x0014224000000000ull,
    0x0028440200000000ull, 0x0050080402000000ull, 0x0020100804020000ull, 0x0040201008040200ull
};

bitboard_t RMagicDB[0x19000], BMagicDB[0x1480];

const size_t BMagicIndex[NB_SQUARE] = {
    4992, 2624,  256,  896, 1280, 1664, 4800, 5120,
    2560, 2656,  288,  928, 1312, 1696, 4832, 4928,
    0,  128,  320,  960, 1344, 1728, 2304, 2432,
    32,  160,  448, 2752, 3776, 1856, 2336, 2464,
    64,  192,  576, 3264, 4288, 1984, 2368, 2496,
    96,  224,  704, 1088, 1472, 2112, 2400, 2528,
    2592, 2688,  832, 1216, 1600, 2240, 4864, 4960,
    5056, 2720,  864, 1248, 1632, 2272, 4896, 5184
};

const size_t RMagicIndex[64] = {
    86016, 73728, 36864, 43008, 47104, 51200, 77824, 94208,
    69632, 32768, 38912, 10240, 14336, 53248, 57344, 81920,
    24576, 33792,  6144, 11264, 15360, 18432, 58368, 61440,
    26624,  4096,  7168,     0,  2048, 19456, 22528, 63488,
    28672,  5120,  8192,  1024,  3072, 20480, 23552, 65536,
    30720, 34816,  9216, 12288, 16384, 21504, 59392, 67584,
    71680, 35840, 39936, 13312, 17408, 54272, 60416, 83968,
    90112, 75776, 40960, 45056, 49152, 55296, 79872, 98304
};

bitboard_t calc_sliding_attacks(int sq, bitboard_t occ, const int dir[4][2])
{
    const Rank r = rank_of(sq);
    const int f = file_of(sq);
    bitboard_t result = 0;

    for (int i = 0; i < 4; ++i) {
        int dr = dir[i][0], df = dir[i][1];
        Rank _r = r + dr;
        int _f = f + df;

        while (0 <= _r && _r < NB_RANK && file_ok(_f)) {
            const int _sq = square(_r, _f);
            bb::set(result, _sq);

            if (bb::test(occ, _sq))
                break;

            _r += dr, _f += df;
        }
    }

    return result;
}

bitboard_t init_magic_occ(const int* sq, int squareCount, bitboard_t lineOcc)
{
    bitboard_t result = 0;

    for (int i = 0; i < squareCount; ++i)
        if (bb::test(lineOcc, i))
            bb::set(result, sq[i]);

    return result;
}

void init_helper(int sq, const bitboard_t mask[], const bitboard_t magic[], const int shift[],
                 bitboard_t magicDb[], const size_t magicIndex[], const int dir[4][2])
{
    int squares[NB_SQUARE];
    int squareCount = 0;

    bitboard_t temp = mask[sq];

    while (temp)
        squares[squareCount++] = bb::pop_lsb(temp);

    for (temp = 0; temp < (1ULL << squareCount); temp++) {
        bitboard_t occ = init_magic_occ(squares, squareCount, temp);
        const int idx = (occ * magic[sq]) >> shift[sq];
        bitboard_t *p = magicDb + magicIndex[sq];
        p[idx] = calc_sliding_attacks(sq, occ, dir);
    }
}

}    // namespace

namespace bb {

void init_slider_attacks()
{
    const int Bdir[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    const int Rdir[4][2] = {{-1,0},{0,-1},{0,1},{1,0}};

    for (int i = A1; i <= H8; i++) {
        init_helper(i, BMask, BMagic, BShift, BMagicDB, BMagicIndex, Bdir);
        init_helper(i, RMask, RMagic, RShift, RMagicDB, RMagicIndex, Rdir);
    }
}

bitboard_t battacks(int sq, bitboard_t occ)
{
    assert(square_ok(sq));
    const int idx = ((occ & BMask[sq]) * BMagic[sq]) >> BShift[sq];
    return (BMagicDB + BMagicIndex[sq])[idx];
}

bitboard_t rattacks(int sq, bitboard_t occ)
{
    assert(square_ok(sq));
    const int idx = ((occ & RMask[sq]) * RMagic[sq]) >> RShift[sq];
    return (RMagicDB + RMagicIndex[sq])[idx];
}

}    // namespace bb
