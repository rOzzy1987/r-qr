#ifndef __QR_DATA_H
#define __QR_DATA_H

#include <stdint.h>
#include "qr_types.h"




/**
 * Special characters that can be still used in alphanmeric mode
 * 
 * The order of them defines the byte value they get when encoding
 */
const uint8_t qr_alphanum_chars[] = {
    ' ','$','%','*','+',
    '-','.','/',':'
};

/**
 * Character capacities for different modes
 * 
 * index by [mode], [version] then [ec-level]
 */
const uint16_t qr_capacities[][40][4] = {
    { // QR_MODE_NUM
        /* version  1 */ {  41,  34,  27,  17},
        /* version  2 */ {  77,  63,  48,  34},
        /* version  3 */ { 127, 101,  77,  58},
        /* version  4 */ { 187, 149, 111,  82},
        /* version  5 */ { 255, 202, 144, 106},
        /* version  6 */ { 322, 255, 178, 139},
        /* version  7 */ { 370, 293, 207, 154},
        /* version  8 */ { 461, 365, 259, 202},
        /* version  9 */ { 552, 432, 312, 235},
        /* version 10 */ { 652, 513, 364, 288},
        /* version 11 */ { 772, 604, 427, 331},
        /* version 12 */ { 883, 691, 489, 374},
        /* version 13 */ {1022, 796, 580, 427},
        /* version 14 */ {1101, 871, 621, 468},
        /* version 15 */ {1250, 991, 703, 530},
        /* version 16 */ {1408,1082, 775, 602},
        /* version 17 */ {1548,1212, 876, 674},
        /* version 18 */ {1725,1346, 948, 746},
        /* version 19 */ {1903,1500,1063, 813},
        /* version 20 */ {2061,1600,1159, 919},
        /* version 21 */ {2232,1708,1224, 969},
        /* version 22 */ {2409,1872,1358,1056},
        /* version 23 */ {2620,2059,1468,1108},
        /* version 24 */ {2812,2188,1588,1228},
        /* version 25 */ {3057,2395,1718,1286},
        /* version 26 */ {3283,2544,1804,1425},
        /* version 27 */ {3517,2701,1933,1501},
        /* version 28 */ {3669,2857,2085,1581},
        /* version 29 */ {3909,3035,2181,1677},
        /* version 30 */ {4158,3289,2358,1782},
        /* version 31 */ {4417,3486,2473,1897},
        /* version 32 */ {4686,3693,2670,2022},
        /* version 33 */ {4965,3909,2805,2157},
        /* version 34 */ {5253,4134,2949,2301},
        /* version 35 */ {5529,4343,3081,2361},
        /* version 36 */ {5836,4588,3244,2524},
        /* version 37 */ {6153,4775,3417,2625},
        /* version 38 */ {6479,5039,3599,2735},
        /* version 39 */ {6743,5313,3791,2927},
        /* version 40 */ {7089,5596,3993,3057},
    },{ // QR_MODE_ALPHA
        /* version  1 */ {  25,  20,  16,  10},
        /* version  2 */ {  47,  38,  29,  20},
        /* version  3 */ {  77,  61,  47,  35},
        /* version  4 */ { 114,  90,  67,  50},
        /* version  5 */ { 154, 122,  87,  64},
        /* version  6 */ { 195, 154, 108,  84},
        /* version  7 */ { 224, 178, 125,  93},
        /* version  8 */ { 279, 221, 157, 122},
        /* version  9 */ { 335, 262, 189, 143},
        /* version 10 */ { 395, 311, 221, 174},
        /* version 11 */ { 468, 366, 259, 200},
        /* version 12 */ { 535, 419, 296, 227},
        /* version 13 */ { 619, 483, 352, 259},
        /* version 14 */ { 667, 528, 376, 283},
        /* version 15 */ { 758, 600, 426, 321},
        /* version 16 */ { 854, 656, 470, 365},
        /* version 17 */ { 938, 734, 531, 408},
        /* version 18 */ {1046, 816, 574, 452},
        /* version 19 */ {1153, 909, 644, 493},
        /* version 20 */ {1249, 970, 702, 557},
        /* version 21 */ {1352,1035, 742, 587},
        /* version 22 */ {1460,1134, 823, 640},
        /* version 23 */ {1588,1248, 890, 672},
        /* version 24 */ {1704,1326, 963, 744},
        /* version 25 */ {1853,1451,1041, 779},
        /* version 26 */ {1990,1542,1094, 864},
        /* version 27 */ {2132,1637,1172, 910},
        /* version 28 */ {2223,1732,1263, 958},
        /* version 29 */ {2369,1839,1322,1016},
        /* version 30 */ {2520,1994,1429,1080},
        /* version 31 */ {2677,2113,1499,1150},
        /* version 32 */ {2840,2238,1618,1226},
        /* version 33 */ {3009,2369,1700,1307},
        /* version 34 */ {3183,2506,1787,1394},
        /* version 35 */ {3351,2632,1867,1431},
        /* version 36 */ {3537,2780,1966,1530},
        /* version 37 */ {3729,2894,2071,1591},
        /* version 38 */ {3927,3054,2181,1658},
        /* version 39 */ {4087,3220,2298,1774},
        /* version 40 */ {4296,3391,2420,1852},
    },{ // QR_MODE_BYTE
        /* version  1 */ {  17,  14,  11,   7},
        /* version  2 */ {  32,  26,  20,  14},
        /* version  3 */ {  53,  42,  32,  24},
        /* version  4 */ {  78,  62,  46,  34},
        /* version  5 */ { 106,  84,  60,  44},
        /* version  6 */ { 134, 106,  74,  58},
        /* version  7 */ { 154, 122,  86,  64},
        /* version  8 */ { 192, 152, 108,  84},
        /* version  9 */ { 230, 180, 130,  98},
        /* version 10 */ { 271, 213, 151, 119},
        /* version 11 */ { 321, 251, 177, 137},
        /* version 12 */ { 367, 287, 203, 155},
        /* version 13 */ { 425, 331, 241, 177},
        /* version 14 */ { 458, 362, 258, 194},
        /* version 15 */ { 520, 412, 292, 220},
        /* version 16 */ { 586, 450, 322, 250},
        /* version 17 */ { 644, 504, 364, 280},
        /* version 18 */ { 718, 560, 394, 310},
        /* version 19 */ { 792, 624, 442, 338},
        /* version 20 */ { 858, 666, 482, 382},
        /* version 21 */ { 929, 711, 509, 403},
        /* version 22 */ {1003, 779, 565, 439},
        /* version 23 */ {1091, 857, 611, 461},
        /* version 24 */ {1171, 911, 661, 511},
        /* version 25 */ {1273, 997, 715, 535},
        /* version 26 */ {1367,1059, 751, 593},
        /* version 27 */ {1465,1125, 805, 625},
        /* version 28 */ {1528,1190, 868, 658},
        /* version 29 */ {1628,1264, 908, 698},
        /* version 30 */ {1732,1370, 982, 742},
        /* version 31 */ {1840,1452,1030, 790},
        /* version 32 */ {1952,1538,1112, 842},
        /* version 33 */ {2068,1628,1168, 898},
        /* version 34 */ {2188,1722,1228, 958},
        /* version 35 */ {2303,1809,1283, 983},
        /* version 36 */ {2431,1911,1351,1051},
        /* version 37 */ {2563,1989,1423,1093},
        /* version 38 */ {2699,2099,1499,1139},
        /* version 39 */ {2809,2213,1579,1219},
        /* version 40 */ {2953,2331,1663,1273},
    }
};

