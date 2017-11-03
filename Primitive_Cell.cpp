#include<bitset>

#include "Primitive_Cell.hpp"

/* Printing Functions */
void print_bin_line(const int &x, std::string s){
	std::cout << std::bitset<INT_BIT_LENGTH>(x) << " : " << s << std::endl;
}

void print_rules_of_life(){
	std::cout << "Rule 1: Any live cell with fewer than two live neighbours dies." << std::endl;
	std::cout << "Rule 2: Any live cell with two or three live neighbours lives." << std::endl;
	std::cout << "Rule 3: Any live cell with more than three live neighbours dies." << std::endl;
	std::cout << "Rule 4: Any dead cell with exactly three live neighbours becomes a live cell." << std::endl;
}

void Primitive_Cell::print_field(){
	for(int ii = 0; ii < INT_BIT_LENGTH; ++ii){print_bin_line(datafield[ii],"");}
}

/* Populate Data */
void Primitive_Cell::set_bitfield(const unsigned int (&field)[INT_BIT_LENGTH]){
	std::copy(std::begin(field), std::end(field), std::begin(datafield));
}

/* Evaluation of Cell */
void Primitive_Cell::evaluate_cell(const unsigned int row_above,
	unsigned int shift_right_fifo, const unsigned int row_below,
	unsigned int shift_left_fifo, const bool upper_right,
	const bool lower_right,	const bool lower_left, const bool upper_left)
{
	// Working Data
	bool UL = upper_left, CL, DL, UR = upper_right, CR, DR;
	int dataset[3];

	// Priming the Loop
	dataset[0] = row_above;
	CL = (shift_right_fifo & (0x01 << (INT_BIT_LENGTH-1))) ? (1) : (0); shift_right_fifo <<=1;
	CR = (shift_left_fifo & 0x01) ? (1) : (0); shift_right_fifo >>=1; shift_left_fifo >>=1;
	dataset[1] = datafield[0];

	// Primary Loop
	for(unsigned int ii = 1; ii < (INT_BIT_LENGTH); ++ii){

		// Read-in New Data
		dataset[2] = datafield[ii];
		DL = (shift_right_fifo & (0x01 << (INT_BIT_LENGTH-1))) ? (1) : (0); shift_right_fifo <<=1;
		DR = (shift_left_fifo & 0x01) ? (1) : (0); shift_right_fifo >>=1; shift_left_fifo >>=1;

		// Evaluate Cell
		datafield[ii-1] = evaluate_line(UL, dataset[0], UR, CL, dataset[1], CR, DL, dataset[2], DR);

		// "Move" old data
		UL = CL; CL = DL; UR = CR; CR = DR; dataset[0] = dataset[1]; dataset[1] = dataset[2];
	}

	// Employ Remaining Parameters
	DL = lower_left; DR = lower_right; dataset[2] = row_below;
	datafield[INT_BIT_LENGTH-1] = evaluate_line(UL, dataset[0], UR, CL, dataset[1], CR, DL, dataset[2], DR);
}

unsigned int Primitive_Cell::evaluate_line(const bool upper_left, const unsigned int upper, const bool upper_right,
	const bool center_left, const unsigned int center, const bool center_right, const bool lower_left,
	const unsigned int lower, const bool lower_right)
{
	// Sum Column (Binary Adder in Software)
	unsigned int twos_col = (upper & (center | lower)) | (center & lower);
	unsigned int ones_col = upper ^ center ^ lower;

	unsigned int ones_ls = (upper_right ^ center ^ lower_right) ? (1) : (0);
	unsigned int twos_ls = ((upper_right & (center_right | lower_right)) | (center_right & lower_right)) ? (1) : (0);

	unsigned int ones_rs = (upper_left ^ center_left ^ lower_left) ? (0x01 << (INT_BIT_LENGTH-1)) : (0);
	unsigned int twos_rs = ((upper_left & (center_left | lower_left)) | (center_left & lower_left)) ? (0x01 << (INT_BIT_LENGTH-1)) : (0);

	// Copy and Shift Columns to consider left and right neighbors
	unsigned int twos_col_ls = (twos_col << 1) | twos_ls;
	unsigned int twos_col_rs = (twos_col >> 1) | twos_rs;
	unsigned int ones_col_ls = (ones_col << 1) | ones_ls;
	unsigned int ones_col_rs = (ones_col >> 1) | ones_rs;

	// Block Sum the Ones
	unsigned int twos_ones = (ones_col_ls & (ones_col | ones_col_rs)) | (ones_col & ones_col_rs);
	unsigned int ones_block = ones_col_ls ^ ones_col ^ ones_col_rs;

	// Complete the Sum
	unsigned int twos_block = (~(twos_col_ls | twos_col_rs) & (twos_col ^ twos_ones)) | (~(twos_col | twos_ones) & (twos_col_ls ^ twos_col_rs));
	unsigned int fours_block = (~(twos_col_ls ^ twos_col_rs ^ twos_col ^ twos_ones) /* XNOR */
					 & ~(twos_col_ls & twos_col_rs & twos_col & twos_ones) /* NAND */
					 & (twos_col_ls | twos_col_rs | twos_col | twos_ones) /* NAND(~A,~B,~C,~D)*/);

	// Conway's Rules
	unsigned int conway = (fours_block & center & ~(twos_block | ones_block)) | ((twos_block & ones_block) & ~(fours_block));

	// Debugging Printouts
#ifdef DEBUG_COMPUTATION_STACK
	// Corners are Neglected with this reporting Engine
	std::cout << "--- [0] Input Data ---" << std::endl;
	print_bin_line(upper,"Line (N-1)");
	print_bin_line(center,"Line (N)");
	print_bin_line(lower,"Line (N+1)");
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

	return conway;
}

/* Populating Public Shares */
void Primitive_Cell::populate_shares(){
	/* WARNING : Not thread safe */
	top_row = datafield[0];
	bottom_row = datafield[INT_BIT_LENGTH-1];
	// FIFO's are not set here
	upper_left = (datafield[0] & (0x01 << (INT_BIT_LENGTH-1))) ? (1) : (0);
	upper_right = (datafield[0] & 0x01) ? (1) : (0);
	lower_left = (datafield[INT_BIT_LENGTH-1] & (0x01 << (INT_BIT_LENGTH-1))) ? (1) : (0);
	lower_right = (datafield[INT_BIT_LENGTH-1] & 0x01) ? (1) : (0);
}

void Primitive_Cell::seed_fifo(){
	unsigned int left, right;
	for(unsigned int ii = 0; ii < INT_BIT_LENGTH; ++ii){
		left <<= 1;
		right <<= 1;
		left = (datafield[ii] & (0x01 << (INT_BIT_LENGTH-1))) ? (1) : (0);
		right = (datafield[ii] & 0x01) ? (1) : (0);
	}
	shift_left_fifo[~volatile_fifo] = left;
	shift_right_fifo[~volatile_fifo] = right;
}

/* Getters :: Intended to return a copy of the data */
unsigned int Primitive_Cell::get_left_edge(){return shift_left_fifo[~volatile_fifo];}
unsigned int Primitive_Cell::get_right_edge(){return shift_right_fifo[~volatile_fifo];}
unsigned int Primitive_Cell::get_top_row(){return top_row;}
unsigned int Primitive_Cell::get_bottom_row(){return bottom_row;}
bool Primitive_Cell::get_upper_left(){return upper_left;}
bool Primitive_Cell::get_upper_right(){return upper_right;}
bool Primitive_Cell::get_lower_left(){return lower_left;}
bool Primitive_Cell::get_lower_right(){return lower_right;}
