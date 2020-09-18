#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64(double number) {
    return *((uint64_t * )(&number));
}

bool getBit(const uint64_t number, const uint8_t index) {
    uint64_t mask = (uint64_t) 1 << index;
    uint64_t temp = mask & number;
    temp >>= index;
    return temp;
}


bool isPositiveNumber(uint64_t number) {
    return !getBit(number, 63);
}

bool isDenormalizedNumber(uint64_t number) {

    //1.approach  via for loop
    /*  bool denormalizedFlag = false;

       for (uint8_t i = 52; i < 63; i++) {
           denormalizedFlag = getBit(number, i) == 0;
           if (!denormalizedFlag)
               break;
       }
       return denormalizedFlag && getBit(number, 0) == 1;
   */
    //2. approach via only mask
    /*((uint16_t) (number >> (uint64_t) 52) | (uint16_t) 0x800) == (uint16_t) 0x800;*/

    bool check_exponent = ((uint16_t)(number >> (uint64_t) 52) | (uint16_t) 0x800) == (uint16_t) 0x800;
    bool check_first_bit = getBit(number, 0) == 1;
    return check_exponent && check_first_bit;
}

bool isNan(uint64_t number) {

    //1.approach  via for loop
/*    bool nanFlag = false;
    for (uint8_t i = 52; i < 63; i++) {
        nanFlag = getBit(number, i) == 1;
        if (!nanFlag)
            break;
    }
    return nanFlag;*/
    //2. approach via only mask
    // ((number >> (uint64_t) 52) | (uint16_t) 0x800) == (uint16_t) 0xFFF

    return ((uint16_t)(number >> (uint64_t) 52) | (uint16_t) 0x800) == (uint16_t) 0xFFF;
}

/**
 * Checkers here:
 */

bool checkForPlusZero(uint64_t number) {
    return number == 0x0;
}

bool checkForMinusZero(uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf(uint64_t number) {
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf(uint64_t number) {
    return number == 0xFFF0000000000000;
}

bool checkForPlusNormal(uint64_t number) {
    return isPositiveNumber(number) && !isDenormalizedNumber(number) && !isNan(number);
}

bool checkForMinusNormal(uint64_t number) {
    return !isPositiveNumber(number) && !isDenormalizedNumber(number) && !isNan(number);
}

bool checkForPlusDenormal(uint64_t number) {
    return isPositiveNumber(number) && isDenormalizedNumber(number) && !isNan(number);
}

bool checkForMinusDenormal(uint64_t number) {
    return !isPositiveNumber(number) && isDenormalizedNumber(number) && !isNan(number);
}

bool checkForSignalingNan(uint64_t number) {
    return isNan(number) && getBit(number, 0) == 1;
}

bool checkForQuietNan(uint64_t number) {
    return isNan(number) && getBit(number, 51) == 1;
}


void classify(double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    } else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    } else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    } else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    } else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    } else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    } else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    } else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    } else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    } else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    } else {
        printf("Error.\n");
    }
}
