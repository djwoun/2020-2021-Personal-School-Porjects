// lab.cpp
// The program takes in a string and up to two integers for commands. 
//The user can manipulate an extremely large number of bits with the program.
// Dong Jun Woun
// 02/04/2021
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <limits>
using namespace std;
class Bitset {
	// class Bitset divided into public and private
public:
	//  accessor(const) and mutator functions
	void resizethyvector();
	void Test(int testNum) const;
	void Set(int bitNum);
	void Clear(int bitNum);
	void GetNumSets() const;
	int GetSet(unsigned int index) const;
private:
	// private member vector
	vector<unsigned int> integers{ 0 };

};
// non member function
void ToBinary(int value, int spacingNumber);


int main()
{
	// class B
	Bitset B;
	// used do function to loop
	// initialized string input to take in user input
	do {
		string input;
		while ((cout << "Type Bitset Command" << endl) && !(getline(cin, input))) {
			if ((cin.eof())) {
				//eof
				break;
			}
		}

		// initialized needed variables
		int bit = 0;
		int value = 0;
		int spacingNumber = 4;
		string operation;
		// separated user input into needed catergories
		stringstream ss;
		ss.str(input);
		ss >> operation >> bit >> spacingNumber;
		// if user typed in invalid command, the program error checks
		if (!((operation == "t") || (operation == "s") || (operation == "c") || (operation == "n") || (operation == "g") || (operation == "q"))) {
			if ((cin.eof())) {
				//eof
				break;
			}
			continue;
		}

		// depending on the command, an a bit is tested, set,or cleared etc.
		// function from constructor
		if (operation == "t") {
			B.Test(bit);
		}
		else if (operation == "s") {
			B.Set(bit);
		}
		else if (operation == "c") {
			B.Clear(bit);
		}
		else if (operation == "n") {
			B.GetNumSets();
		}
		else if (operation == "g") {
			// because the vector cannot be called on in ToBinary, the value of the number had to got first
			value = B.GetSet(bit);
			ToBinary(value, spacingNumber);
		}
		else if (operation == "q") {
			break;
		}
		//eof
	} while (!((cin.eof())));

}

// resize function
void Bitset::resizethyvector() {
	// increases the size of the vector by 1
	int i = integers.size() + 1;
	integers.resize(i);
}

void  Bitset::Test(int testNum) const {
	// variables i is the index # where the bit is tested
	unsigned int i = testNum / 32;

	// tests if the vector[index] exists or if the vector size is equal to the index tested
  if (integers.size() < i)
	{
		printf("0\n");
	}
	else if (1 == ((integers[i] >> testNum) & 1)) {
		// if it is 1 at the tested bit, 1 is printed for true. else 0 is printed for false
		printf("1\n");
	}
	else {
		printf("0\n");
	}
}
void  Bitset::Set(int bitNum) {
	// i and j needed to see if the bit that is going to be set exists
  // if not vector is resized, and loops until vector that has the Nth bit exists
	int j = integers.size();
	double dbitNum = bitNum;
	double i = dbitNum / 32;
	int index = bitNum / 32;
	while (i >= j) {
		resizethyvector();
		j = integers.size();
	}
	// bit at the location is set to 1
	integers[index] = integers[index] | (1 << bitNum);
}
void  Bitset::Clear(int bitNum) {

	// k and j needed to see if the set bit that is going to be cleared exists
	  // if so nothing happens
	int j = integers.size();
	double dbitNum = bitNum;
	double k = dbitNum / 32;
	unsigned int	i = integers.size() - 1;
	if (k < j) {
		integers[i] = integers[i] & ~(1 << bitNum);
	}


	// checks if vector is 0 at the most significant vector size
	//if the while is true, vector is resized down 1 until vector size of 1

	while (integers[i] == 0 && !(integers.size() == 1)) {
		integers.resize(i);
		i--;

	}
}

void  Bitset::GetNumSets() const {
	// number of sets printed
	cout << integers.size() << endl;
}
int  Bitset::GetSet(unsigned int index) const {
	// set exists, the set of four byte is returned
	// if else, 0 is returned 
	if (integers.size() > index) {
		return integers[index];
	}
	else
	{
		return 0;
	}

}
void ToBinary(int value, int spacingNumber) {

	// starts big => goes down (reads right to left)
	// checks if the set at that point (in binary) is 1  or 0
  // if 1 "1" is printed if not "0" is printed
  // example print "1111 1110 1010 0101 0000 0000 1111 0101"  
	int counter = 1;
	for (int i = 31; i >= 0; i--) {
		if (1 == ((value >> i) & 1)) {
			printf("1");
		}
		else {
			printf("0");
		}
		// when counter reaches the number for spacing, a space is printed
		// counter resets and counts for the next spacing
		if (counter == spacingNumber) {
			printf(" ");
			counter = 0;
		}
		counter++;
	}

	cout << endl;


}
