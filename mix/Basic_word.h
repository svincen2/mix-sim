#ifndef MIX_BASIC_WORD_H
#define MIX_BASIC_WORD_H

#include "Byte.h"
#include "Sign.h"
#include "Field.h"
#include <stdexcept>
#include <vector>

namespace MIX
{
	/*
	* Template for the basic format of MIX machine words.
	* Bytes are indexed starting at 1.
	*/
	template<unsigned int Size>
	class Basic_word
	{
	public:
		static const unsigned int num_bytes = Size;

		// Constructors.
		Basic_word();
		Basic_word(const Basic_word&);
		Basic_word(Basic_word&&) = delete;
		Basic_word(const std::vector<Byte>&);
		Basic_word(Sign, const std::vector<Byte>&);

		// Assignments.
		Basic_word& operator=(const Basic_word&);
		Basic_word& operator=(Basic_word&&) = delete;

		// Accessors.
		const Sign& sign() const { return _sign; }
		const Byte& byte(int) const;
		std::vector<Byte> get_bytes(int, int) const;

		// Mutators.
		void sign(Sign);
		Byte& byte(int);
		void set_bytes(int, int, const std::vector<Byte>&);

	private:
		Sign _sign;
		std::vector<Byte> _bytes;
	};

	/*
	* Construct a word with all bits cleared to 0,
	* and the sign set to plus (+).
	*/
	template<unsigned int Size>
	Basic_word<Size>::Basic_word() : _sign{Sign::Plus}, _bytes(num_bytes)
	{
	}

	/*
	* Copy the given word.
	*/
	template<unsigned int Size>
	Basic_word<Size>::Basic_word(const Basic_word<Size>& copy)
		: _sign{copy._sign}, _bytes{copy._bytes}
	{
	}

	/*
	* Construct a word with the sign set to plus (+),
	* and the rightmost bytes set to the corresponding given bytes.
	*/
	template<unsigned int Size>
	Basic_word<Size>::Basic_word(const std::vector<Byte>& vb)
		: _sign{Sign::Plus}, _bytes{vb}
	{
		// Fill with zeros until full.
		while(_bytes.size() < num_bytes)
			_bytes.push_back(0);
	}

	/*
	* Copy assignment.
	*/
	template<unsigned int Size>
	Basic_word<Size>& Basic_word<Size>::operator=(
			const Basic_word<Size>& wt)
	{
		_sign = wt._sign;
		_bytes = wt._bytes;
		return *this;
	}

	/*
	* Get the byte at the given index.
	* Read only.
	*/
	template<unsigned int Size>
	const Byte& Basic_word<Size>::byte(int index) const
	{
		if(index < 1 || num_bytes < index)
			throw std::invalid_argument{"Index out of bounds"};
		return _bytes[index - 1];
	}

	/*
	* Get the byte at the given index.
	* Read and write.
	*/
	template<unsigned int Size>
	Byte& Basic_word<Size>::byte(int index)
	{
		if(index < 1 || num_bytes < index)
			throw std::invalid_argument{"Index out of bounds"};
		return _bytes[index - 1];
	}

	/*
	* Return copies of the bytes in the given range: [begin, end].
	*/
	template<unsigned int Size>
	std::vector<Byte> Basic_word<Size>::get_bytes(int begin, int end) const
	{
		std::vector<Byte> vb{};
		for(int i = begin; i <= end; ++i)
			vb.push_back(byte(i));
		return vb;
	}

	/*
	* Sets the bytes in the given range [begin, end] to the bytes given.
	*/
	template<unsigned int Size>
	void Basic_word<Size>::set_bytes(int begin,
									 int end,
									 const std::vector<Byte>& vb)
	{
		if(vb.size() != (end - begin + 1))
			throw std::invalid_argument{"Wrong number of bytes"};
		for(int i = begin, j = 0; i <= end; ++i, ++j)
		{
			byte(i) = vb[j];
		}
	}

	/*
	* Set the sign to the given sign value.
	*/
	template<unsigned int Size>
	void Basic_word<Size>::sign(Sign s)
	{
		if(s != Sign::Plus && s != Sign::Minus)
			throw std::invalid_argument{"Unknown sign value"};
		_sign = s;
	}

	/*
	* Reads the field of the given word as an integer value.
	*/
	template<unsigned int Size>
	int to_int(const Basic_word<Size>& bw, const Field& field)
	{
		// Ensure word has enough bytes.
		if(Size < field.size())
			throw std::invalid_argument{"Field is too big"};
		int result{0};
		unsigned int begin = (field.left == 0 ? 1 : field.left);
		for(int i = begin; i <= field.right; ++i)
		{
			result |= (bw.byte(i) & bit_mask);
			result <<= byte_size;
		}
		if(field.left == 0 && bw.sign() == Sign::Minus)
			result = -result;
		return result;
	}

}
#endif
