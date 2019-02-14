/**
Code taken from:
https://barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
*/

#include <cstdint>

#define POLYNOMIAL 0xD8 /* 11011 followed by 0's */

#define POLYNOMIAL_8 0xD8
#define POLYNOMINAL_16 0x1021
#define POLYNOMINAL_32 0x04C11DB7

/*
 * The width of the CRC calculation and result.
 * Modify the typedef for a 16 or 32-bit CRC standard.
 */
typedef uint8_t crc;

#define WIDTH (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))

crc crcSlow(uint8_t const message[], int nBytes) {
  crc remainder = 0;

  /*
   * Perform modulo-2 division, a byte at a time.
   */
  for (int byte = 0; byte < nBytes; ++byte) {
    /*
     * Bring the next byte into the remainder.
     */
    remainder ^= (message[byte] << (WIDTH - 8));

    /*
     * Perform modulo-2 division, a bit at a time.
     */
    for (uint8_t bit = 8; bit > 0; --bit) {
      /*
       * Try to divide the current data bit.
       */
      if (remainder & TOPBIT) {
        remainder = (remainder << 1) ^ POLYNOMIAL;
      } else {
        remainder = (remainder << 1);
      }
    }
  }

  /*
   * The final remainder is the CRC result.
   */
  return (remainder);

} /* crcSlow() */
