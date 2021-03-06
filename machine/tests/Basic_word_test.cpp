#include "catch.hpp"
#include "Helpers.h"
#include "../Basic_word.h"
#include "../Field_spec.h"
#include <sstream>
#include <string>

using namespace mix;

SCENARIO("Constructing a basic word")
{
	GIVEN("Nothing")
	{
		WHEN("A basic word is constructed with no parameters")
		{
			Basic_word<5> bw{};
			THEN("The sign is Plus, and all bytes are 0")
			{
				REQUIRE(bw.sign() == Sign::Plus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
		}
	}
}

SCENARIO("Copying a basic word")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Plus, {1, 2, 3, 4, 5}};
		WHEN("The word is copied with a constructor")
		{
			Basic_word<5> copy{bw};
			THEN("All the bytes are the same")
			{
				REQUIRE(copy.sign() == bw.sign());
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("The word is copied with an assignment")
		{
			Basic_word<5> copy{};
			copy = bw;
			THEN("All the bytes are the same")
			{
				REQUIRE(copy.sign() == bw.sign());
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
	}
}

SCENARIO("Moving a basic word")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Plus, {1, 2, 3, 4, 5}};
		WHEN("The word is moved with a constructor")
		{
			Basic_word<5> moved{std::move(bw)};
			THEN("All bytes are the same")
			{
				REQUIRE(moved.sign() == Sign::Plus);
				require_bytes_are(moved, {1, 2, 3, 4, 5});
			}
		}
		WHEN("The word is moved with an assignment")
		{
			Basic_word<5> moved{};
			moved = std::move(bw);
			THEN("All bytes are the same")
			{
				REQUIRE(moved.sign() == Sign::Plus);
				require_bytes_are(moved, {1, 2, 3, 4, 5});
			}
		}
	}
}

SCENARIO("Converting basic words")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Converting to a smaller word")
		{
			Basic_word<2> smaller{bw};
			THEN("The left-most bytes are dropped")
			{
				REQUIRE(smaller.sign() == Sign::Minus);
				REQUIRE(smaller.byte(1) == bw.byte(4));
				REQUIRE(smaller.byte(2) == bw.byte(5));
			}
		}
		WHEN("Narrowing using assignment operator")
		{
			Basic_word<2> smaller{};
			smaller = bw;
			THEN("The left-most bytes are dropped")
			{
				REQUIRE(smaller.sign() == Sign::Minus);
				REQUIRE(smaller.byte(1) == bw.byte(4));
				REQUIRE(smaller.byte(2) == bw.byte(5));
			}
		}
		WHEN("Converting to a larger word")
		{
			Basic_word<7> larger{bw};
			THEN("The left-most bytes are set to 0")
			{
				REQUIRE(larger.sign() == Sign::Minus);
				REQUIRE(larger.byte(1) == 0);
				REQUIRE(larger.byte(2) == 0);
				REQUIRE(larger.byte(3) == bw.byte(1));
				REQUIRE(larger.byte(4) == bw.byte(2));
				REQUIRE(larger.byte(5) == bw.byte(3));
				REQUIRE(larger.byte(6) == bw.byte(4));
				REQUIRE(larger.byte(7) == bw.byte(5));
			}
		}
	}
}

SCENARIO("Shifting right")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Shifting by more than the number of bytes")
		{
			bw.shift_right(bw.num_bytes + 1);
			THEN("All bytes are 0")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
		}
		WHEN("Shifting by less than the number of bytes")
		{
			bw.shift_right(bw.num_bytes - 2);
			THEN("Bytes are shifted, and left-filled with 0")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 1, 2});
			}
		}
		WHEN("Shifting all bytes")
		{
			bw.shift_right(bw.num_bytes);
			THEN("All bytes are 0")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
		}
		WHEN("Shifting by a negative amount")
		{
			
			THEN("Invalid argument is thrown")
			{
				REQUIRE_THROWS_AS(bw.shift_right(-1), std::invalid_argument);
			}
		}
		WHEN("Shifting by 0")
		{
			bw.shift_right(0);
			THEN("Word is unnaffected")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Copy-shifting")
		{
			Basic_word<5> copy{bw.shifted_right(3)};
			THEN("The new word is shifted correctly, the old is unnaffected")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});

				REQUIRE(copy.sign() == Sign::Minus);
				require_bytes_are(copy, {0, 0, 0, 1, 2});
			}
		}
	}
}

