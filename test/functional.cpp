#include <gtest/gtest.h>
#include <string>
#include <codecvt_utf8.hpp>
#include <iterator>
#include <cstdio>
#include <fstream>
#include <locale>

class Utf8Test : public ::testing::Test {

	public:

			code::codecvt_utf8<char, wchar_t> convertion_strict;
			code::codecvt_utf8_state<wchar_t> state;
			
			char* simple_utf8_text;
			wchar_t* simple_unicode_text;
			size_t simple_utf8_text_length;
			size_t simple_unicode_text_length;

			void get_simple_utf8_text(char*& buffer) {
				buffer = new char [11];
				buffer[0] = 0x7f;
				buffer[1] = 0xdf;
				buffer[2] = 0xbf;
				buffer[3] = 0xef;
				buffer[4] = 0xbf;
				buffer[5] = 0xbf;
				buffer[6] = 0xf4;
				buffer[7] = 0x8f;
				buffer[8] = 0xbf;
				buffer[9] = 0xbf;
				buffer[10] = 0x00;
			}

			void get_simple_unicode_text(wchar_t*& buffer) {
				buffer = new wchar_t [5];
				buffer[0] = 0x7f;
				buffer[1] = 0x7ff;
				buffer[2] = 0xffff;
				buffer[3] = 0x10ffff;
				buffer[4] = 0x00;
			}

			
			Utf8Test() {
				get_simple_utf8_text(simple_utf8_text);
				simple_utf8_text_length = strlen(simple_utf8_text);
				get_simple_unicode_text(simple_unicode_text);
				simple_unicode_text_length = wcslen(simple_unicode_text);
			}

};

TEST_F(Utf8Test, codecvt_utf8_state_simple_test) {
	EXPECT_EQ(0, state.left_characters_to_encode());
	EXPECT_EQ(0, state.left_characters_to_decode());
	EXPECT_EQ(true, state.is_encoding_complete());
	EXPECT_EQ(true, state.is_decoding_complete());
	EXPECT_EQ(true, state.is_valid());

	const int size = 4;
	const wchar_t character = L'a';
	state.encode(size, character);
	for (int sizeIter=size-1;sizeIter >= 0; sizeIter--) {
		EXPECT_EQ(character, state.unicode());
		EXPECT_EQ(sizeIter, state.left_characters_to_encode());
		EXPECT_EQ(sizeIter==0?true:false, state.is_encoding_complete());
		if (sizeIter > 0)
			state.encoded();
	}
	state.decode(size);
	for (int sizeIter=size-1;sizeIter >= 0; sizeIter--) {
		EXPECT_EQ(character, state.unicode());
		EXPECT_EQ(sizeIter, state.left_characters_to_decode());
		EXPECT_EQ(sizeIter==0?true:false, state.is_decoding_complete());
		if (sizeIter > 0)
			state.decoded();
	}
}


TEST_F(Utf8Test, codecvt_utf8_strict_conv_encoding) {
	EXPECT_EQ(-1, convertion_strict.encoding());
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_always_noconv) {
	EXPECT_EQ(false, convertion_strict.always_noconv());
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_max_length) {
	EXPECT_EQ(6, convertion_strict.max_length());
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_length_simple_1) {
	code::codecvt_utf8_state<wchar_t> state;
	EXPECT_EQ(simple_unicode_text_length,
		convertion_strict.length(state, simple_utf8_text, simple_utf8_text+simple_utf8_text_length, simple_unicode_text_length+2));
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_length_simple_2) {
	code::codecvt_utf8_state<wchar_t> state;
	EXPECT_EQ(simple_unicode_text_length,
		convertion_strict.length(state, simple_utf8_text, simple_utf8_text+simple_utf8_text_length, simple_unicode_text_length));
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_length_simple_3) {
	code::codecvt_utf8_state<wchar_t> state;
	EXPECT_EQ(simple_unicode_text_length-2,
		convertion_strict.length(state, simple_utf8_text, simple_utf8_text+simple_utf8_text_length, simple_unicode_text_length-2));
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_in_simple) {
	code::codecvt_utf8_state<wchar_t> state;
	const char* out_next = 0;
	wchar_t* in_next = 0;
	wchar_t* buffer = new wchar_t [simple_unicode_text_length];
	EXPECT_EQ(std::codecvt_base::ok,
		convertion_strict.in(state, simple_utf8_text, simple_utf8_text+simple_utf8_text_length, out_next, buffer, buffer+simple_unicode_text_length, in_next));
	for (size_t s = 0; s < simple_unicode_text_length; s++) {
		EXPECT_EQ(simple_unicode_text[s], buffer[s]);
	}
	EXPECT_EQ(simple_utf8_text+simple_utf8_text_length, out_next);
	EXPECT_EQ(buffer+simple_unicode_text_length, in_next);
}
TEST_F(Utf8Test, codecvt_utf8_strict_conv_out_simple) {
	code::codecvt_utf8_state<wchar_t> state;
	char* out_next;
	const wchar_t* in_next;
	char* buffer = new char [simple_utf8_text_length];
	EXPECT_EQ(std::codecvt_base::ok,
		convertion_strict.out(state, simple_unicode_text, simple_unicode_text+simple_unicode_text_length, in_next, buffer, buffer+simple_utf8_text_length, out_next));
	for (size_t s=0; s<simple_utf8_text_length; s++) {
		EXPECT_EQ(simple_utf8_text[s], buffer[s]);
	}
	EXPECT_EQ(simple_unicode_text+simple_unicode_text_length, in_next);
	EXPECT_EQ(buffer+simple_utf8_text_length, out_next);
}


TEST_F(Utf8Test, codecvt_utf8_strict_final_decoding_test) {
	std::wifstream actual("UTF-8-demo.txt");
	std::wifstream expected("UNICODE-8-demo.txt");

	ASSERT_EQ(true, actual.is_open());
	ASSERT_EQ(true, expected.is_open());

	code::codecvt_utf8<char, wchar_t>* c = new code::codecvt_utf8<char, wchar_t>();
	actual.imbue(std::locale(actual.getloc(), c));
	ASSERT_EQ(c, use_facet<std::codecvt>(actual.getloc()));

	std::istream_iterator<wchar_t, wchar_t> actual_iter(actual);
	std::istream_iterator<wchar_t, wchar_t> expected_iter(expected);
	std::istream_iterator<wchar_t, wchar_t> eos;

	while (actual_iter != eos && expected_iter != eos) {
		ASSERT_EQ(*actual_iter, *expected_iter);
		actual_iter++;
		expected_iter++;
	}
	EXPECT_EQ(true, eos == actual_iter);
	EXPECT_EQ(true, eos == expected_iter);
}

