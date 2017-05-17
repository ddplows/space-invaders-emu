#include <stdint.h>
#include "arch8080.h"


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

int Sub8080(State8080* state)
{
	uint16_t subParam = 0, offset = 0;
	uint8_t* opcode = &state->memory[state->pc];

	switch(*opcode & 0x0F) {
	case 0x00:
		subParam = (uint16_t)state->b;
		break;
	case 0x01:
		subParam = (uint16_t)state->c;
		break;
	case 0x02:
		subParam = (uint16_t)state->d;
		break;
	case 0x03:
		subParam = (uint16_t)state->e;
		break;
	case 0x04:
		subParam = (uint16_t)state->h;
		break;
	case 0x05:
		subParam = (uint16_t)state->l;
		break;
	case 0x06:
		offset = (state->h<<8) | (state->l);
		subParam = state->memory[offset];
		break;
	case 0x07:
		subParam = (uint16_t)state->a;
		break;
	case 0x08:
		subParam = (uint16_t)state->b - (uint16_t)state->cc.cy;
		break;
	case 0x09:
		subParam = (uint16_t)state->c - (uint16_t)state->cc.cy;
		break;
	case 0x0a:
		subParam = (uint16_t)state->d - (uint16_t)state->cc.cy;
		break;
	case 0x0b:
		subParam = (uint16_t)state->e - (uint16_t)state->cc.cy;
		break;
	case 0x0c:
		subParam = (uint16_t)state->h - (uint16_t)state->cc.cy;
		break;
	case 0x0d:
		subParam = (uint16_t)state->l - (uint16_t)state->cc.cy;
		break;
	case 0x0e:
		offset = (state->h<<8) | (state->l);
		subParam = state->memory[offset] - (uint16_t)state->cc.cy;
		break;
	case 0x0f:
		subParam = (uint16_t)state->a - (uint16_t)state->cc.cy;
		break;
	}
	uint16_t answer = (uint16_t)state->a - subParam;

	//check zero flag
	if((answer & 0xff) == 0)
		state->cc.z = 1;
	else
		state->cc.z = 0;

	//check sign flag
	state->cc.s = ((answer & 0x80) != 0);
	//check carry flag
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);
	state->a = answer & 0xff;

	return 0;
}

int Move8080(State8080* state)
{
	uint8_t* opcode = &state->memory[state->pc];
	uint8_t* to, *from;
	uint8_t toCode=0, fromCode=0;
	uint16_t offset=0;
	//destination switch
	switch(*opcode/0x08) {
	case 0x08:
		to = &state->b;
		break;
	case 0x09:
		to = &state->c;
		break;
	case 0x0A:
		to = &state->d;
		break;
	case 0x0B:
		to = &state->e;
		break;
	case 0x0C:
		to = &state->h;
		break;
	case 0x0D:
		to = &state->l;
		break;
	case 0x0E:
		offset = (state->h<<8) | (state->l);
		to = &state->memory[offset];
		break;
	case 0x0F:
		to = &state->a;
		break;

	}
	//source switch
	switch(*opcode%0x08) {
	case 0x00:
		from = &state->b;
		break;
	case 0x01:
		from = &state->c;
		break;
	case 0x02:
		from = &state->d;
		break;
	case 0x03:
		from = &state->e;
		break;
	case 0x04:
		from = &state->h;
		break;
	case 0x05:
		from = &state->l;
		break;
	case 0x06:
		offset = (uint16_t)((state->h<<8) | (state->l));
		from = &state->memory[offset];
		break;
	case 0x07:
		from = &state->a;
		break;
	}

	*to = *from;
	return 0;
}