SCENARIO("Rotating right")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Rotating by 0")
		{
			bw.rotate_right(0);
			THEN("The word is unchanged")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Rotating by a negative amount")
		{
			THEN("An exception is thrown")
			{
				REQUIRE_THROWS_AS(bw.rotate_right(-1), std::invalid_argument);
			}
		}
		WHEN("Rotating by an amount equal to the number of bytes")
		{
			bw.rotate_right(bw.num_bytes);
			THEN("The basic word is unchanged")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Rotating by an amount larger than number of bytes")
		{
			bw.rotate_right(bw.num_bytes + 1);
			THEN("Full rotations are skipped. Only remainder is considered")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {5, 1, 2, 3, 4});
			}
		}
		WHEN("Rotating by an amount between [0, Number of bytes]")
		{
			bw.rotate_right(3);
			THEN("Bytes are rotated correctly")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {3, 4, 5, 1, 2});
			}
		}
	}
}

SCENARIO("Left shifting")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Shifting by a negative amount")
		{
			THEN("Invalid argument is thrown")
			{
				REQUIRE_THROWS_AS(bw.shift_left(-1), std::invalid_argument);
			}
		}
		WHEN("Shifting by more than the number of bytes")
		{
			bw.shift_left(bw.num_bytes + 2);
			THEN("All bytes are 0")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
		}
		WHEN("Shifting by less than the number of bytes")
		{
			bw.shift_left(bw.num_bytes - 3);
			THEN("Bytes are shifted, and right-filled with 0")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {3, 4, 5, 0, 0});
			}
		}
		WHEN("Shifting all bytes")
		{
			bw.shift_left(bw.num_bytes);
			THEN("All bytes are 0")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
		}
		WHEN("Shifting by 0")
		{
			bw.shift_left(0);
			THEN("Word is unnaffected")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Copy-shifting")
		{
			Basic_word<5> copy{bw.shifted_left(3)};
			THEN("The new word is shifted correctly, the old is unnaffected")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});

				REQUIRE(copy.sign() == Sign::Minus);
				require_bytes_are(copy, {4, 5, 0, 0, 0});
			}
		}
	}
}

SCENARIO("Rotating left")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Rotating by 0")
		{
			bw.rotate_left(0);
			THEN("The word is unchanged")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Rotating by a negative amount")
		{
			THEN("An invalid argument is thrown")
			{
				REQUIRE_THROWS_AS(bw.rotate_left(-1), std::invalid_argument);
			}
		}
		WHEN("Rotating by an amount larger than number of bytes")
		{
			bw.rotate_left(bw.num_bytes + 2);
			THEN("The full rotations are ignored, only remainder is used")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {3, 4, 5, 1, 2});
			}
		}
		WHEN("Rotating by an amount equal to the number of bytes")
		{
			bw.rotate_left(bw.num_bytes);
			THEN("The basic word is unchanged")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Rotating by an amount between [0, Number of bytes]")
		{
			bw.rotate_left(bw.num_bytes - 3);
			THEN("The bytes are rotated correctly")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {3, 4, 5, 1, 2});
			}
		}
	}
}

