
/* STEPHEN A. FRIEDMAN
   CSE 202
   2/8/2016


   
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
      
       //0x55 gives the byte 01010101. By shifting that leftward 8
       //bits then ORing with itself we have 16 bits of the
       //0101...pattern. Then shifting these 16 bits over and ORing
       //with the previous 16 bit number gives 32 bits following the
       //0101...pattern. ANDing this number with x give us all of the
       //even numbered bit locations where x had 1's. Taking our
       //0101... patterned number again (called u) and XORing with x
       //tells us the opposite of if x conatined
       //allEvenBits. Therefore I took the opposite logic of this
       //using the ! operator and returned this answer. 
       int t =  (((0x55<<8)|0x55)<<16);
       int u = t| ((0x55<<8)|0x55);
       int y = u&x;
       int z = !(u^y);
       return z;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  //y will always be 1 (assuming x is anything but 0) beacuse
  //any number (besides 0) ANDed with its negative value
  //will result in 1.
  int y = x&(~x+1);
  //inside here we get -1>>31 (assuming y is not 0) which gives -1. Taking the complement we
  //get 0. Finally 0&1 gives us 0.
  //The only event where these things change is when x is infact 0 and
  //the result will come to be 1.
  return (~((~(y)+1)>>31))&1; 
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  /* int y = 1<<highbit;
  int neg = 0 +(~1+1);
  int x = neg<<(lowbit+(~1+1));
  int u = x^neg;
  u = u<<((highbit+ (~lowbit+1))+1);
  return y|u;
  */

  /*int m = 1<<31;
  int negLowBit=(~lowbit+1);
  int n = ~(m>>(30+(~highbit+1)));
  int o = ~(m>>(31+(negLowBit)));
  //int p = o<<(highbit+ (negLowBit)+1);
  return o^n;
  
  int m = 1<<31;
  int high = ~((~1+1)<<(highbit+1));
  int k = highbit+(~lowbit+1)+1;
  int low = ~(m>>(31+(~k+1)));
  int move = low<<(highbit+1+(~k+1));
  int result = (high & move);
  return result;
*/

  //Gets a bunch of 1s shifted over highbit times to the left
  int a = (~1+1)<<highbit;
  //does the same as the last line but with lowbit
  int b= (~1+1)<<lowbit;
  //creates a mask of 1s that I will use in the next line
  int c = 1<<highbit;
  //XOR a and b to create a number containing their differing bits
  //which contains the numbers of 1s equal to highbit minus lowbit.
  //Then OR it with c to get the mask of all 1s that is the length of highbit.
  int d = (a^b)|c;
  //Finally this line is a safeguard against the few cases where there
  //exist bits that are 1 within the range of lowbit number of
  //bits. By shifting right the lowbit number of times, we chop off
  //these extra bits and replace them back with 0s.
  return (d>>lowbit)<<lowbit;
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
  //this brute force approach uses a counter to track the number of
  //1's that appear in x. It iterates over all 32 bits and tallies the
  //number of 1's which is stored in the counter variable. Finally the
  //last line of code checks if the counter is an odd or even number
  //of 1s. If it is an even number, then x has an even number of
  //0s. If it is an odd number, then x has an odd number of 0s.
  int counter = 0;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  counter = counter + (x&1);
  x=x>>1;
  return counter&1;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  //t becomes -1 if x does not equal 0.  if x is 0, then t becomes 0.
  int t = ~(!!x)+1;
  //plays out the if/else logic.
  //the code before the OR sign will give the value of y if t is -1
  //otherwise it will give a 0 if t is 0. The code to the right of the
  //OR sign will give z if t is 0 otherwise it will give 0 if t is
  //1. Fianlly by ORing these two halves together, where one side will
  //have a non-zero value and the other side will have a 0 value, we
  //will return the non 0 value.
  return (t&y)|((~t)&z);  
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  //first line gets me the MSB
  int z = x>>31;
  //this line gets me if the MSB is 1 or 0 by converting z from -1 to
  //1 or z from 0 to 0
  int y = !!(z);
  //combining these together yields x if x>0 otherwise, it becomes x
  //added with (2 raised to the n power)added with -1
  int w =  (x +(y<<n) +z);
  //finally shifting the bits rightward n times serves as the dividing
  //by (2 raised to the n power) to give the answer
  return w  >> n; 
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
  return uf;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
 return 2;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  return 2;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  return 2;
}
/* 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
  //shifting then ORing allows me to find the highest significant bit
  //so the last value a had will be equivalent to the MSB that is a 1 followed by
  //all ones. The last line then goes about extracting only the MSB
  int a = x|(x>>1);
  a=(a|(a>>2));
  a=(a|(a>>4));
  a=(a|(a>>8));
  a=(a|(a>>16));
  //extracting the MSB by getting rid of all the other 1s. This is
  //accomploshed by the XOR with a shifted over 1 bit to the right and
  //ORed with the largest negative number which then altogether is
  //ANDed with the final a value. It essentially pulls out the extra
  //ones for me and leaves me with the MSB containing the 1.
  return ((a^(a>>1)) | (1<<31)) & a;
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*
    int diff = 0x39+(~x+1);
  int inRange=(9+(~diff+1));
  int v = inRange>>31;
  int w = !!(inRange+(~10+1));
  return v+w;
  */
  //first I create -48 and 58. Then I create variables that represent
  //how x compares to the lower and higher bounds. By then ORing these
  //two results I can determine if the result if positive or
  //negative. Using the ! sign I change the true/false value. So, if
  //a|b was positive then x was an Ascii Digit. If a|b resulted in a
  //negative, then x was not an Ascii Digit. 
    int low  = ~0x2F;
    int high = 0x3a;
    int a = x + low;
    int b = ~x + high;
    return !((a|b) >> 31);

}
/*
 * isTmax - returns 1 if x is the maximum two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  //first I create the smallest possible number. If x happens to be
  //the maximum two's complement number, then by adding it to the
  //minimum number the result would be -1. Then adding the 1 at the
  //end will give a value of 0. The zero will then be applied to !
  //which changes the value to 1. All other values for x will result
  //in either a negative or positive number that when applied to the !
  //operator, will result in an answer if 0.
  return !(((1<<31)+x)+1);
}
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {
  //couldnt figure this out either
  return 2;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  //the first line of code is pulling out the 8 bits sequence
  //requested which is determined by n. The second line then uses an
  //AND to only pull out the bits containing 1s which all fall in the
  //least significant 8 bits of the 32 bit number.
  int y = x>>(n<<3);
  int u = 255 & y;
      return u;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  //simply negates x by finding the 2's complement then adding 1
  return ~x+1;
}
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
  //this method, although  brute forced, does work.
  //By artifically recreating the value of
  //01001001001001001001001001001001 I have successfully completed the
  //puzzle. Each of the numbers you see in my code acts as 8 of the
  //bits that go into the overall number. Using shifts and ORing on
  //these bytes, I formulate the final number.
  int f = (((((73<<8)|36)<<8)|146)<<8)|73;
  
  return f;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  //creates the smallest number (-2147483648) by putting the 1 into
  //the MSB position and this makes the number become negative
  return 1<<31;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  return 2;
}
/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x)
{
  //I couldnt get this one to work at all
  return 2;
}
