// lab.cpp (float_lab.cpp)
// COSC130
// Dong Jun Woun
// 02/14/2021
#include <cstdio>

// added a function integerAddition (adds two numbers)
// other than that copied from canvas
struct Real
{
	//sign bit
	int sign;
	//UNBIASED exponent
	long exponent;
	//Fraction including implied 1. at bit index 23
	unsigned long fraction;
};
Real Decode(int float_value);
int Encode(Real real_value);
Real Normalize(Real value);
Real Multiply(Real left, Real right);
Real Add(Real left, Real right);
long integerAddition(long left, long right);
int main(int argc, char* argv[])
{
	int left, right;
	char op;
	int value; Real rLeft, rRight, result; if (argc < 4) {
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	} sscanf(argv[1], "%f", (float*)&left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%f", (float*)&right); rLeft = Decode(left);
	rRight = Decode(right); if (op == 'x') {
		result = Multiply(rLeft, rRight);
	}
	else if (op == '+') {
		result = Add(rLeft, rRight);
	}
	else {
		printf("Unknown operator '%c'\n", op);
		return -2;
	}
	value = Encode(result);
	printf("%.3f %c %.3f = %.3f (0x%08x)\n",
		*((float*)&left),
		op,
		*((float*)&right),
		*((float*)&value),
		value
	); return 0;
}

Real Decode(int float_value)
{
	// decode takes a 32-bit number and converts into a Real structure
   // split into sign, fraction, and exponent
   // shifted each piece 31, 0 , and 23 bits and masked
	Real* Decode = new Real;

	// shift 31 bits and mask 111_1111_1111_1111_1111_1111
	int k = float_value >> 31;
	k = k & 1;
	Decode->sign = k;
	// shift 0 bits and mask 1
	int a = float_value;
	a = a & 8388607;
	Decode->fraction = a;

	// shift 23 bits and mask 1111_1111
	int j = float_value >> 23;
	j = j & 255;
	Decode->exponent = j;

	// return Real structure
	return *Decode;
}

int Encode(Real real_value)
{
	// encodes takes a Real structure and convert it back to a 32-bit number in IEEE-754 format
 // shifts each Real structure to its appropriate bit and uses or to put it into a 32 bit number
	int j = 0;
	j = j | (real_value.sign << 31) | (real_value.fraction) | (real_value.exponent << 23);

	return j;

}

Real Normalize(Real value)
{
	// normalize will manipulate the exponent and fraction so the fraction is 1.<fraction>
	Real* Normalize = new Real[0];

	// saves the information into a Real structure of Normalize[0]
	Normalize[0].sign = value.sign;
	Normalize[0].exponent = value.exponent;
	Normalize[0].fraction = value.fraction;

	// if the bit is 0 at the shifted location, the function is not in 1.<fraction>
	if ((Normalize[0].fraction >> 23) == 0) {

		for (int i = 23; i >= 0; i--)
		{
			if (1 == ((value.fraction >> i) & 1))
			{
				// shifts the the most significant big to the left and subtracts 1 from the exponent
				// after 23 (maximum) shifts the fraction should be in 1.<fraction>
				Normalize[0].fraction = Normalize[0].fraction << 1;
				Normalize[0].exponent = integerAddition(Normalize[0].exponent, -1);
        
        // back to 23 and counts down to the bit with 1
        // when  (Normalize[0].fraction >> 23)==1 the loop stops
        i = 23;
				// if the bit is 1 at the shifted location, the function is  in 1.<fraction>
				// so break
				if (((Normalize[0].fraction >> 23) & 1) == 1) {
					break;
				}
			}
		}
	}
	else if ((Normalize[0].fraction >> 23) > 0) {
		// if the fraction is greater than 0 at the shifted location, the function is not normalized
   // we already know the the first bit at the shifted location is 0, 
   // so if the fraction is greater 0 after the shift, it has to be 2 or more
		for (int i = 63; i >= 24; i--)
		{
			if (1 == ((value.fraction >> i) & 1))
			{
				// starts at bit 63 and sees if the bit is 0 or 1
				// if it's 1 the fraction is shifted to right and adds 1 from the exponent
				// by i=24, the function should have shifted all 1 bits that are not in the form 1.<fraction>
				Normalize[0].fraction = Normalize[0].fraction >> 1;

				Normalize[0].exponent = integerAddition(Normalize[0].exponent, 1);
			}
		}
	}

	// deletes the 24th bit as it is not needed
	if (1 == ((Normalize[0].fraction >> 23) & 1))
	{
		Normalize[0].fraction = Normalize[0].fraction & ~(1 << 23);
	}

	return Normalize[0];
}
Real Multiply(Real left, Real right)
{
	Real* Multiply = new Real[0];

	// adds 1 to the fraction so it becomes 1.<fraction>
	int y = 1;
	right.fraction = right.fraction | (y << 23);
	left.fraction = left.fraction | (y << 23);

	// subtracts 127 from each and sum , after add 127
	Multiply[0].exponent = ((left.exponent - 127) + (right.exponent - 127)) + 127;

	// if one side is negative, the reslut will be negative
	// if both sides are negative/positive, the result will be positive
	if ((left.sign == 0 && left.sign == 0) || (left.sign == 1 && left.sign == 1)) {
		Multiply[0].sign = 0;
	}
	if ((left.sign == 0 && right.sign == 1) || (left.sign == 1 && right.sign == 0)) {
		Multiply[0].sign = 1;
	}

	// temporary will store the fraction x ith bit
	long temporary = 0;
	for (int i = 23; i >= 0; i--) {
		// The result of fraction x ith bit is added
		// if ith bit is 0, temporary is 0, so 0 is added
		// if ith bit is 1, temporary is  fraction x ith bit
		// when the multiplier shifted is 0, the loop will break.
		Multiply[0].fraction = integerAddition(Multiply[0].fraction, temporary);
		if (1 == ((right.fraction >> i) & 1)) {
			temporary = (left.fraction >> (23 - i));
		}
		else if (0 == ((right.fraction >> i))) {
			break;
		}
		else if (0 == ((right.fraction) & 1)) {
			temporary = 0;
		}
	}
	// normalize
	Multiply[0] = Normalize(Multiply[0]);

	return Multiply[0];
}
Real Add(Real left, Real right)
{
	Real* Add = new Real[0];

	// adds 1 to the fraction so it becomes 1.<fraction>
	int y = 1;
	right.fraction = right.fraction | (y << 23);
	left.fraction = left.fraction | (y << 23);

	// make sure left.exponent = right.exponent
	while (left.exponent > right.exponent)
	{
		// if left is larger exponent of right.exponent +1
		// the fraction gets shifted right 1 as the exponent decreased 1
		right.exponent = integerAddition(right.exponent, 1);
		right.fraction = (right.fraction >> 1);
	}

	while (left.exponent < right.exponent)
	{
		// if right is larger exponent of left.exponent +1
		// the fraction gets shifted right 1 as the exponent decreased 1
		left.exponent = integerAddition(left.exponent, 1);
		left.fraction = (left.fraction >> 1);
	}



	if ((left.sign == 0) && (right.sign == 0)) {
		// if the two numbers are positive
	 // the sign is set to 0, the two fractions can be simply added
	 // the exponent can be equal to any left or right as they are equal at this point
		Add[0].fraction = integerAddition(left.fraction, right.fraction);
		Add[0].sign = 0;
		Add[0].exponent = left.exponent;
	}
	if ((left.sign == 0) && (right.sign == 1)) {
		// if the the right is negative and left is positive
	 // the exponent can be equal to any left or right as they are equal at this point
	 // if the two fractions are equal the exponent is set to 0 ( the result is 0)
		Add[0].exponent = left.exponent;
		if ((right.fraction == left.fraction)) {
			Add[0].exponent = 0;
		}

		// two's complement
		right.fraction = ~right.fraction;
		// add 1
		right.fraction = integerAddition(right.fraction, 1);
		// they can be added now as right side is in two's complement
		Add[0].fraction = integerAddition(left.fraction, right.fraction);
		// positive, nothing else has to be done



		if (0 == ((Add[0].fraction >> 63) & 1))
		{
			Add[0].sign = 0;
		}
		else if (1 == ((Add[0].fraction >> 63) & 1))
		{
			// negative
		// flipped bit
		// add one	
			Add[0].sign = 1;
			Add[0].fraction = ~Add[0].fraction;
			Add[0].fraction = integerAddition(Add[0].fraction, 1);
     
		}
	}
	else if ((left.sign == 1) && (right.sign == 0)) {
		// if the the left is negative and right is positive
		// the exponent can be equal to any left or right as they are equal at this point
		// if the two fractions are equal the exponent is set to 0 ( the result is 0)
		Add[0].exponent = left.exponent;
		if ((right.fraction == left.fraction)) {
			Add[0].exponent = 0;
		}

		// two's complement
		left.fraction = ~left.fraction;
		// add 1
		left.fraction = integerAddition(left.fraction, 1);
		// they can be added now as right side is in two's complement
		Add[0].fraction = integerAddition(left.fraction, right.fraction);

		// positive, nothing else has to be done
		if (0 == ((Add[0].fraction >> 63) & 1))
		{
			Add[0].sign = 0;
		}
		else if (1 == ((Add[0].fraction >> 63) & 1))
		{
			// negative
		// flipped bit
		// add one	
			Add[0].sign = 1;
			Add[0].fraction = ~Add[0].fraction;
			Add[0].fraction = integerAddition(Add[0].fraction, 1);
		}
	}
	else if ((left.sign == 1) && (right.sign == 1)) {
		// negative
	  // flipped bit
	  // add one
		left.fraction = ~left.fraction;
		right.fraction = ~right.fraction;

		left.fraction = integerAddition(left.fraction, 1);
		right.fraction = integerAddition(right.fraction, 1);

		// add the fraction
		// the exponents can be added at this point
		Add[0].fraction = integerAddition(left.fraction, right.fraction);
		Add[0].exponent = left.exponent;

		// two negatives numbers added should end with a negative
		// the fraction should be flipped (it's in two's complement)
		// add one
		Add[0].sign = 1;
		Add[0].fraction = ~Add[0].fraction;
		Add[0].fraction = integerAddition(Add[0].fraction, 1);
	}
	
	// normalize
	Add[0] = Normalize(Add[0]);

	return Add[0];
}

// function that adds two numbers
long integerAddition(long left, long right) {
	long a = left;
	long b = right;


	while (b != 0)
	{
		// algorithm found online
		// example (3, 128)
		// 0000_0011 = a
		// 1000_0000 = b
		// 0000_0000 = Carrier a&b

		// 1000_0011 = a = a^b
		// 0000_0000 = b = Carrier << 1;

		// a is the sum
		long Carrier = a & b;
		a = a ^ b;
		b = Carrier << 1;
	}
	return  a;
}


