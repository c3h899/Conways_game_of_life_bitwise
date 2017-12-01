#include <bitset>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Primitive_Cell.hpp"

int main() {

	unsigned int dat[INT_BIT_LENGTH] = {0};
	dat[7] = 0x7070707070;

	dat[1] = 0x20000000;
	dat[2] = 0x10000000;
	dat[3] = 0x70000000;

	dat[INT_BIT_LENGTH-1] = 0xFFFFFFFF;

	Primitive_Cell cell0;
	cell0.set_bitfield(dat); // Copy in
	cell0.populate_shares(); // Feed Shares
	std::cout << "Before:" << std::endl;
	cell0.print_field();

	for(auto ii = 0; ii < 5; ++ii){
		std::cout << "\n After:" << std::endl;
		cell0.evaluate_cell(); // Evaluate Cell as Zero Padded
		cell0.print_field();
	}
}

