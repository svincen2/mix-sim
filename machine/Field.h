#ifndef MIX_INSTRUCTION_FIELD_H
#define MIX_INSTRUCTION_FIELD_H

#include "Byte.h"

namespace MIX
{
    namespace Instruction
    {
        /*
        * A MIX machine instruction field specification.
        * Specifies the bytes of a word to be utilized in an instruction.
        */
        struct Field
        {
            unsigned int left;
            unsigned int right;

            // Constructors.
            Field();
            Field(unsigned int, unsigned int);
            Field(const Field&);

            // Number of bytes in range specified by field.
            unsigned int range_size() { return right - left + 1; }
        };

        // Convert a number to a field specification.
        Field to_field_spec(const unsigned int);

        // Convert a field specification to a number.
        Byte to_byte(const Field&);
    }
}
#endif