SCENARIO("Copying a range")
{
	GIVEN("Two basic words")
	{
		Basic_word<5> first{Sign::Plus, {1, 2, 3, 4, 5}};
		Basic_word<5> second{Sign::Minus, {6, 7, 8, 9, 0}};
		WHEN("First index in range is negative")
		{
			THEN("Invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(first.copy_range(second, -1, 0),
								  std::invalid_argument);
			}
		}
		WHEN("First index is greater than last index")
		{
			THEN("Invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(first.copy_range(second, 1, 0),
								  std::invalid_argument);
			}
		}
		WHEN("Last index is greater than the number of bytes")
		{
			THEN("Invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(first.copy_range(second, 0, 6),
								  std::invalid_argument);
			}
		}
		WHEN("Copying entire range")
		{
			first.copy_range(second, 0, 5);
			THEN("All bytes from second are copied into first, including sign")
			{
				REQUIRE(first.sign() == second.sign());
				require_bytes_match(first, second);
			}
		}
		WHEN("Copying range that doesn't include the sign")
		{
			first.copy_range(second, 1, 4);
			THEN("Only applicable bytes are copied, not the sign")
			{
				REQUIRE(first.sign() == Sign::Plus);
				REQUIRE(first.byte(1) == second.byte(1));
				REQUIRE(first.byte(2) == second.byte(2));
				REQUIRE(first.byte(3) == second.byte(3));
				REQUIRE(first.byte(4) == second.byte(4));
				REQUIRE(first.byte(5) == 5);
			}
		}
		WHEN("Copying a range of a single byte")
		{
			first.copy_range(second, 3, 3);
			THEN("Only the byte is copied")
			{
				REQUIRE(first.sign() == Sign::Plus);
				require_bytes_are(first, {1, 2, 8, 4, 5});
			}
		}
		WHEN("Copying only the sign")
		{
			first.copy_range(second, 0, 0);
			THEN("Only the sign is copied")
			{
				REQUIRE(first.sign() == Sign::Minus);
				require_bytes_are(first, {1, 2, 3, 4, 5});
			}
		}
	}
}

SCENARIO("Reading and writing a basic word")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{};
		WHEN("Written to an output stream")
		{
			std::stringstream ss{};
			ss << bw;
			THEN("The sign and bytes are written correctly")
			{
				std::string s{ss.str()};
				REQUIRE(s[0] == '+');
				REQUIRE(s[1] == 0);
				REQUIRE(s[2] == 0);
				REQUIRE(s[3] == 0);
				REQUIRE(s[4] == 0);
				REQUIRE(s[5] == 0);
			}
		}
		WHEN("Read from an input stream")
		{
			std::stringstream ss{};
			ss << bw;
			Basic_word<5> bw2{Sign::Minus, {1, 2, 3, 4, 5}};
			ss >> bw2;
			THEN("The sign and bytes are read correctly")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {0, 0, 0, 0, 0});
			}
		}
		WHEN("Reading an incomplete word")
		{
			std::stringstream ss{};
			ss << Sign::Plus;
			for (int i = 0; i < 4; ++i)
				ss << static_cast<Byte>(i);
			ss >> bw;
			THEN("Some bytes are marked as invalid")
			{
				REQUIRE(bw.sign() == Sign::Plus);
				require_bytes_are(bw, {0, 1, 2, 3, INVALID_BYTE});
			}
		}
	}
}

SCENARIO("Converting a range to an integer", "[A]")
{
	GIVEN("A basic word of [ 0x1f, 0x10 ]")
	{
		Basic_word<2> bw{Sign::Plus, {0x1f, 0x10}};
		WHEN("Converting whole word to integer")
		{
			int i{bw.to_int()};
			THEN("The integer is 2000")
			{
				REQUIRE(i == 2000);
			}
		}
	}
	GIVEN("A basic word of [ 0, 0x1f, 0x10, 0, 20]")
	{
		Basic_word<5> bw{Sign::Plus, {0, 0x1f, 0x10, 0, 20}};
		WHEN("Converting range of [2,3] to integer")
		{
			int i{bw.to_int(2, 3)};
			THEN("The integer is 2000")
			{
				REQUIRE(i == 2000);
			}
		}
	}
	GIVEN("A basic word of -[ 1, 2, 3, 4, 5 ]")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Converting range of [1, 4] to integer")
		{
			int i{bw.to_int(1, 4)};
			THEN("The integer is 270532")
			{
				REQUIRE(i == 270532);
			}

			Field_spec fs{1, 4};
			i = bw.to_int(fs);
			THEN("The integer is 270532")
			{
				REQUIRE(i == 270532);
			}
		}
		WHEN("Converting a range of [0, 4] to integer")
		{
			int i{bw.to_int(0, 4)};
			THEN("The integer is -270532")
			{
				REQUIRE(i == -270532);
			}

			Field_spec fs{0, 4};
			i = bw.to_int(fs);
			THEN("The integer is -270532")
			{
				REQUIRE(i == -270532);
			}
		}
		WHEN("Converting an invalid range to integer")
		{
			THEN("An invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(bw.to_int(-1, 1),
								  std::invalid_argument);

				REQUIRE_THROWS_AS(bw.to_int(1, 6),
								  std::invalid_argument);
				Field_spec fs{1, 6};
				REQUIRE_THROWS_AS(bw.to_int(fs),
								  std::invalid_argument);
			}
		}
	}
}

