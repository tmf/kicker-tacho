// Character set data for 7-segment display
// Eli McIlveen
// 22 July 2007
// http://www.forgeryleague.com/
//
// This code is hereby placed in the public domain.

// Uses ASCII character map except for control characters. Can easily be cut down 
// to save space in memory.
//
// The first 16 characters are hex digits 0-F, to simplify the display of numerals.
// Characters 16-23 and 24-31 provide an animated "figure-8" cycle with one and two
// segments lit at a time, respectively.
//
// For obvious reasons, some letters and punctuation are rather hard to decipher. 
// Rather than duplicate shapes, alternative versions of some characters are offered 
// (e.g. 6 7 9, upper/lowercase letters).

// Bit order: DP A B C D E F G
// Segments A-F run clockwise from the top; G is the middle bar; DP = decimal point.
//
// Pins to connect to common-cathode LED display via a 74HC595:
// DP-15, A-1, B-2, C-3, D-4, E-5, F-6, G-7 (shiftOut using LSBFIRST)
// Or:
// DP-7, A-6, B-5, C-4, D-3, E-2, F-1, G-15 (shiftOut using MSBFIRST)



const byte ledCharSet[128] = {
  // 00-0F: Hex digits
  B11101110, B00101000, B11001101, B01101101,	// 0123
  B00101011, B01100111, B11100011, B00101110,	// 4567
  B11101111, B00101111, B01110111, B00011111,	// 89AB
  B01001110, B00111101, B01001111, B01000111,	// CDEF

  // 10-1F: Figure-8 drawing (8-character cycle)
  B01000000, B00100000, B00000001, B00000100,	// 1-segment
  B00001000, B00010000, B00000001, B00000010,

  B01100000, B00100001, B00000101, B00001100,	// 2-segment
  B00011000, B00010001, B00000011, B01000010,

  // 20-2F: Punctuation (barely recognizable!)
  B00000000, B00011000, B00001010, B11101110,	//  !"#
  B01011010, B01001001, B00000111, B00001000,	// $%&'
  B11000111, B01101101, B01100011, B00110001,	// ()*+
  B00100000, B00000001, B00010000, B10001001,	// ,-./

  // 30-3F: Decimal digits (alternate) and more punctuation
  B11101110, B00101000, B11001101, B01101101,	// 0123
  B00101011, B01100111, B11100011, B00101110,	// 4567
  B11101111, B00101111, B01000001, B01000001,	// 89:;
  B11000001, B01000001, B01100001, B10001101,	// <=>?

  // 40-5F: Capital letters and punctuation
  B01101110, B10101111, B11101111, B11000110,	// @ABC
  B11101110, B11000111, B10000111, B11100110,	// DEFG
  B10101011, B00101000, B01101000, B10101011,	// HIJK
  B11000010, B10101110, B10100001, B11101110,	// LMNO

  B10001111, B11111110, B10101111, B01100111,	// PQRS
  B11000011, B11101010, B11101010, B11101010,	// TUVW
  B10101011, B00101011, B11001101, B11000111,	// XYZ[
  B00100011, B01101101, B01101100, B01000000,	// \]^_

  // 60-7F: Lowercase letters and punctuation
  B01100000, B01111101, B00011111, B00001101,	// `abc
  B00111101, B01101111, B01000111, B01111011,	// defg
  B00010111, B00010000, B00011000, B00101111,	// hijk
  B00001100, B01010101, B01101010, B00011101,	// lmno

  B01100111, B01110011, B00000101, B00010011,	// pqrs
  B00001111, B00011100, B00100011, B01011101,	// tuvw
  B01101100, B00111011, B00100101, B01000011,	// xyz{
  B00110110, B01100001, B01000000, B11111111	// |}~
};
byte swapBits(byte b, int b1, int b2){
  unsigned int x = ((b >> b1) ^ (b >> b2)) & ((1U << 1) - 1); // XOR temporary
  byte r = b ^ ((x << b1) | (x << b2));
  return r;
}


byte translate7SegmentByte(byte src)
{
  byte dst=0;
  /*
  int bit[8];
   for(int i=0; i<8;i++){
   bit[i] = src & (1 << i);
   }
   dst |= bit[5] << 5;
   dst |= bit[4] << 4;
   dst |= bit[3] << 3;
   dst |= bit[0] << 0;
   dst |= bit[2] << 2;
   dst |= bit[1] << 1;
   dst |= bit[6] << 6;
   dst |= bit[7] << 7;*/
  dst = src;
  /*dst = swapBits(dst, 0, 5);
  dst = swapBits(dst, 1, 4);
  dst = swapBits(dst, 2, 3);
  dst = swapBits(dst, 3, 5);
  dst = swapBits(dst, 4, 5);*/
  Serial.print (" ");
  Serial.print (src);
  Serial.print ("=");
  Serial.println (dst);
  return dst;





}


