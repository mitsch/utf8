///
///      @file  read_utf8_file.cpp
///     @brief  test suite of codecvt_utf8
///
/// The application creates an instance of std::ifstream which opens a file given by the
/// first argument. Immediately after instancing the own codecvt_utf8 facet will be set
/// and will put the incoming characters into the wide character standard output. Any 
/// error or irregular behavior will lead to an abortion.
///
///    @author  Michael Koch (mk), uockc@stud.uni-karlsruhe.de
///
///  @internal
///    Created  12/16/2010
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
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

///
/// @brief  read file encoded in utf8
/// @param  filename name of the file to open
///
void read_utf8_file (const char* filename) {
	assert(filename != NULL);

	std::wifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error(std::string("couldn't be open: ") + filename);
	}

	file.imbue(std::locale(file.getloc(), new code::codecvt_utf8<char, wchar_t>()));
	file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	std::wcout << file.rdbuf();
}


///
/// @brief call read_utf8_file with first argument
///
int main(const int argc, const char* argv[]) {
	if (argc < 2) {
		throw std::runtime_error("not enough parameters");
	}

	read_utf8_file(argv[1]);

	return 0;
}