uint16_t qr_available_modules(uint8_t version) {
    version++;
    if (version == 1) {
      return 208;
    }
    uint8_t alignmentCount = (version / 7) + 2;
    uint8_t size = (version << 2) + 17;
    return (size * size)
      - 192 // 3 finder patterns
      - (alignmentCount*alignmentCount - 3) * 25 // alignment patterns
      - (((version << 2) + 1) << 1) // timing patterns
      + (alignmentCount - 2) * 10 // timing patterns already covered by alignment patterns
      - 31 // version strip + black module
      - (version > 6 ? 36 : 0); // version polynomial
  }

static const uint16_t qr_block_struct_data[40][4][2] = {
  { { 7,  1}, {10,  1}, {13,  1}, {17,  1} },
  { {10,  1}, {16,  1}, {22,  1}, {28,  1} },
  { {15,  1}, {26,  1}, {18,  2}, {22,  2} },
  { {20,  1}, {18,  2}, {26,  2}, {16,  4} },
  { {26,  1}, {24,  2}, {18,  4}, {22,  4} },
  { {18,  2}, {16,  4}, {24,  4}, {28,  4} },
  { {20,  2}, {18,  4}, {18,  6}, {26,  5} },
  { {24,  2}, {22,  4}, {22,  6}, {26,  6} },
  { {30,  2}, {22,  5}, {20,  8}, {24,  8} },
  { {18,  4}, {26,  5}, {24,  8}, {28,  8} },
  { {20,  4}, {30,  5}, {28,  8}, {24, 11} },
  { {24,  4}, {22,  8}, {26, 10}, {28, 11} },
  { {26,  4}, {22,  9}, {24, 12}, {22, 16} },
  { {30,  4}, {24,  9}, {20, 16}, {24, 16} },
  { {22,  6}, {24, 10}, {30, 12}, {24, 18} },
  { {24,  6}, {28, 10}, {24, 17}, {30, 16} },
  { {28,  6}, {28, 11}, {28, 16}, {28, 19} },
  { {30,  6}, {26, 13}, {28, 18}, {28, 21} },
  { {28,  7}, {26, 14}, {26, 21}, {26, 25} },
  { {28,  8}, {26, 16}, {30, 20}, {28, 25} },
  { {28,  8}, {26, 17}, {28, 23}, {30, 25} },
  { {28,  9}, {28, 17}, {30, 23}, {24, 34} },
  { {30,  9}, {28, 18}, {30, 25}, {30, 30} },
  { {30, 10}, {28, 20}, {30, 27}, {30, 32} },
  { {26, 12}, {28, 21}, {30, 29}, {30, 35} },
  { {28, 12}, {28, 23}, {28, 34}, {30, 37} },
  { {30, 12}, {28, 25}, {30, 34}, {30, 40} },
  { {30, 13}, {28, 26}, {30, 35}, {30, 42} },
  { {30, 14}, {28, 28}, {30, 38}, {30, 45} },
  { {30, 15}, {28, 29}, {30, 40}, {30, 48} },
  { {30, 16}, {28, 31}, {30, 43}, {30, 51} },
  { {30, 17}, {28, 33}, {30, 45}, {30, 54} },
  { {30, 18}, {28, 35}, {30, 48}, {30, 57} },
  { {30, 19}, {28, 37}, {30, 51}, {30, 60} },
  { {30, 19}, {28, 38}, {30, 53}, {30, 63} },
  { {30, 20}, {28, 40}, {30, 56}, {30, 66} },
  { {30, 21}, {28, 43}, {30, 59}, {30, 70} },
  { {30, 22}, {28, 45}, {30, 62}, {30, 74} },
  { {30, 24}, {28, 47}, {30, 65}, {30, 77} },
  { {30, 25}, {28, 49}, {30, 68}, {30, 81} }
};