int Or8080(State8080* state)
{
	uint8_t* opcode = &state->memory[state->pc];
	uint16_t orParam = 0, offset = 0;
	switch(*opcode & 0x0F) {
	case 0x00:
		orParam = (uint16_t)state->b;
		break;
	case 0x01:
		orParam = (uint16_t)state->c;
		break;
	case 0x02:
		orParam = (uint16_t)state->d;
		break;
	case 0x03:
		orParam = (uint16_t)state->e;
		break;
	case 0x04:
		orParam = (uint16_t)state->h;
		break;
	case 0x05:
		orParam = (uint16_t)state->l;
		break;
	case 0x06:
		offset = (uint16_t)((state->h << 8) | (state->l));
		orParam = state->memory[offset];
		break;
	case 0x07:
		orParam = (uint16_t)state->a;
		break;
	}
	uint16_t answer = (uint16_t)state->a | orParam;

	//check zero flag
	if((answer & 0xff) == 0)
		state->cc.z = 1;
	else
		state->cc.z = 0;

	//check sign flag
	state->cc.s = ((answer & 0x80) != 0);
	//check carry flag
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);
	state->a = answer & 0xff;

	return 0;
}

int Compare8080(State8080* state)
{
	uint8_t* opcode = &state->memory[state->pc];
	uint16_t cmpParam = 0, offset = 0;
	switch(*opcode & 0x0F) {
	case 0x00:
		cmpParam = (uint16_t)state->b;
		break;
	case 0x01:
		cmpParam = (uint16_t)state->c;
		break;
	case 0x02:
		cmpParam = (uint16_t)state->d;
		break;
	case 0x03:
		cmpParam = (uint16_t)state->e;
		break;
	case 0x04:
		cmpParam = (uint16_t)state->h;
		break;
	case 0x05:
		cmpParam = (uint16_t)state->l;
		break;
	case 0x06:
		offset = (uint16_t)((state->h << 8) | (state->l));
		cmpParam = state->memory[offset];
		break;
	case 0x07:
		cmpParam = (uint16_t)state->a;
		break;
	}
	uint16_t answer = (uint16_t)state->a - cmpParam;

	//check zero flag
	if((answer & 0xff) == 0)
		state->cc.z = 1;
	else
		state->cc.z = 0;

	//check sign flag
	state->cc.s = ((answer & 0x80) != 0);
	//check carry flag
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);

	return 0;
}

int Add8080(State8080* state)
{
	uint16_t addParam = 0, offset = 0;
	uint8_t* opcode = &state->memory[state->pc];

	switch(*opcode & 0x0F) {
	case 0x00:
		addParam = (uint16_t)state->b;
		break;
	case 0x01:
		addParam = (uint16_t)state->c;
		break;
	case 0x02:
		addParam = (uint16_t)state->d;
		break;
	case 0x03:
		addParam = (uint16_t)state->e;
		break;
	case 0x04:
		addParam = (uint16_t)state->h;
		break;
	case 0x05:
		addParam = (uint16_t)state->l;
		break;
	case 0x06:
		offset = (state->h<<8) | (state->l);
		addParam = state->memory[offset];
		break;
	case 0x07:
		addParam = (uint16_t)state->a;
		break;
	case 0x08:
		addParam = (uint16_t)state->b + (uint16_t)state->cc.cy;
		break;
	case 0x09:
		addParam = (uint16_t)state->c + (uint16_t)state->cc.cy;
		break;
	case 0x0a:
		addParam = (uint16_t)state->d + (uint16_t)state->cc.cy;
		break;
	case 0x0b:
		addParam = (uint16_t)state->e + (uint16_t)state->cc.cy;
		break;
	case 0x0c:
		addParam = (uint16_t)state->h + (uint16_t)state->cc.cy;
		break;
	case 0x0d:
		addParam = (uint16_t)state->l + (uint16_t)state->cc.cy;
		break;
	case 0x0e:
		offset = (state->h<<8) | (state->l);
		addParam = state->memory[offset] + (uint16_t)state->cc.cy;
		break;
	case 0x0f:
		addParam = (uint16_t)state->a + (uint16_t)state->cc.cy;
		break;
	}
	uint16_t answer = (uint16_t)state->a + addParam;

	//check zero flag
	if((answer & 0xff) == 0)
		state->cc.z = 1;
	else
		state->cc.z = 0;

	//check sign flag
	state->cc.s = ((answer & 0x80) != 0);
	//check carry flag
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);
	state->a = answer & 0xff;

	return 0;
}

