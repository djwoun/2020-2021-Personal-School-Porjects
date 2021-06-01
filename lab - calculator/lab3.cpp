// lab3.cpp
// The program takes ./lab3 number operator number
// return result
// Dong Jun Woun
// 02/11/2021

// below is just the copy of the provided code
#include <cstdio>
int Multiply(int leftop, int rightop);
int Add(int leftop, int rightop);
int Sub(int leftop, int rightop);
int Twos(int op);
int Div(int leftop, int rightop);
int Mod(int leftop, int rightop);
int main(int argc, char* argv[])
{
    int left, right, result;
    char op;

    if (4 != argc)
    {
        printf("Usage: %s <left> <op> <right>\n", argv[0]);
        return -1;
    }
    sscanf(argv[1], "%d", &left);
    sscanf(argv[2], "%c", &op);
    sscanf(argv[3], "%d", &right);

    switch (op)
    {
    case 'm':
    case 'x':
    case '*':
        result = Multiply(left, right);
        break;
    case 'a':
    case '+':
        result = Add(left, right);
        break;
    case 's':
    case '-':
        result = Sub(left, right);
        break;
    case 'd':
    case '/':
        result = Div(left, right);
        break;
    case '%':
        result = Mod(left, right);
        break;
    default:
        result = -11111111;
        break;
    }
    printf("%d\n", result);
    return 0;
}

