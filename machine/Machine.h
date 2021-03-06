#ifndef MIX_MACHINE_MACHINE_H
#define MIX_MACHINE_MACHINE_H

#include "Basic_word.h"
#include "Field_spec.h"
#include "Instruction.h"
#include "Op_code.h"
#include "Sign.h"
#include "Word.h"
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace mix
{
	// Mix machine.
	class Machine
	{
	public:
		// A bit is either on or off.
		enum class Bit : Byte { On, Off };

		// Comparison values
		enum class Comparison_value : Byte { Equal, Greater, Less };

		// Constants.
		static const unsigned int mem_size;
		static const unsigned int num_index_registers;


		// Constructors and destructor.
		Machine();
		Machine(const Machine&) = delete;
		Machine(Machine&&) = delete;


		/* Functions. */

		// Running the machine.
		void start(std::vector<std::string>&);
		void load_program(std::istream*);
		void run_program();
		void execute_next_instruction();
		int read_address(const Word&) const;
		void dump_memory(std::ostream*) const;
		Instruction decode(const Word&) const;

		// Executing instructions.
		const Word memory_content(int, const Field_spec&) const;


		// Accessors.
		int program_counter() const { return pc; }
		Bit overflow_bit() const { return overflow; }
		Comparison_value comparison_indicator() const { return compare; }
		Half_word jump_register() const { return jump; }
		Word accumulator() const { return accum; }
		Word extension_register() const { return exten; }
		Half_word index_register(int) const;
		Word memory_cell(int) const;

		// Mutators.
		void overflow_bit(Bit);
		void jump_register(const Half_word&);
		void accumulator(const Word&);
		void extension_register(const Word&);
		void index_register(int, const Half_word&);
		void memory_cell(int, const Word&);


	private:
		// Program counter.
		int pc;

		// Operation result flags.
		Bit overflow;
		Comparison_value compare;

		// Registers.
		Half_word jump;
		Word accum;
		Word exten;
		std::vector<Half_word> index;

		// Memory.
		std::vector<Word> memory;

		// End of program flag.
		bool program_finished;

		// Validations.
		void check_arguments(const std::vector<std::string>&) const;
		void check_program_input_stream(std::istream*) const;
		void check_index_register_number(int) const;
		void check_memory_cell_address(int) const;
	};
}
#endif

