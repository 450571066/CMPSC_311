/* 
 * CS:APP Data Lab 
 * 
 * Tianqi Liu, tzl60
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
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ( ~ ( ( ~ x ) | ( ~ y ) ) );
		// x & y == ~ ( ~ ( x & y ), ~ ( x & y) = ~ x | ~ y
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  int mask = 0x55 + (0x55 << 8);  //to make a 0x55555555 mask
  mask = mask + (mask << 16);     

  return !((x & mask) ^ mask);    //by taking this mask, x should equals to mask if it is all even bits
}				  //if x with mask == mask, x & mask ^ mask should return 0,
                                  //otherwise return a number 
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
  int allOne = ~0;               //0xffffffff
  highbit = allOne << highbit;   
  highbit = ~ (highbit << 1);    //if high bit == 31, move left one more should do seperatly 
  lowbit = allOne << lowbit;
  return highbit & lowbit;       //the ones that need to stay will be overlapped and return one,
				 //otherwise return zero 

}
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  int mask = 0xff;
  int shiftN = n << 3;            // n * 8
  mask = ~ (mask << shiftN);      //let the place needed to be replaced be 0
  x = (x & mask) + (c << shiftN); //clean the data in that place and replace c in there	
  return x;
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
  int retVal;			   
  retVal = x ^ (x >> 16);     //fold the number in half, and xor front half and back half
			      //if there are two one or zero on in same place, it will return 0 means even
			      //if there are one zero and one one in same place, it will return 1 means odd
  retVal = retVal ^ (retVal >> 8); //redo the fold
  retVal = retVal ^ (retVal >> 4);
  retVal = retVal ^ (retVal >> 2);
  retVal = retVal ^ (retVal >> 1);  

  return retVal & 1;          //the last bit is 1 means there are odd zeros and 0 means even zeros 
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 0x8 << 28;          //min twos complement int is 0x80000000
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
  
  return (x >> 31) & 1;      //one singed int is negtive if its sign bit == 1
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  int result = x + y;                       
  int retVal = (x ^ result) & (y ^ result); //only if the bits ofi both (x and x+y), (y and x + y)
					    //are different, retVal get a return value 1 
					    //x  y  result(sign bit)  addOK
					    //0  0    0	                1 
					    //0  0    1                 0
					    //0  1    0		   	1 
					    //0  1    1                 1
					    //1  0    0                 1
					    //1  0    1                 1
					    //1  1    0                 0
					    //1  1    1                 1
  retVal = retVal >> 31;		    //we only need the sign bit	
  return !retVal;
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int mask = x >> 31;            //mask == 0x0 if sign bit == 0 or mask == 0xffffffff if sign bit == 1
  x = (x + mask) ^ mask;         //if mask == 0, x + mask = x, x ^ mask = x, because x is positive
				 //if mask == 1, x + mask = x - 1, x ^ mask = ~ x, which is invert of twos complement
  return x;
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
  if ( (uf & 0x7fffffff) > 0x7f800000)
    return uf; 	                //if exp == 0xff and frac != 0, this floatpoint value is NaN, return itself   
  return uf ^ 0x80000000;       //let the sign bit = ~ sign bit 
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  unsigned exp, sign, round;
  if ( (uf & 0x7fffffff) >= 0x7f800000)
    return uf; 					//NaN return itself
  exp = uf & 0x7f800000;			
  sign = uf  & 0x80000000;			//get exp and sign
  round = !((uf & 0x3) ^ 0x3);			//if the last two bit of frac is both 1, then round 1
  if ( exp <= 0x800000)
    return sign | (((uf & 0x7ffffff) >> 1) + round); //if exp == 1, exp = 0, frac = frac >> 1 + 0x400000 + round
						     //if exp == 0, exp = 0, frac = frac >> 1 + round	
  return uf - 0x800000;				//normal case, exp = exp - 1

}
/*
unsigned float_half(unsigned uf) {
	unsigned exp, sign, round;
	exp = uf & 0x7f800000;
	sign = uf & 0x80000000;			//get exp and sign
	if (exp == 0x7f800000)
		return uf; 					//NaN return itself
	round = ((uf & 0x3) == 0x3);			//if the last two bit of frac is both 1, then round 1
	if (exp <= 0x800000)
		return sign | (((uf & 0x7ffffff) >> 1) + round); //if exp == 1, exp = 0, frac = frac >> 1 + 0x400000 + round
														 //if exp == 0, exp = 0, frac = frac >> 1 + round	
	return uf - 0x800000;				//normal case, exp = exp - 1

}*/