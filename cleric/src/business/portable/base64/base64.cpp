/*
 * Copyright (c) 2013 Adam Rudd.
 * See LICENSE for more information
 */

#include "base64.hpp"

#ifdef __AVR_ATmega32U4__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#define read_uint32(ptr) (*(uint32_t *)(ptr))
#define read_uint8(ptr) (*(uint8_t *)(ptr))
#define read_ptr(ptr) (*(uint8_t **)ptr)
#endif

/* b64_alphabet:
 * 		Description: Base64 alphabet table, a mapping between integers
 * 					 and base64 digits
 * 		Notes: This is an extern here but is defined in Base64.c
 */
// MPP adjusted the table removing + and /  (-_ instead) in order to use in URL
const char PROGMEM b64_alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

/* 'Private' declarations */
inline void a3_to_a4(uint8_t *a4, uint8_t *a3);
inline void a4_to_a3(uint8_t *a3, uint8_t *a4);
inline uint8_t b64_lookup(int8_t c);

size_t base64_encode(char *output, const uint8_t *input, size_t inputLen) {
  int16_t i = 0, j = 0;
  size_t encLen = 0;
  uint8_t a3[3];
  uint8_t a4[4];

  while (inputLen--) {
    a3[i++] = *(input++);
    if (i == 3) {
      a3_to_a4(a4, a3);

      for (i = 0; i < 4; i++) {
        output[encLen++] = read_uint8(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for (j = 0; j < i + 1; j++) {
      output[encLen++] = read_uint8(&b64_alphabet[a4[j]]);
    }

    while ((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

size_t base64_decode(uint8_t *output, const char *input, size_t inputLen) {
  int16_t i = 0, j = 0;
  int16_t decLen = 0;
  uint8_t a3[3];
  uint8_t a4[4];

  while (inputLen--) {
    if (*input == '=') {
      break;
    }

    a4[i++] = *(input++);
    if (i == 4) {
      for (i = 0; i < 4; i++) {
        a4[i] = b64_lookup(a4[i]);
      }

      a4_to_a3(a3, a4);

      for (i = 0; i < 3; i++) {
        output[decLen++] = a3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      a4[j] = '\0';
    }

    for (j = 0; j < 4; j++) {
      a4[j] = b64_lookup(a4[j]);
    }

    a4_to_a3(a3, a4);

    for (j = 0; j < i - 1; j++) {
      output[decLen++] = a3[j];
    }
  }
  // TODO MPP 20-Jan-2018 - Remove after the tests - below corrupts memory
  // giving nothing in return output[decLen] = '\0';
  return decLen;
}

size_t base64_enc_len(size_t inputLen) {
  return (inputLen + 2 - ((inputLen + 2) % 3)) / 3 * 4;
}

size_t base64_dec_len(const char *input, size_t inputLen) {
  size_t i = 0;
  size_t numEq = 0;
  for (i = inputLen - 1; input[i] == '='; i--) {
    numEq++;
  }

  return ((6 * inputLen) / 8) - numEq;
}

inline void a3_to_a4(uint8_t *a4, uint8_t *a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

inline void a4_to_a3(uint8_t *a3, uint8_t *a4) {
  a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
  a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
  a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

inline uint8_t b64_lookup(int8_t c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 71;
  if (c >= '0' && c <= '9') return c + 4;
  if (c == '-') return 62;
  if (c == '_') return 63;
  return -1;
}
