
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>

#define LINE_BUF_LEN (1024*16)

typedef std::pair<int, int> int_pair_t;

std::string read_line(std::istream& in)
{
	static char buf[LINE_BUF_LEN+1];
	in.getline(buf, LINE_BUF_LEN);
	return std::string(buf);
}

int_pair_t to_int_pair(const std::string& line)
{
	int_pair_t ret;
	std::stringstream in(line);
	in >> ret.first;
	in >> ret.second;
	return ret;
}

int_pair_t order(const int_pair_t& p) {
	return p.first < p.second ? p : std::make_pair(p.second, p.first);
}

void print(const int_pair_t& p)
{
	std::cerr << p.first << "," << p.second << std::endl;
}

int cycle_number(int n)
{
	int len=1;

	while (n != 1) {
		len++;
		if (n%2 == 0) { n = n/2; }
		else { n = n*3 + 1; }
	}
	
	return len;
}

int main()
{
	std::string line;
	do {
		line = read_line(std::cin);
		if (line.empty()) {
			break;
		}

		int_pair_t p = to_int_pair(line);
		int_pair_t op = order(p);
		int maxcn = 0;
		for (int i=op.first; i<=op.second; ++i) {
			maxcn = std::max(maxcn, cycle_number(i));
		}

		std::cout << p.first << " " << p.second << " " << maxcn << std::endl;
	} while (true);

	return 0;
}