QrBlockStruct qr_block_struct(uint8_t version, QrEcc ecLevel){
    uint16_t totalWords = qr_available_modules(version) >> 3;
    QrBlockStruct s = {0,0,0,0};
    s.ecWordsPerBlock = qr_block_struct_data[version][ecLevel][0];
    uint16_t totalBlocks = qr_block_struct_data[version][ecLevel][1],
        shortBlockLength = totalWords / totalBlocks;
    s.longBlocks = totalWords - (shortBlockLength * totalBlocks);
    s.dataWordsPerShortBlock = shortBlockLength - s.ecWordsPerBlock;
    s.shortBlocks = totalBlocks - s.longBlocks;
    return s;
}


const uint8_t qr_mode_indicator[] = {1,2,4};
const uint8_t qr_mode_charsPerUnit[] = {3, 2, 1};
const uint8_t qr_mode_unitBitLength[] = {10, 11, 8};

uint8_t qr_lengthBits(uint8_t v, uint8_t mode){
    
    switch(mode) {
        case 0:
            return v<10 ? 10 : v < 27 ? 12 :  14;
        case 1:
            return v<10 ? 9 : v < 27 ? 11 :  13;
        case 2:
            return v<10 ? 8 : 16;
    }
    return 0;
}

uint8_t qr_size(uint8_t v){
    return 17 + (v << 2);
}

/**
 * @brief Generates an array of alignment factors for a QR code of a given
 * version. The array is 0-indexed, with the first element being the position
 * of the first alignment pattern, and the last element being the position of
 * the last alignment pattern.
 * @param[in] v The version of the QR code, 0-based.
 * @param[out] n The size of the returned array.
 * @return An array of alignment factors, 0-indexed. The caller is responsible
 * for freeing the memory when done.
 */
uint8_t *qr_alignment_factors(uint8_t v, uint8_t &n) {
        v++; // switch to 1-based
        if (v == 1) {
            n = 1;
            return new uint8_t[1]{6};
        }

        n = (v/7) + 2; 
        uint8_t *r = new uint8_t[n];
        r[0] = 6;
        r[n-1]= 10 + v * 4;

        if (n == 2) return r;

        uint8_t d = r[n-1] - r[0]; 
        uint8_t s = d/(n-1);
        uint8_t rem = d%(n-1);
        if (rem) s++;
        if (s&1) s++;

        uint8_t x = r[n-1];
        for(uint8_t i =  n-2; i > 0; i--){
            x-= s;
            r[i]=x;
        }
        return r;
}


const uint16_t qr_format_divisor = 0b10100110111;
const uint16_t qr_format_mask = 0b101010000010010;
const uint32_t qr_version_divisor = 0b1111100100101;


#endif //__QR_DATA_H