SCENARIO("Negating the sign")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Plus};
		WHEN("Negating the sign")
		{
			bw.negate();
			THEN("A sign of plus becomes minus")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
			bw.negate();
			THEN("A sign of minus becomes plus")
			{
				REQUIRE(bw.sign() == Sign::Plus);
				require_bytes_are(bw, {0, 0, 0, 0, 0});
			}
		}
	}
}

SCENARIO("Aligning fields")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Left aligning (4:5) field")
		{
			Basic_word<5> bw2{bw.field_aligned_left({4, 5})};
			THEN("Bytes [4,5] are shifted to [1,2]")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {4, 5, 0, 0, 0});
			}
		}
		WHEN("Left aligning (3:3) field")
		{
			Basic_word<5> bw2{bw.field_aligned_left({3, 3})};
			THEN("Byte 3 is moved to position 1")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {3, 0, 0, 0, 0});
			}
		}
		WHEN("Left aligning (0:4) field")
		{
			Basic_word<5> bw2{bw.field_aligned_left({0, 4})};
			THEN("The fifth byte is 0, the rest are untouched")
			{
				REQUIRE(bw2.sign() == Sign::Minus);
				require_bytes_are(bw2, {1, 2, 3, 4, 0});
			}
		}
		WHEN("Right aligning (1:3) field")
		{
			Basic_word<5> bw2{bw.field_aligned_right({1, 3})};
			THEN("Bytes [1,3] are shifted to [3,5]")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {0, 0, 1, 2, 3});
			}
		}
		WHEN("Right aligning (4:4) field")
		{
			Basic_word<5> bw2{bw.field_aligned_right({4, 4})};
			THEN("Byte 4 is moved to position 5")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {0, 0, 0, 0, 4});
			}
		}
		WHEN("Right aligning (0:2) field")
		{
			Basic_word<5> bw2{bw.field_aligned_right({0, 2})};
			THEN("Sign is changed, and bytes [1,2] are moved to [4,5]")
			{
				REQUIRE(bw2.sign() == Sign::Minus);
				require_bytes_are(bw2, {0, 0, 0, 1, 2});
			}
		}
	}
}

SCENARIO("Left-most bytes")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Asking for the 3 leftmost bytes")
		{
			Basic_word<5> bw2{bw.leftmost_bytes(3)};
			THEN("The new word contains only the 3 leftmost bytes")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {1, 2, 3, 0, 0});
			}
		}
		WHEN("Asking for the 5 leftmost bytes")
		{
			Basic_word<5> bw2{bw.leftmost_bytes(5)};
			THEN("The new word is the same as the old, except the sign")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Asking for more bytes than the word contains")
		{
			THEN("An invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(bw.leftmost_bytes(10),
								  std::invalid_argument);
			}
		}
	}
}