int And8080(State8080* state)
{
	uint16_t andParam = 0, offset = 0, answer=0;
	uint8_t* opcode = &state->memory[state->pc];

	switch((*opcode & 0x0F) % 0x08) {
	case 0x00:
		andParam = (uint16_t)state->b;
		break;
	case 0x01:
		andParam = (uint16_t)state->c;
		break;
	case 0x02:
		andParam = (uint16_t)state->d;
		break;
	case 0x03:
		andParam = (uint16_t)state->e;
		break;
	case 0x04:
		andParam = (uint16_t)state->h;
		break;
	case 0x05:
		andParam = (uint16_t)state->l;
		break;
	case 0x06:
		offset = (state->h<<8) | (state->l);
		andParam = state->memory[offset];
		break;
	case 0x07:
		andParam = (uint16_t)state->a;
		break;
	}

	if(*opcode & 0xF0 < 0x08) {
		//and
		answer = (uint16_t)state->a & andParam;
	} else {
		//XOR
		answer = (uint16_t)state->a ^ andParam;
	}

	//check zero flag
	if((answer & 0xff) == 0)
		state->cc.z = 1;
	else
		state->cc.z = 0;

	//check sign flag
	state->cc.s = ((answer & 0x80) != 0);
	//check carry flag
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);
	state->a = answer & 0xff;

	return 0;
}

int Inc8080(State8080* state)
{
	uint8_t* opcode = &state->memory[state->pc];
	uint16_t offset = 0, answer = 0;
	switch((*opcode-4) / 8 ) {
	case 0x00:
		answer = state->b + 1;
		state->b = answer & 0xff;
		break;
	case 0x01:
		answer = state->c + 1;
		state->c = answer & 0xff;
		break;
	case 0x02:
		answer = state->d + 1;
		state->d = answer & 0xff;
		break;
	case 0x03:
		answer = state->e + 1;
		state->e = answer & 0xff;
		break;
	case 0x04:
		answer = state->h + 1;
		state->h = answer & 0xff;
		break;
	case 0x05:
		answer = state->l + 1;
		state->l = answer & 0xff;
		break;
	case 0x06:
		offset = (uint16_t)(state->h << 8) | (uint16_t)(state->l);
		answer = state->memory[offset] + 1;
		state->memory[offset] = answer & 0xff;
		break;
	case 0x07:
		answer = state->a + 1;
		state->a = answer & 0xff;
		break;
	}
	state->cc.z = ((answer & 0xff) == 0);
	state->cc.s = ((answer & 0x80) != 0);
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);

	return 0;
}

int Dec8080(State8080* state)
{
	uint8_t* opcode = &state->memory[state->pc];
	uint16_t offset = 0, answer = 0;

	switch((*opcode-5) / 8 ) {
	case 0x00:
		answer = state->b - 1;
		state->b = answer & 0xff;
		break;
	case 0x01:
		answer = state->c - 1;
		state->c = answer & 0xff;
		break;
	case 0x02:
		answer = state->d - 1;
		state->d = answer & 0xff;
		break;
	case 0x03:
		answer = state->e - 1;
		state->e = answer & 0xff;
		break;
	case 0x04:
		answer = state->h - 1;
		state->h = answer & 0xff;
		break;
	case 0x05:
		answer = state->l - 1;
		state->l = answer & 0xff;
		break;
	case 0x06:
		offset = (uint16_t)(state->h << 8) | (uint16_t)(state->l);
		answer = state->memory[offset] - 1;
		state->memory[offset] = answer & 0xff;
		break;
	case 0x07:
		answer = state->a - 1;
		state->a = answer & 0xff;
		break;
	}
	state->cc.z = ((answer & 0xff) == 0);
	state->cc.s = ((answer & 0x80) != 0);
	state->cc.cy = (answer > 0xff);
	state->cc.p = parity(answer&0xff, 8);

	return 0;
}