int Multiply(int leftop, int rightop) {
    // result stores the return value
    // newleftop and newrightop is used when number is negative
    // temporary temporarily stores if the nth bit is 0 or 1 
    int result = 0;
    int temporary = 0;
    int newleftop = 0;
    int newrightop = 0;

    if (1 == (((leftop >> 31) & 1) & ((rightop >> 31) & 1))) {
        ///both numbers two's complement
        newleftop = Twos(leftop);
        newrightop = Twos(rightop);
        for (int i = 0; i <= 31; i++) {
            // result and temporary is added, starts with 0+0
            result = Add(result, temporary);
            if (1 == ((newrightop >> i) & 1)) {
                // if the bit is 1, the left number is leftshifted and stored in temporary
                temporary = newleftop << i;
            }
            else if (0 == (newrightop >> i)) {
                // if the right number equals 0 after being shifted right i times
                // multiplication is done, so the loop breaks and returns the result;
                break;
            }
            else if (0 == ((newrightop >> i) & 1)) {
                // if the bit is 0, 0 is stored 
                temporary = 0;
            }
        }
        return result;
    }

    else if (0 == (((leftop >> 31) & 1) | ((rightop >> 31) & 1))) {
        //both number positive
        for (int i = 0; i <= 31; i++) {
            result = Add(result, temporary);
            if (1 == ((rightop >> i) & 1)) {
                temporary = leftop << i;
            }
            else if (0 == (rightop >> i)) {
                break;
            }
            else if (0 == ((rightop >> i) & 1)) {
                temporary = 0;
            }
        }
        return result;
    }

    else if (1 == (((leftop >> 31) & 1) ^ ((rightop >> 31) & 1))) {
        // one numeber is negative
        if (1 == ((leftop >> 31) & 1)) {
            // left number is negative
            newleftop = Twos(leftop);

            for (int i = 0; i <= 31; i++) {
                result = Add(result, temporary);

                if (1 == ((rightop >> i) & 1)) {
                    temporary = newleftop << i;

                }
                else if (0 == (rightop >> i)) {
                    result = Twos(result);
                    break;
                }
                else if (0 == ((rightop >> i) & 1)) {
                    temporary = 0;
                }
            }
            return result;

        }
        else if (1 == ((rightop >> 31) & 1)) {
            // right number is negative
            newrightop = Twos(rightop);
            for (int i = 0; i <= 31; i++) {
                result = Add(result, temporary);
                if (1 == ((newrightop >> i) & 1)) {
                    temporary = leftop << i;
                }
                else if (0 == (newrightop >> i)) {
                    result = Twos(result);
                    break;
                }
                else if (0 == ((newrightop >> i) & 1)) {
                    temporary = 0;
                }
            }
            return result;
        }
    }
    return 0;
}
int Add(int leftop, int rightop) {
    // result stores the return value
    // carryover and carryin is to carry the overflow
    // sum is 1 if one of the leftop, rightop, or carryin is 1
    int result = 0;
    int carryover = 0;
    int carryin = 0;
    int sum = 0;
    for (int i = 0; i <= 31; i++)
    {
        // sum is 1 if one of the leftop, rightop, or carryin is 1
        sum = ((leftop >> i) & 1) ^ ((rightop >> i) & 1) ^ (carryin & 1);
        // carryover is 1 if one of the combinations below result in 1
        // left & Right | left & carryin | right & carryin
        carryover = ((((leftop >> i) & 1) & ((rightop >> i) & 1)) ^ (((leftop >> i) & 1) & (carryin & 1)) ^ (((rightop >> i) & 1) & (carryin & 1)));
        // carryin holds the overflow value (1 or 0)
        carryin = carryover;
        // add result and sum
        result = result | ((sum << i));
    }
    return result;
}
int Sub(int leftop, int rightop) {
    // twos complement then just add the two 
    int newrightop = Twos(rightop);
    return Add(leftop, newrightop);
}
int Twos(int op) {
    // flip all the bits
    int newrightop = ~op;
    // add 1
    newrightop = Add(newrightop, 1);
    // result
    return newrightop;
}
int Div(int leftop, int rightop) {
    // looked at Dr.Marz's notes
    // Remainder will be the leftop and be the remainder
    // divisor will be the rightop and be the divisor 
    // quotient counts the number of sub happened
    long Remainder = 0;
    long Divisor = 0;
    int quotient = 0;

    // if divisor is 0 the remainder  is returned
    if (rightop == 0) { return leftop; }
    // divide by combination of negative
    if (1 == (((leftop >> 31) & 1) & ((rightop >> 31) & 1))) {
        //both numbers two's compl
        // widens the left and right value
        Remainder = Twos(leftop);
        Divisor = static_cast<long>(Twos(rightop)) << 32;
        for (int i = 31; i >= 0; i--) {
            // remainder is subtracted by the divisor
            Remainder = (Remainder << 1) - Divisor;
            // if remainder is above 0 after
            if (Remainder >= 0) {
                // set bit i to know the div result
                quotient |= 1 << i;
            }
            else {
                // if remainder is below 0 the remainder is restore
                Remainder += Divisor;
            }
        }
        return quotient;
    }
    else if (0 == (((leftop >> 31) & 1) | ((rightop >> 31) & 1))) {
        Remainder = leftop;
        Divisor = static_cast<long>(rightop) << 32;
        //both number positive
        for (int i = 31; i >= 0; i--) {
            Remainder = (Remainder << 1) - Divisor;
            if (Remainder >= 0) {
                quotient |= 1 << i;
            }
            else {
                Remainder += Divisor;
            }
        }
        return quotient;
    }
    else if (1 == (((leftop >> 31) & 1) ^ ((rightop >> 31) & 1))) {
        if (1 == ((leftop >> 31) & 1)) {
            //left is negative
            Remainder = Twos(leftop);
            Divisor = static_cast<long>(rightop) << 32;

            for (int i = 31; i >= 0; i--) {
                Remainder = (Remainder << 1) - Divisor;
                if (Remainder >= 0) {
                    quotient |= 1 << i;
                }
                else {
                    Remainder += Divisor;
                }
            }
            return quotient;
        }
        else if (1 == ((rightop >> 31) & 1)) {
            //right is negative
            Remainder = leftop;
            Divisor = static_cast<long>(Twos(rightop)) << 32;

            for (int i = 31; i >= 0; i--) {
                Remainder = (Remainder << 1) - Divisor;
                if (Remainder >= 0) {
                    quotient |= 1 << i;
                }
                else {
                    Remainder += Divisor;
                }
            }
            return quotient;
        }
    }
    return 0;
}
int Mod(int leftop, int rightop) {
// looked at Dr.Marz's notes
    // Remainder will be the leftop and be the remainder
    // divisor will be the rightop and be the divisor 
    // quotient counts the number of sub happened
    long Remainder = 0;
    long Divisor = 0;
    int quotient = 0;

 // if divisor is 0 remainder is returned
    if (rightop == 0) { return leftop; }
      // divide by combination of negative
    if (1 == (((leftop >> 31) & 1) & ((rightop >> 31) & 1))) {
         //both numbers two's compl
        // widens the left and right value
        Remainder = Twos(leftop);
        Divisor = static_cast<long>(Twos(rightop)) << 32;
        for (int i = 31; i >= 0; i--) {
        // remainder is subtracted by the divisor
            Remainder = (Remainder << 1) - Divisor;
            // if remainder is above 0 after
            if (Remainder >= 0) {
             // set bit i to know the div result
                quotient |= 1 << i; 
            }
            else {
             // if remainder is below 0 the remainder is restore
                Remainder += Divisor; 
            }
        }
        Remainder = (Remainder >> 32);
        return Remainder;
    }
    else if (0 == (((leftop >> 31) & 1) | ((rightop >> 31) & 1))) {
        Remainder = leftop;
        Divisor = static_cast<long>(rightop) << 32;
        //both number positive
        for (int i = 31; i >= 0; i--) {
            Remainder = (Remainder << 1) - Divisor;
            if (Remainder >= 0) {
                quotient |= 1 << i; 
            }
            else {
                Remainder += Divisor;
            }
        }
        Remainder = (Remainder >> 32);
        return Remainder;
    }
    else if (1 == (((leftop >> 31) & 1) ^ ((rightop >> 31) & 1))) {
        if (1 == ((leftop >> 31) & 1)) {
            Remainder = Twos(leftop);
            Divisor = static_cast<long>(rightop) << 32;
            for (int i = 31; i >= 0; i--) {
                Remainder = (Remainder << 1) - Divisor;
                if (Remainder >= 0) {
                    quotient |= 1 << i; 
                }
                else {
                    Remainder += Divisor; 
                }
            }
            Remainder = (Remainder >> 32);
            return Remainder;
        }
        else if (1 == ((rightop >> 31) & 1)) {
            Remainder = leftop;
            Divisor = static_cast<long>(Twos(rightop)) << 32;
            for (int i = 31; i >= 0; i--) {
                Remainder = (Remainder << 1) - Divisor;
                if (Remainder >= 0) {
                    quotient |= 1 << i; 
                }
                else {
                    Remainder += Divisor; 
                }
            }
            Remainder = (Remainder >> 32);
            return Remainder;
        }
    }
    return 0;

}