#include <stdio.h>
unsigned int face_buttons = 0;
unsigned int right_stick = 0;
unsigned int left_stick = 0;
unsigned int triggers = 0;
/*Face buttons will be represented with 00 as the last two significant bits,
 * Right stick will be 01
 * Left stick will be 10
 * Triggers will be 11
 */
/*
  In this project, you are prohibited from using ANY complex statement.
  This means you are restricted to assignment, bitwise, logical and
  arithmatic operators
 */
/*
  Read a message and SET the bits of the `face_buttons`, `right_stick`,
      `left_stick`, and `triggers` variables based on the contents of the
      message.  The specifics of the bit mapping are up to you, but the
      two least significant bits MUST be used to uniquely identify which
      variable is being modified i.e. with the message:
          0000 .... 0001 1010
      We would SET the second and third bits of the variable corresponding 
      with 0b10
 *///This will probably use the OR operator

void set_bits(unsigned int msg) {
	unsigned int mask;
	mask = (msg & ((1 << 2) -1));
	face_buttons |= ((mask == 0) * msg);
	right_stick |= ((mask == 1) * msg);	
	left_stick |= ((mask == 2) * msg);
	triggers |= ((mask == 3) * msg);
}

/*
  Read a message and CLEAR the bits of the `face_buttons`, `right_stick`,
      `left_stick`, and `triggers` variables based on the contents of the
      message.  The specifics of the bit mapping are up to you, but the
      two least significant bits MUST be used to uniquely identify which
      variable is being modified - i.e. with the message:
          0000 .... 0001 1010
      We would CLEAR the second and third bits of the variable corresponding 
      with 0b10
 *///Actually probably exclusive or
//This will probably use the bitwise and operator with the message blank where the bit needs to be turned off
void clear_bits(unsigned int msg) {
  unsigned int mask;
  mask = (msg & ((1 << 2) -1));
  face_buttons = ~face_buttons;
  face_buttons |= ((mask == 0) * msg);
  face_buttons = ~face_buttons;
  
  
  right_stick = ~right_stick;
  right_stick |= ((mask == 1) * msg);
  right_stick = ~right_stick;


  left_stick = ~left_stick;
  left_stick |= ((mask == 2) * msg);
  left_stick = ~left_stick;


  triggers = ~triggers;
  triggers |= ((mask == 3) * msg);
  triggers = ~triggers;
}

/*
  for the get_<value> functions, return just the value of the requested
  field.  I.e. `get_t` would extract the 0 or 1 of the `face_buttons`
  integer corresponding to 't' and return it.
 */

int get_x(void) {
  return ((face_buttons >> 2) & 1);
}
int get_o(void) {
	return ((face_buttons >> 3) & 1);
}
int get_s(void) {
	return ((face_buttons >> 5) & 1);

}
int get_t(void) {
	return ((face_buttons >> 4) & 1);
}
int get_up(void) {
	return ((face_buttons >> 6) & 1);
}
int get_left(void) {
	return ((face_buttons >> 7) & 1);
}
int get_down(void) {
	return ((face_buttons >> 8) & 1);

}
int get_right(void) {
	return ((face_buttons >> 9) & 1);
}
int get_r1(void) {
	return ((face_buttons >> 13) & 1);
}
int get_r2(void) {
	unsigned int mask = ((1u << (16 - 2 + 1)) - 1) << 2;
	return (triggers & mask) >> 2;
}
int get_l1(void) {
	return ((face_buttons >> 12) & 1);
}
int get_l2(void) {
	return (triggers) >> 17;
}
int get_rx(void) {
	return (right_stick) >> 17;
}
int get_ry(void) {
	unsigned int mask = ((1u << (16 - 2 + 1)) - 1) << 2;
	return (right_stick & mask) >> 2;
}
int get_lx(void) {
	return left_stick >> 17;
}
int get_ly(void) {
	unsigned int mask = ((1u << (16 - 2 + 1)) - 1) << 2;
	return (left_stick & mask) >> 2;
}
int get_start(void) {
	return ((face_buttons >> 10) & 1);
}
int get_option(void) {
	return ((face_buttons >> 11) & 1);
}
