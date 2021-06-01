// The program reads in a textfile and structures a MUD game based on the text.
#include <iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<limits>
using namespace std;
class Move {
public:
	// initializer list for Move Variable
	Move()
		: move("none") {}

	void MoveYeaorNay(string move);
	void MoveNay(string move);
private:
	// private member variable
	string move;
};

struct rooms_t
{
	// number of Rooms 
	int numRooms = 0;
};

struct direction {
	// direction and exit room #
	string exit1;
	int exit1num;
	string exit1dir;

	string exit2;
	int exit2num;
	string exit2dir;

	string exit3;
	int exit3num;
	string exit3dir;

	string exit4;
	int exit4num;
	string exit4dir;
};

struct description {
	// description of rooms
	string descript1;
	string descript2;
	string descript3;
	string descript4;

};
struct roomInfo_t
{
	// room # and nested structs for description and direction
	string room;
	description descript;
	direction direct;

};

int main(int argc, char** argv)
{
	rooms_t p;
	ifstream fin;
	string word;
	// Makes sure there is an argument given
	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << "[inputfile] " << endl;
		return 1;
	}

	// if the program fails to open the file, the program exits
	fin.open(argv[1]);
	if (!fin) {
		perror(argv[1]);
		return 1;
	}


	// using while to go through textfile
	while (getline(fin, word))
	{

		if (word.size() > 0) {
			// counts every tidle
			if (word == "~") {
				++p.numRooms;
			}
		}
	}
	// clear seekg to reuse getline
	fin.clear();
	fin.seekg(0);

	// calculates # of rooms
	p.numRooms /= 3;

	// initializes the number of rooms based on calculation from above
	roomInfo_t* theRooms = new roomInfo_t[p.numRooms];

	// count will count the number of times tidles comes out in a room and resets to 0 when 3 tidles are counted
	// i counts the room number 
	int count = 0;
	int  i = 0;
	while (getline(fin, word))
	{
		if (word == "~") {
			count++;
			//	cout << count << endl;
			if (count == 3) {
				// 3 tidle = new roo 
				count = 0;
				i++;
			}
			continue;
		}
		if (count == 0) {
			theRooms[i].room = word;
			// count is 0(before any ~ the room # is alloacted
		}
		if (count == 1) {
			// at count 1, description of the room is allocated
			if (theRooms[i].descript.descript1.size() == 0)
			{
				// if theRooms[i].descript.descript1 is not empty, it will be filled with a line of description
				theRooms[i].descript.descript1 = word;
			}
			else if (theRooms[i].descript.descript2.size() == 0) {
				
				theRooms[i].descript.descript2 = word;
			}
			else if (theRooms[i].descript.descript3.size() == 0) {
				
				theRooms[i].descript.descript3 = word;
			}
			else if (theRooms[i].descript.descript4.size() == 0) {
				
				theRooms[i].descript.descript4 = word;
			}
		}

		if (count == 2) {
			// at count 2 , direction and room numbers are allocated
			if (theRooms[i].direct.exit1.size() == 0)
			{
				//	 if theRooms[i].direct.exit1 is not empty, it will be filled with a line of directions
				// used stringstream to separate the exit direction and exit room number
				theRooms[i].direct.exit1 = word;

				istringstream ss;

				ss.str(theRooms[i].direct.exit1);

				ss >> theRooms[i].direct.exit1dir >> theRooms[i].direct.exit1num;
			}
			else if (theRooms[i].direct.exit2.size() == 0) {
				
				theRooms[i].direct.exit2 = word;

				istringstream ss;

				ss.str(theRooms[i].direct.exit2);

				ss >> theRooms[i].direct.exit2dir >> theRooms[i].direct.exit2num;


			}
			else if (theRooms[i].direct.exit3.size() == 0) {
				
				theRooms[i].direct.exit3 = word;

				istringstream ss;

				ss.str(theRooms[i].direct.exit3);

				ss >> theRooms[i].direct.exit3dir >> theRooms[i].direct.exit3num;
			}
			else if (theRooms[i].direct.exit4.size() == 0) {
				
				theRooms[i].direct.exit4 = word;

				istringstream ss;

				ss.str(theRooms[i].direct.exit4);

				ss >> theRooms[i].direct.exit4dir >> theRooms[i].direct.exit4num;
			}
		}



	}


	// int r is the room number( when r = 0 room #0 )
	//move will hold the user input, Move m is a constructor
	Move m;
	string move;
	int r = 0;
	do {

		while ((cin >> move)) {
			// takes in the direction keys , quit, and look key


			if (!((move == "n") || (move == "e") || (move == "w") || (move == "s") || (move == "q") || (move == "l"))) {
				if ((cin.eof())) {
					break;
				}
				cout << "Wrong Key" << endl;
				// error checking for any other key 
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;

			}

			if (move == "l") {


				cout << theRooms[r].room << endl;
				if (theRooms[r].descript.descript1.size() > 0)
				{
					// the descriptions are outputted if they exist
					cout << theRooms[r].descript.descript1 << endl;
				}
				if (theRooms[r].descript.descript2.size() > 0) {
					
					cout << theRooms[r].descript.descript2 << endl;
				}
				if (theRooms[r].descript.descript3.size() > 0) {
					
					cout << theRooms[r].descript.descript3 << endl;
				}
				if (theRooms[r].descript.descript4.size() > 0) {
					
					cout << theRooms[r].descript.descript4 << endl;
				}
				cout << endl;

				if (theRooms[r].direct.exit1.size() > 0)
				{
					// the directions are outputted if they exist
					cout << "Exits: " << endl << theRooms[r].direct.exit1dir << endl;

				}
				if (theRooms[r].direct.exit2.size() > 0) {
					
					cout << theRooms[r].direct.exit2dir << endl;
				}
				if (theRooms[r].direct.exit3.size() > 0) {
					
					cout << theRooms[r].direct.exit3dir << endl;
				}
				if (theRooms[r].direct.exit4.size() > 0) {
					
					cout << theRooms[r].direct.exit4dir << endl;
				}

			}

			// depending on the direction the user moved, r is set to the new room that is entered
			if (move == theRooms[r].direct.exit1dir) {
				r = theRooms[r].direct.exit1num;
				m.MoveYeaorNay(move);
				break;
			}
			else if (move == theRooms[r].direct.exit2dir) {
				r = theRooms[r].direct.exit2num;
				m.MoveYeaorNay(move);
				break;
			}
			else if (move == theRooms[r].direct.exit3dir) {
				r = theRooms[r].direct.exit3num;
				m.MoveYeaorNay(move);
				break;
			}
			else if (move == theRooms[r].direct.exit4dir) {
				r = theRooms[r].direct.exit4num;
				m.MoveYeaorNay(move);
				break;
			}
			else {
				m.MoveNay(move);
			}
			if ((move == "q")) {
				break;
				// if q, the program stops
			}

		}

		if ((move == "q")) {
			break;
		}

	} while (!((cin.eof()) || (move == "q")));
	delete[] theRooms;
	// delete[]
	return 0;
}

// If the player moved towards a specific direction successfully, the program tells the player they successfully moved towards that direction
void Move::MoveYeaorNay(string move) {
	if (move == "n") {
		cout << "You moved NORTH." << endl;
	}
	else if (move == "e") {
		cout << "You moved EAST." << endl;
	}
	else if (move == "w") {
		cout << "You moved WEST." << endl;
	}
	else if (move == "s") {
		cout << "You moved SOUTH." << endl;
	}
}

// If the player moved towards a specific direction unsuccessfully, the program tells the player that they unsuccessfully moved towards that direction
void Move::MoveNay(string move) {
	if (move == "n") {
		cout << "You can't go North!" << endl;
	}
	else if (move == "e") {
		cout << "You can't go East!" << endl;
	}
	else if (move == "w") {
		cout << "You can't go West!" << endl;
	}
	else if (move == "s") {
		cout << "You can't go South!" << endl;
	}


}
