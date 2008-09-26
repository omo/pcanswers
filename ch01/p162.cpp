
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

struct grid_t {
	size_t w;
	size_t h;
	std::vector< std::string > src;
	std::vector< std::vector<int> >  dst;

	bool empty() const { return w == 0 && h == 0; }

	char dst_char(int x, int y) const {
		if (src[y][x] == '*') {
			return '*';
		} else {
			return '0' + static_cast<char>(dst[y][x]);
		}
	}

	void acc(int x, int y) {
		//std::cout << x << "," << y << std::endl;
		if (0 <= x && x < w && 0 <= y && y < h) {
			dst[y][x] += 1;
		}
	}

};

grid_t read(std::istream& in)
{
	grid_t ret;
	
	std::stringstream head(read_line(in));
	head >> ret.h;
	head >> ret.w;

	for (int i=0; i<ret.h; ++i) {
		std::string line = read_line(in);
		ret.src.push_back(line);
	}

	for (int i=0; i<ret.h; ++i) {
		ret.dst.push_back(std::vector<int>(ret.w, 0));
	}

	return ret;
}

void fill(grid_t& g)
{
	for (int i=0; i<g.h; ++i) {
		for (int j=0; j<g.w; ++j) {
			if ('*' == g.src[i][j]) {
				g.acc(j-1,i-1);
				g.acc(j-1,i+0);
				g.acc(j-1,i+1);
				g.acc(j+0,i-1);
				g.acc(j+0,i-0);
				g.acc(j+0,i+1);
				g.acc(j+1,i-1);
				g.acc(j+1,i-0);
				g.acc(j+1,i+1);
			}
		}
	}
}

void write(std::ostream& out, const grid_t& g)
{
	for (int i=0; i<g.h; ++i) {
		for (int j=0; j<g.w; ++j) {
			out << g.dst_char(j, i);
		}
		out << std::endl;
	}
}

int main()
{
	int i=1;

	do {
		grid_t g = read(std::cin);
		if (g.empty()) {
			break;
		}
		
		if (1 < i) {
			std::cout << std::endl;
		}

		fill(g);
		std::cout << "Field #" << i << ":" << std::endl;
		write(std::cout, g);
		++i;
	} while (true);

	return 0;
}
