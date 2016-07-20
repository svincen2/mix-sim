#ifndef MIX_MACHINE_MACHINE_H
#define MIX_MACHINE_MACHINE_H

#include "Basic_word.h"
#include "Field_spec.h"
#include "Op_class.h"
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
		static const unsigned int MEM_SIZE;
		static const unsigned int NUM_INDEX_REGISTERS;


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

		// Executing instructions.
		const Word memory_contents(int, const Field_spec&) const;
		void execute_load(const Word&);


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
		void store_in_memory(int, const Word&);
		void load_accumulator(const Word&);
		void load_extension_register(const Word&);
		void load_index_register(int, const Half_word&);


	private:
		int pc;
		Bit overflow;
		Comparison_value compare;
		Half_word jump;
		Word accum;
		Word exten;

		std::vector<Half_word> index;
		std::vector<Word> memory;

		// Operation aliases.
		using Operation = std::function<void(const Word&)>;
		using Op_map = std::map<Op_code, Operation>;

		// Load operations.
		Op_map load_ops;

		// Operation maps.
		std::map<Op_class, Op_map*> ops;

		// Helpers.
		void check_arguments(const std::vector<std::string>&) const;
		void check_program_input_stream(std::istream*) const;
		void check_index_register_number(int) const;
		void init_load_ops();
		void init_ops();
	};
}
#endif

