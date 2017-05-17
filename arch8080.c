typedef struct ConditionCodes {
	uint8_t z:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	uint8_t pad:3;
} ConditionCodes;

typedef struct State8080 {
	uint8_t    a;
	uint8_t    b;
	uint8_t    c;
	uint8_t    d;
	uint8_t    e;
	uint8_t    h;
	uint8_t    l;
	uint16_t    sp;
	uint16_t    pc;
	uint8_t     *memory;
	struct      ConditionCodes      cc;
	uint8_t     int_enable;
} State8080;

int parity(int x, int size)
{
	int i;
	int p = 0;
	x = (x & ((1<<size)-1));
	for (i=0; i<size; i++) {
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}