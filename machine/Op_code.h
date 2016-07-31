#ifndef MIX_MACHINE_OP_CODE_H
#define MIX_MACHINE_OP_CODE_H

#include "Byte.h"

namespace mix
{
	// Mix assembly language op codes.
	enum Op_code : Byte
	{
		// Arithmetic instructions.
		ADD = 1,
		SUB,
		MUL,
		DIV,

		// Load operations.
		LDA = 8,
		LD1, LD2, LD3, LD4, LD5, LD6,
		LDX,

		// Load negative operations.
		LDAN,
		LD1N, LD2N, LD3N, LD4N, LD5N, LD6N,
		LDXN,

		// Store operations.
		STA,
		ST1, ST2, ST3, ST4, ST5, ST6,
		STX,
		STJ,
		STZ
	};
}
#endif

