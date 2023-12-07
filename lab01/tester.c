#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controller.h"

#define MAYBE(x, p) (x == -1 ? '?' : ((x >> p) & 1) + '0')

unsigned int* db[4] = { &face_buttons, &left_stick, &right_stick, &triggers };
char* db_names[4] = { "face_buttons", "left_stick", "right_stick", "triggers" };
int db_map[2][4]; // [0]: code -> idx, [1]: idx -> code
int noop(void) { return 0; }

int (*getters[])(void) = {
			  get_x, get_o, get_s, get_t,
			  get_up, get_left, get_down, get_right,
			  get_r1, get_r2, get_l1, get_l2,
			  get_rx, get_ry, get_lx, get_ly, get_start, get_option, noop
};
const char* names[18] = { "x", "o", "s", "t", "up", "left", "down", "right", "r1", "r2", "l1", "l2", "rx", "ry", "lx", "ly", "start", "option" };
int widths[18] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0x7FFF, 1, 0x7FFF,  0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 1, 1 };
char map[4][32][3];

void clear_db(int v) {
  for (int i=0; i<4; i++)
    *db[i] = v;
}

int get_idx_from_name(char* name) {
  for (int i=0; i<18; i++) {
    if (strncmp(names[i], name, 2) == 0) {
      return i;
    }
  }
  return 18;
}

void direct_map_test(int offset) {
  int err = 0;
  printf("    Detected direct mapping [offset=%d]\n", offset);
  for (int idx=0; idx<18; idx++) {
    int mode=0, bit, stop=0;
    clear_db(0);
    for (; mode<4 && stop==0; mode++) {
      bit=2;
      for (; bit<32 && stop==0; bit++) {
	if (strncmp(map[mode][bit], names[idx], 2) == 0) {
	  stop=1;
	  break;
	}
      }
      if (stop) break;
    }
    bit += (mode != 0x0 ? 0 : offset);
    set_bits((widths[idx] << bit) | db_map[1][mode]);
    if (!getters[idx]()) {
      printf("Error - 'get_%s' not set after using set_bits\n", names[idx]);
      err = 1;
    }
  }
  if (!err) {
    printf("Completed SET validate successfully\n");
  }

  err = 0;
  for (int idx=0; idx<18; idx++) {
    int mode=0, bit, stop=0;
    clear_db(0xFFFFFFFF);
    for (; mode<4 && stop==0; mode++) {
      bit=2;
      for (; bit<32 && stop==0; bit++) {
	if (strncmp(map[mode][bit], names[idx], 2) == 0) {
	  stop=1;
	  break;
	}
      }
      if (stop) break;
    }
    bit += (mode != 0x0 ? 0 : offset);
    clear_bits((widths[idx] << bit) | db_map[1][mode]);
    if (getters[idx]()) {
      printf("Error - 'get_%s' not cleared after using clear_bits\n", map[mode][idx]);
      err = 1;
    }
  }
  if (!err) {
    printf("Completed CLEAR validate successfully\n");
  }
}

int get_map(int idx) {
  char change = 0;
  for (int mode=0; mode<4; mode++)
    for (int bit=1; bit<=30; bit++) {
      clear_db(0);
      *db[mode] = (0x1 << (32 - bit));
      if (getters[idx]()) {
	if (strcmp(map[mode][32 - bit], "  ") != 0)
	  printf("Error - Conflict, multiple buttons mapped to one bit '%s' & '%s'\n", map[mode][32 - bit], names[idx]);
	else {
	  change = 1;
	  strncpy(map[mode][32 - bit], names[idx], 2);
	}
      }
    }
  return change;
}

