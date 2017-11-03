#ifndef _H_PRIMITIVE_CELL_HPP
#define _H_PRIMITIVE_CELL_HPP

#include<iostream>
#include<string>

//#define DEBUG_COMPUTATION_STACK

void print_bin_line(const int &x, std::string s);
void print_rules_of_life();

enum Const{
	INT_BYTE_LENGTH = sizeof(int),
	INT_BIT_LENGTH = 8*Const::INT_BYTE_LENGTH,
};

class Primitive_Cell {
	/*
	 * Smallest implemented board primitive
	 * Implements INT_BIT_LENGTH x INT_BIT_LENGTH bit-field
	 * Evaluates board from top-to-bottom
	 *
	 * Intended for lock-step parallelism:
	 * Evaluations may be called concurrently.
	 * Shares may be called concurrently.
	 * Evaluations and Shares may NOT be called concurrently.
	 */
	public:
		// Printing
		void print_field();

		// Data Retrieval
		unsigned int get_left_edge(); // Returns left edge of cell's present generation
		unsigned int get_right_edge(); // Returns right edge of cell's present generation
		unsigned int get_top_row(); // Returns top row of cell's present generation
		unsigned int get_bottom_row(); // Returns bottom row of cell's present generation
		bool get_upper_left(); // Returns upper left corner of cell's present generation
		bool get_upper_right(); // Returns upper right corner of cell's present generation
		bool get_lower_left(); // Returns bottom left corner of cell's present generation
		bool get_lower_right(); // Returns bottom right corner of cell's present generation

		// Evaluations
		void populate_shares();
		void evaluate_cell(const unsigned int row_above = 0,
							unsigned int shift_right_fifo = 0,
							const unsigned int row_below = 0,
							unsigned int shift_left_fifo = 0,
							const bool upper_right = 0,
							const bool lower_right = 0,
							const bool lower_left = 0,
							const bool upper_left = 0);

		// Setting
		void set_bitfield(const unsigned int (&field)[INT_BIT_LENGTH]);

	protected:
	private:
		// Working Data
		unsigned int datafield[INT_BIT_LENGTH]; // Bit-field containing working data set
		bool volatile_fifo = 0; // Tracks FIFO presently being written to.
		bool cell_evolved[1] = {0}; // Tracks if cell change in most recent evaluation
		unsigned int generation = 0; // (Optional) Tracks age of cell in generations

		// Data Copy
		bool upper_left = 0; // Copy of upper-left corner of bit-field
		bool upper_right = 0; // Copy of upper-right corner of bit-field
		bool lower_left = 0; // Copy of lower-left corner of bit-field
		bool lower_right = 0; // Copy of lower-right corner of bit-field
		unsigned int shift_left_fifo[2] = {0}; // Copy of leftmost edge of bit-field
		unsigned int shift_right_fifo[2] = {0}; // Copy of rightmost edge of bit-field
		unsigned int top_row = 0; // Copy of topmost row of bit-field
		unsigned int bottom_row = 0; // Copy of bottommost row of bit-field

		// Methods
		void seed_fifo();
		unsigned int evaluate_line(const bool upper_left,
									const unsigned int upper,
									const bool upper_right,
									const bool center_left,
									const unsigned int center,
									const bool center_right,
									const bool lower_left,
									const unsigned int lower,
									const bool lower_right);
};

#endif
