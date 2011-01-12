#include <gtest/gtest.h>
#include <string>
#include <codecvt_utf8.hpp>

class Utf8Test : public ::testing::Test {

	public:

			code::codecvt_utf8<char, wchar_t> conversation_strict;
			code::codecvt_utf8_state<wchar_t> state;
			std::string utf8_text;
			std::wstring unicode_text;
			
			Utf8Test() {
				const char buf1 [] = {(char)0x7f, (char)0xdf, (char)0xbf, (char)0xef, (char)0xbf, (char)0xbf,
															(char)0xf7, (char)0xbf, (char)0xbf, (char)0xbf, (char)0x00};
				const wchar_t buf2 [] = {0x7f, 0x7ff, 0xffff, 0x10ffff, 0x00};
				utf8_text = std::string(buf1);
				ASSERT_STREQ(buf1, utf8_text.c_str());
				unicode_text = std::wstring(buf2);
				ASSERT_STREQ(buf2, unicode_text.c_str());
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

TEST_F(Utf8Test, codecvt_utf8_simple_test) {
	code::codecvt_utf8_state<wchar_t> state;
	EXPECT_EQ(-1, conversation_strict.encoding());
	EXPECT_EQ(false, conversation_strict.always_noconv());
	EXPECT_EQ(6, conversation_strict.max_length());
	state = code::codecvt_utf8_state<wchar_t>();
	EXPECT_EQ(unicode_text.length(), conversation_strict.length(state, utf8_text.c_str(), utf8_text.c_str()+utf8_text.size(), unicode_text.length()+2));
	EXPECT_EQ(true, state.is_decoding_complete());
	state = code::codecvt_utf8_state<wchar_t>();
	EXPECT_EQ(unicode_text.length(), conversation_strict.length(state, utf8_text.c_str(), utf8_text.c_str()+utf8_text.size(), unicode_text.length()));
	EXPECT_EQ(true, state.is_decoding_complete());
	state = code::codecvt_utf8_state<wchar_t>();
	EXPECT_EQ(unicode_text.length()-2, conversation_strict.length(state, utf8_text.c_str(), utf8_text.c_str()+utf8_text.size(), unicode_text.length()-2));
	EXPECT_EQ(true, state.is_decoding_complete());
	
	char* convertion_out_buffer = new char [utf8_text.size()];
	wchar_t* convertion_in_buffer = new wchar_t [unicode_text.size()];

	{
		state = code::codecvt_utf8_state<wchar_t>();
		const char* out_next = 0;
		wchar_t* in_next = 0;
		wchar_t* buffer = new wchar_t [unicode_text.size()];
		EXPECT_EQ(std::codecvt_base::ok, conversation_strict.in(state, utf8_text.c_str(), utf8_text.c_str()+utf8_text.size(), out_next,
			buffer, buffer+unicode_text.size(), in_next));
		EXPECT_STREQ(unicode_text.c_str(), buffer);
		EXPECT_EQ(utf8_text.c_str()+utf8_text.size(), out_next);
		EXPECT_EQ(buffer+unicode_text.size(), in_next);
	}
}


