//============================================================================
// Name        : bitmanip.cpp
// Author      : user
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bitset>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Primitive_Cell.hpp"

/* Debugging */

//#include "bitfield.hpp"
//#define NOT_CRAP

#ifdef NOT_CRAP

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
#endif

#ifndef NOT_CRAP
/* Debugging */
//#define DEBUG_CELL_POSITION // Print numerical position of each live cell relative to one being constructed.
//#define DEBUG_CELL_CREATION // Print Cell, Neighbor Count, Present Life Status.
//#define DEBUG_CELL_FUTURE // Print Cell Expected Future Outcome

void print_int_array(const std::vector<int> &dat);

int main() {

	/* Testing Routines */
	std::vector<int> dataset;
	dataset.reserve(3);
	int num_neighbors, is_live, outcome = 0, mask = 0, is_set = 0, jj = 0;

	// Generate Results Mask [WORKS]
	for(auto ii = 0; ii < (INT_BIT_LENGTH/3 - 1); ++ii){
		mask <<=3; // Pre-Shifted Data
		mask |= 0x02;
	}

for(auto kk = 0; kk < 1024*1024*32; ++kk){

	// Generate Test Data & Results
	// Procedural Data Extends << of LSB, -1 used to correct for this.
	outcome = 0; // Ensure outcome is reset each iteration
	for(auto ii = 0; ii < (INT_BIT_LENGTH/3 - 1); ++ii){ // Loops over number of Unique cells supported by the data type.
		/* Pre-Shift Data (also ensured zero-valued data) */
		dataset[0] <<= 3;
		dataset[1] <<= 3;
		dataset[2] <<= 3;

		is_live = (rand() % 2) << 1; // Cell Live; Shifted to bit 1.
		num_neighbors = rand() % 9; // Between 0...8 Neighbors

		/* Expected Results */
		outcome <<= 3; // Pre-Shift Expected Outcome
		if(num_neighbors < 2){outcome &= ~0x02;} // RULE 1
		if((num_neighbors > 1) & (num_neighbors < 4)){outcome |= is_live;} // RULE 2
		if(num_neighbors > 4){outcome &= ~is_live;} // RULE 3
		if(num_neighbors == 3){outcome |= 0x02;} // RULE 4

		/* DEBUGGING */
#ifdef DEBUG_CELL_CREATION
		std::cout << "\nCell is Live: " << is_live << std::endl;
		std::cout << "Neighbor Count: " << num_neighbors << std::endl;
#endif

		/* Generate Test Set */
		jj = 0; is_set = 0; // Iterator and Mask
		if(is_live){dataset[1] |= 0x02;}
#ifdef DEBUG_CELL_POSITION
		std::cout << "Positions:";
#endif
		while(jj < num_neighbors){ // [BROKEN]
			int pos = (rand() % 8) + 1;
			if((pos == 1) && ((is_set & 0x01) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 1";
				#endif
				is_set |= 0x01;
				jj++;
				dataset[0] |= 0x04;
			} else if((pos == 2) && ((is_set & 0x02) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 2";
				#endif
				is_set |= 0x02;
				jj++;
				dataset[0] |= 0x02;
			} else if((pos == 3) && ((is_set & 0x04) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 3";
				#endif
				is_set |= 0x04;
				jj++;
				dataset[0] |= 0x01;
			} else if((pos == 4) && ((is_set & 0x08) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 4";
				#endif
				is_set |= 0x08;
				jj++;
				dataset[1] |= 0x04;
			} else if((pos == 5) && ((is_set & 0x10) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 5";
				#endif
				is_set |= 0x10;
				jj++;
				dataset[1] |= 0x01;
			} else if((pos == 6) && ((is_set & 0x20) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 6";
				#endif
				is_set |= 0x20;
				jj++;
				dataset[2] |= 0x04;
			} else if((pos == 7) && ((is_set & 0x40) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 7";
				#endif
				is_set |= 0x40;
				jj++;
				dataset[2] |= 0x02;
			} else if((pos == 8) && ((is_set & 0x80) == 0)){
				#ifdef DEBUG_CELL_POSITION
					std::cout << " 8";
				#endif
				is_set |= 0x80;
				jj++;
				dataset[2] |= 0x01;
			}
		}
#ifdef DEBUG_CELL_POSITION
		std::cout << "." << std::endl;;
#endif

#ifdef DEBUG_CELL_CREATION
		print_bin_line(dataset[0],"N-1");
		print_bin_line(dataset[1],"N");
		print_bin_line(dataset[2],"N+1");
#endif

#ifdef DEBUG_CELL_FUTURE
		print_bin_line(outcome,"Future");
#endif

	}


	/*
	 * Counting Neighbors
	 * Reduce 3x3 spatial grid of bits to simplified count of values
	 */
	auto ln = 1;

	// Sum Column (Binary Adder in Software)
	unsigned int twos_col = (dataset[ln-1] & (dataset[ln] | dataset[ln+1])) | (dataset[ln] & dataset[ln+1]);
	unsigned int ones_col = dataset[ln-1] ^ dataset[ln] ^ dataset[ln+1];

	// Copy and Shift Columns to consider left and right neighbors
	// WARNING: Zero-padding will occur.
	unsigned int twos_col_ls = twos_col << 1;
	unsigned int twos_col_rs = twos_col >> 1;
	unsigned int ones_col_ls = ones_col << 1;
	unsigned int ones_col_rs = ones_col >> 1;

	// Block Sum the Ones
	unsigned int twos_ones = (ones_col_ls & (ones_col | ones_col_rs)) | (ones_col & ones_col_rs);
	unsigned int ones_block = ones_col_ls ^ ones_col ^ ones_col_rs;

	// Complete the Sum
	unsigned int twos_block = (~(twos_col_ls | twos_col_rs) & (twos_col ^ twos_ones)) | (~(twos_col | twos_ones) & (twos_col_ls ^ twos_col_rs));
	unsigned int fours_block = (~(twos_col_ls ^ twos_col_rs ^ twos_col ^ twos_ones) /* XNOR */
					 & ~(twos_col_ls & twos_col_rs & twos_col & twos_ones) /* NAND */
					 & (twos_col_ls | twos_col_rs | twos_col | twos_ones) /* NAND(~A,~B,~C,~D)*/);

	// Conway's Rules
	unsigned int conway = (fours_block & dataset[ln] & ~(twos_block | ones_block)) | ((twos_block & ones_block) & ~(fours_block));

#ifdef DEBUG_COMPUTATION_STACK
	std::cout << "--- [0] Input Data ---" << std::endl;
	print_bin_line(dataset[ln-1],"Line (N-1)");
	print_bin_line(dataset[ln],"Line (N)");
	print_bin_line(dataset[ln+1],"Line (N+1)");
	std::cout << "--- [1] Column Sum ---" << std::endl;
	print_bin_line(twos_col,"2's");
	print_bin_line(ones_col,"1's");
	std::cout << "--- Column Shift ---" << std::endl;
	print_bin_line(twos_col_ls,"(2's)<<1");
	print_bin_line(twos_col,"(2's)");
	print_bin_line(twos_col_rs,"(2's)>>1");
	print_bin_line(ones_col_ls,"(1's)<<1");
	print_bin_line(ones_col,"(1's)");
	print_bin_line(ones_col_rs,"(1's)>>1");
	std::cout << "--- [2] Sum 1's ---" << std::endl;
	print_bin_line(twos_col_ls,"(2's)<<1");
	print_bin_line(twos_col,"(2's)");
	print_bin_line(twos_col_rs,"(2's)>>1");
	print_bin_line(twos_ones,"(2's)(1's)");
	print_bin_line(ones_block,"(1's)");
	std::cout << "--- [3] Completed Sum ---" << std::endl;
	print_bin_line(fours_block,"Exactly 1: 4");
	print_bin_line(twos_block,"Exactly 1: 2");
	print_bin_line(ones_block,"Exactly 1: 1");
	std::cout << "--- [4] Conway's Outcome ---" << std::endl;
	print_bin_line(conway,"Line (N)(t+1)");
#endif

	if((conway & mask) != outcome){
		std::cout << "\nERROR" << std::endl;
		print_bin_line(dataset[ln-1],"Line (N-1)");
		print_bin_line(dataset[ln],"Line (N)");
		print_bin_line(dataset[ln+1],"Line (N+1)");
		std::cout << "--- [5] Expected Outcome ---" << std::endl;
		print_bin_line(conway,"Conway");
		print_bin_line(conway & mask,"Masked Output");
		print_bin_line(outcome,"Expected");

	} else {
//		std::cout << "SUCCESS" << std::endl;
	}
}
	return 0;
}

void print_int_array(const std::vector<int> &dat){
	for(unsigned int ii = 0; ii < dat.capacity(); ++ii){
		std::cout << std::bitset<INT_BIT_LENGTH>(dat[ii]) << " : Line " << ii << std::endl;
	}
}

#endif
