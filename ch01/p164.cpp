
// http://www.programming-challenges.com/pg.php?page=downloadproblem&probid=110103&format=html

#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

#define LINE_BUF_LEN (1024*16)

std::string read_line(std::istream& in)
{
	static char buf[LINE_BUF_LEN+1];
	in.getline(buf, LINE_BUF_LEN);
	return std::string(buf);
}


bool g_glyphs[10][7] = {
	{1, 1, 1, 0, 1, 1, 1}, // 0
	{0, 0, 1, 0, 0, 1, 0}, // 1
	{1, 0, 1, 1, 1, 0, 1}, // 2 
	{1, 0, 1, 1, 0, 1, 1}, // 3
	{0, 1, 1, 1, 0, 1, 0}, // 4
	{1, 1, 0, 1, 0, 1, 1}, // 5
	{1, 1, 0, 1, 1, 1, 1}, // 6
	{1, 0, 1, 0, 0, 1, 0}, // 7
	{1, 1, 1, 1, 1, 1, 1}, // 8
	{1, 1, 1, 1, 0, 1, 1}, // 9
};

void print_digit_line(std::ostream& out, int size, int digit, int line)
{
	if (10 <= digit) {
		print_digit_line(out, size, digit/10, line);
	}

	int d = digit%10;

	if (d != digit) { out << " "; }

	if (0 == line) {
		out << " ";
		for (int i=0; i<size; ++i) { if (g_glyphs[d][0]) { out << "-";	} else { out << " ";	} }
		out << " ";
	} else if (line  <   size+1) {
		if (g_glyphs[d][1]) {
			out << "|";
		} else {
			out << " ";
		}

		for (int i=0; i<size; ++i) { out << " "; }

		if (g_glyphs[d][2]) {
			out << "|";
		} else { 
			out << " ";
		}
	} else if (line ==   size+1) {
		out << " ";
		for (int i=0; i<size; ++i) { if (g_glyphs[d][3]) { out << "-";	} else { out << " ";	} }
		out << " ";
	} else if (line  < 2*size+1+1) {
		if (g_glyphs[d][4]) {
			out << "|";
		} else {
			out << " ";
		}

		for (int i=0; i<size; ++i) { out << " "; }

		if (g_glyphs[d][5]) {
			out << "|";
		} else { 
			out << " ";
		}
	} else if (line == 2*size+1+1) {
		out << " ";
		for (int i=0; i<size; ++i) { if (g_glyphs[d][6]) { out << "-";	} else { out << " ";	} }
		out << " ";
	}
}

void print_digit(std::ostream& out, int size, int digit)
{
	//out << size << "," << digit << std::endl;
	int h = size*2 + 3;
	for (int i=0; i<h; ++i) {
		print_digit_line(out, size, digit, i);
		out << std::endl;
	}
}

int main()
{

	do {
		std::string line = read_line(std::cin);
		std::stringstream ls(line);
		int size;
		int digit;
		ls >> size;
		ls >> digit;
		if (0 == size && 0 == digit) {
			break;
		}

		if (0 == size) {
			continue;
		}

		print_digit(std::cout, size, digit);
		std::cout << std::endl;
	} while (true);

	return 0;
}