SCENARIO("Left-most bytes with sign")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Asking for the 2 leftmost bytes")
		{
			Basic_word<5> bw2{bw.leftmost_with_sign(2)};
			THEN("New word contains 2 leftmost bytes and the sign")
			{
				REQUIRE(bw2.sign() == Sign::Minus);
				require_bytes_are(bw2, {1, 2, 0, 0, 0});
			}
		}
		WHEN("Asking for the 5 leftmost bytes")
		{
			Basic_word<5> bw2{bw.leftmost_with_sign(5)};
			THEN("New word is an exact copy of old one")
			{
				REQUIRE(bw2.sign() == Sign::Minus);
				require_bytes_are(bw2, {1, 2, 3, 4, 5});
			}
		}
	}
}

SCENARIO("Right-most bytes")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Asking for the 3 rightmost bytes")
		{
			Basic_word<5> bw2{bw.rightmost_bytes(3)};
			THEN("The new word contains 3 rightmost bytes")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {0, 0, 3, 4, 5});
			}
		}
		WHEN("Asking for the 5 rightmost bytes")
		{
			Basic_word<5> bw2{bw.rightmost_bytes(5)};
			THEN("The new word contains all bytes of the original")
			{
				REQUIRE(bw2.sign() == Sign::Plus);
				require_bytes_are(bw2, {1, 2, 3, 4, 5});
			}
		}
		WHEN("Asking for too many bytes")
		{
			THEN("An invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(bw.rightmost_bytes(10),
								  std::invalid_argument);
			}
		}
	}
}

SCENARIO("Right-most bytes with sign")
{
	GIVEN("A basic word")
	{
		Basic_word<5> bw{Sign::Minus, {1, 2, 3, 4, 5}};
		WHEN("Asking for the 2 rightmost bytes")
		{
			Basic_word<5> bw2{bw.rightmost_with_sign(2)};
			THEN("The new word copied bytes [4,5] and the sign")
			{
				REQUIRE(bw2.sign() == Sign::Minus);
				require_bytes_are(bw2, {0, 0, 0, 4, 5});
			}
		}
		WHEN("Asking for the 5 rightmost bytes")
		{
			Basic_word<5> bw2{bw.rightmost_with_sign(5)};
			THEN("The new word is an exact copy of the original")
			{
				REQUIRE(bw2.sign() == Sign::Minus);
				require_bytes_are(bw2, {1, 2, 3, 4, 5});
			}
		}
	}
}

SCENARIO("Int max")
{
	WHEN("Asking for the maximum integer value of a basic word")
	{
		int max{Basic_word<5>::int_max()};
		THEN("The returned value is accurate")
		{
			REQUIRE(max == static_cast<int>(std::pow(2, 30)) - 1);
		}
	}
}

SCENARIO("Int min")
{
	WHEN("Asking for the minimum integer value of a basic word")
	{
		int min{Basic_word<5>::int_min()};
		THEN("The returned value is accurate")
		{
			REQUIRE(min == -(static_cast<int>(std::pow(2, 30)) - 1));
		}
	}
}

SCENARIO("Constructing from an int")
{
	GIVEN("An integer value")
	{
		WHEN("Constructing a basic word from a positive int")
		{
			int value{2000};
			Basic_word<5> bw{value};
			THEN("The appropriate word is constructed")
			{
				REQUIRE(bw.sign() == Sign::Plus);
				require_bytes_are(bw, {0, 0, 0, 31, 16});
			}
		}
		WHEN("Constructing a basic word from a negative int")
		{
			int value{-2000};
			Basic_word<5> bw{value};
			THEN("The appropriate word is constructed")
			{
				REQUIRE(bw.sign() == Sign::Minus);
				require_bytes_are(bw, {0, 0, 0, 31, 16});
			}
		}
		WHEN("Constructing from an int which is too large")
		{
			int value{Basic_word<5>::int_max() + 1};
			THEN("An invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(
					Basic_word<5>{value},
					std::invalid_argument
				);
			}
		}
		WHEN("Constructing from a negative int which is too large")
		{
			int value{-(Basic_word<5>::int_max() + 1)};
			THEN("An invalid argument exception is thrown")
			{
				REQUIRE_THROWS_AS(
					Basic_word<5>{value},
					std::invalid_argument
				);
			}
		}
	}
}
