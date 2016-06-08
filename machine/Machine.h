#ifndef MIX_MACHINE_H
#define MIX_MACHINE_H

#include "Bit.h"
#include "Half_word.h"
#include "Word.h"
#include "Field.h"
#include "Comparison.h"
#include "Instruction.h"
#include <functional>
#include <vector>

namespace MIX
{
    class Machine
    {
    public:
        // Constants.
        static const unsigned int num_index_registers{6};
        static const unsigned int memory_size{4000};

        // Constructors.
        Machine();
        Machine(const Machine&) = delete;
        Machine(Machine&&) = delete;


        // Execute the instruction.
        void execute_instruction(const Word&);
        // Get the contents of memory at the given address.
        const Word contents(unsigned int, const Instruction::Field&) const;
        // Get the bytes from the given word in the field, right shifted.
        const Word get_bytes(const Word&, const Instruction::Field&) const;


        // Accessors.
        const Word& accumulator() const { return _accumulator; }
        const Word& extension() const { return _extension; }
        const Half_word& jump_register() const { return _jump; }
        const Half_word& index_register(int) const;
        const Word& memory(int) const;

        // Mutators.
        void accumulator(const Word& a) { _accumulator = a; }
        void extension(const Word& x) { _extension = x; }
        void jump_register(const Half_word& j) { _jump = j; }
        Half_word& index_register(int);
        Word& memory(int);

        // Methods to read parts of instructions.
        const int read_address(const Word&) const;
        const unsigned int read_index(const Word&) const;
        const unsigned int read_modification(const Word&) const;
        const Instruction::Field read_field(const Word&) const;
        const unsigned int read_op_code(const Word&) const;

    private:
        // Pointer to a machine operation.
        using operation = void(Machine::*)(const Word&);

        Bit _overflow;                            // Overflow bit (on/off).
        Comparison _comparison_indicator;         // Comparison flag.
        Word _accumulator;                        // Accumulator register (A).
        Word _extension;                          // Extension register (X).
        Half_word _jump;                          // Jump register (J).
        std::vector<Half_word> _index_registers;  // Index registers (I1-I6).
        std::vector<Word> _memory;                // 4000 word memory (0-3999).
        std::vector<operation> _operation_table;  // Operation dispatch table.

        // Set up the operation table.
        void setup_operation_table();

        // Machine operations.
        void lda(const Word&);
    };
}
#endif