void calculate_map(void) {
  for (int i=0; i<4; i++)
    for (int j=0; j<32; j++)
      strcpy(map[i][j], "  ");

  printf("\nFinding variable mappings [0-1]...\n");
  for (int i=0; i<2; i++)
    for (int j=0; j<4; j++)
      db_map[i][j] = -1;
  
  for (int i=0; i<4; i++) {
    clear_db(0);
    set_bits(0xF0000000 | i);
    for (int j=0; j<4; j++) {
      if (*db[j]) {
	if (db_map[0][i] != -1) {
	  printf("Error - Multiple variables associated with '%d%d' - %s and %s\n",
		 i >> 1, i & 0x1, db_names[db_map[0][i]], db_names[j]);
	}
	if (db_map[1][j] != -1) {
	  printf("Error - Multiple codes associated with '%s' - %d%d and %d%d\n",
		 db_names[j], db_map[1][j] >> 1, db_map[1][j] & 1, i >> 1, i & 1);
	}
	db_map[0][i] = j;
	db_map[1][j] = i;
      }
    }
  }

  int missing = 0;
  for (int i=0; i<4; i++) {
    printf("%d%d -> %s\n", i >> 1, i & 1, db_names[db_map[0][i]]);
    if (db_map[0][i] == -1)
      missing |= 1 << i;
  }
  printf("\nFinding variable mappings [2-31]...\n");
  
  for (int i=0; i<18; i++) {
    if (get_map(i) == 0) {
      printf("Error - Could not find bit associated with 'get_%s'\n", names[i]);
    }
  }

  printf("┌");
  for (int i=0; i<31; i++) {
    printf("────┬");
  }
  printf("────┐\n");
  for (int i=0; i<4; i++) {
    for (int j=31; j>=2; j--) {
      printf("│ %2s ", map[i][j]);
    }
    printf("│  %c │  %c │\n", MAYBE(db_map[1][i], 1), MAYBE(db_map[1][i], 0));
  }
  printf("└");
  for (int i=0; i<31; i++) {
    printf("────┴");
  }
  printf("────┘\n");

  for (int i=0; i<4; i++) {
    if (db_map[1][i] == -1) {
      for (int j=0; j<4; j++) {
	if ((missing >> j) & 1) {
	  printf("Repairing missing mapping for further tests - %s -> %d%d\n", db_names[i], j >> 1, j & 1);
	  db_map[1][i] = j;
	  db_map[0][j] = i;
	  missing &= ~(1 << j);
	}
      }
    }
  }

  printf("\nValidating 'set_bits' and 'clear_bits'...\n");
  int idx, var = db_map[0][0x0];
  *db[var] = 0x0;
  for (idx=29; idx>=0; idx--)
    if (strcmp(map[var][idx], "  "))
      break;
  set_bits(0x1 << idx);

  for (int i=31; i>=0; i--) {
    if (getters[get_idx_from_name(map[var][i])]()) {
      direct_map_test(idx - i);
      printf("\n");
      return;
    }
  }
  printf("  Detected non-direct mapping - Cannot automatically determine msg syntax\n");
}

void print_controller(void) {
  printf("\n");
  printf("        L2-[%05d]                     R2-[%05d]\n", get_l2(), get_r2());
  printf("          L1-[%d]                         R1-[%d]\n\n", get_l1(), get_r1());
  printf("   ↑ [%d]      Start-[%d]         Option-[%d]      T-[%d]\n", get_up(), get_start(),get_option(),get_t());
  printf("← [%d] → [%d]                                  S-[%d] O-[%d]\n", get_left(),get_right(),get_s(),get_o());
  printf("   ↓ [%d]                                        X-[%d]\n", get_down(), get_x());
  printf("             Lx-[%05d]        Rx-[%05d]\n", get_lx(), get_rx());
  printf("             Ly-[%05d]        Ry-[%05d]\n\n\n", get_ly(), get_ry());
}

int snake_alg(int* v, char c, char b) {
  int o = 0;
  if (b == 2) {
    if (c == '0' || c == '1')
      o = '0';
  }
  else {
    if (c >= '0' && c <= '9')      o = '0';
    else if (c >= 'A' && c <= 'Z') o = 'A' - 10;
    else if (c >= 'a' && c <= 'z') o = 'a' - 10;
  }
  if (o)
    *v = (*v * b) + (c - o);
  return o != 0;
}

int get_message(void) {
  char c;
  int i=0, result = 0, base=2, max=32;

  i = snake_alg(&result, getchar(), 2);
  c = getchar();
  if (c == 'x') {
    base = 16;
    max = 8;
    result = i = 0;
  }
  else if (c == 'b') {
    result = i = 0;
  }
  else {
    i += snake_alg(&result, getchar(), 2);
  }

  while (i < max) {
    i += snake_alg(&result, getchar(), base);
  }
  while (getchar() != '\n');
  return result;
}

int main(void) {
  char m;
  int msg;

  while (1) {
    char c = 0;
    while (c != 'C' && c != 'S' && c != 'c' && c != 's' && c != 'm' && c != 'M') {
      printf("[S]et bits, [C]lear bits or calculate [M]ap? ");
      c = getchar();
      while (getchar() != '\n');
      if (c != 'C' && c != 'S' && c != 'c' && c != 's' && c != 'M' && c != 'm')
	printf("Invalid command - Must be 'c', 's' or 'm'\n\n");
    }
    if (c == 'm' || c == 'M') {
      calculate_map();
      continue;
    }
    m = (c == 'S' || c == 's' ? 0 : 1);
    printf("Enter %s 'msg' to send to system: ", (m ? "CLEAR" : "SET"));
    msg = get_message();
    if (m)
      clear_bits(msg);
    else
      set_bits(msg);
    print_controller();
  }
}
