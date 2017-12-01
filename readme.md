# Bitwise implementation of Conway's Game of Life
An exercise in bitwise logic and precursor the implementation in discrete hardware, this project implements a square bitfield as a tile-able object using the bit-length of the processor's architecture as the field's dimension. The entire width of the field is evaluated simultaneously to produce an entire line of results. Lines are scanned from top to bottom.

## Considerations of parallelism
At its core Conway's Game of Life is a simple adder with some conditional behavior. It is believed (unverified) that performance gains from a software implementation of the adder and combinatorial logic can be seen given sufficient execution width.

Further the Game of Life is an iterative state machine depending on only present state to calculate the (next) future state. This allows for entire tiles to be evaluated in parallel assuming that accesses from neighboring elements are guaranteed constant results over the course of the computations of state. This is reflected in class' maintaining copies of the data to be requested by neighboring tiles and the separation of the computation function 'evaluate_cell()' and data setting 'populate_shares()'.

Simple parallelization would:
1 evaluate all the cells
2 synchronize
3 update all shares
4 synchronize
5 repeat

The shares are accessed by neighboring tiles to provide the necessary edge values for proper computation. The principle bitfield is modified during cell evaluation and is not guaranteed to be constant during the course of it's computation.

## Hardware implementation
Commented blocks within 'evaluate_line' are suggested stages in a pipelined evaluation system. Behavior within 'evaluate_cell' provides a description of a very simple state machine.

## Notes
All positions described in the cell object are relative to the object.
