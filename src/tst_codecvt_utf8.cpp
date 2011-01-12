///
///      @file  tst_codecvt_utf8.cpp
///     @brief  tests for codecvt_utf8
///
/// Detailed description starts here.
///
///    @author  Michael Koch (mk), uockc@stud.uni-karlsruhe.de
///
///  @internal
///    Created  12/28/2010
///   Revision  ---
///   Compiler  gcc/g++
///    Company  Universität Karlsruhe, Germany
///  Copyright  Copyright (c) 2010, Michael Koch
///
/// This source code is released for free distribution under the terms of the
/// GNU General Public License as published by the Free Software Foundation.
///=====================================================================================
///


#include <codecvt_utf8.hpp>
#include <gtest/gtest.h>

namespace code
{
	TEST(CodecvtUtf8FunctionalTest, MethodMaxUnderscoreLength)
	{
		codecvt_utf8 cc();
		EXPECT_EQ(6, cc.max_length());
	}

	TEST(CodecvtUtf8FunctionalTest, MethodAlwaysUnderscoreNoconv)
	{
		codecvt_utf8 cc();
		EXPECT_EQ(false, cc.always_noconv());
	}

	TEST(CodecvtUtf8FunctionalTest, MethodEncoding)
	{
		codecvt_utf8 cc();
		EXPECT_EQ(-1, cc.encoding());
	}
}

