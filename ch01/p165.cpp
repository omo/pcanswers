
// http://www.programming-challenges.com/pg.php?page=downloadproblem&probid=110103&format=html

#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>

#define LINE_BUF_LEN (1024*16)

std::string read_line(std::istream& in)
{
	static char buf[LINE_BUF_LEN+1];
	in.getline(buf, LINE_BUF_LEN);
	return std::string(buf);
}

void sort_order(int* x, int* y)
{
	if (*y < *x) {
		int t = *x;
		*x = *y;
		*y = t;
	}
}

class bitmap_t
{
public:
	bitmap_t(size_t w=0, size_t h=0) : m_width(w), m_height(h), m_pixels(w*h, 'O') {}
	void clear(char ch='O') { std::fill(m_pixels.begin(), m_pixels.end(), ch); }
	char get(size_t x, size_t y) { return m_pixels.at(y*m_width+x); }
	void set(size_t x, size_t y, char p) { m_pixels[y*m_width+x] = p; }
	char get_1(size_t x, size_t y) { return get(x-1, y-1); }
	void set_1(size_t x, size_t y, char p) { set(x-1, y-1, p); }

	size_t width() const { return m_width; }
	size_t height() const { return m_height; }
private:
	size_t m_width;
	size_t m_height;
	std::vector<char> m_pixels;
};

class interpreter_t
{
public:
	void execute(const std::string& line)
	{
		std::stringstream ls(line);
		char cmd;
		ls >> cmd;
		switch (cmd) {
		case 'I': {
			int w, h;
			ls >> w;
			ls >> h;
			m_bitmap = bitmap_t(w, h);
		} break;
		case 'C': {
			m_bitmap.clear();
		} break;
		case 'S': {
			ls.get(); // skip whitespace
			std::string filename = read_line(ls);
			save(std::cout, filename);
		} break;
		case 'L': {
			int x, y;
			char c;
			ls >> x;
			ls >> y;
			ls >> c;
			//std::cout << "color:" <<  x << "," << y << "," << c << std::endl;
			m_bitmap.set_1(x, y, c);
		} break;
		case 'V': {
			int x, y1, y2;
			char c;
			ls >> x;
			ls >> y1;
			ls >> y2;
			ls >> c;
			sort_order(&y1, &y2);
			for (int i=y1; i<=y2; ++i) {
				m_bitmap.set_1(x, i, c);
			}
		} break;
		case 'H': {
			int x1, x2, y;
			char c;
			ls >> x1;
			ls >> x2;
			ls >> y;
			ls >> c;
			sort_order(&x1, &x2);
			for (int i=x1; i<=x2; ++i) {
				m_bitmap.set_1(i, y, c);
			}
		} break;
		case 'K': {
			int x1, x2, y1, y2;
			char c;
			ls >> x1;
			ls >> y1;
			ls >> x2;
			ls >> y2;
			ls >> c;
			for (int i=y1; i<=y2; ++i) {
				for (int j=x1; j<=x2; ++j) {
					//std::cout << "p:" << j << "," << i << std::endl;
					m_bitmap.set_1(j, i, c);
				}
			}
		} break;
		case 'F': {
			int x, y;
			char c;
			ls >> x;
			ls >> y;
			ls >> c;

			fill(x, y, c);
		} break;
		default:
			//std::cout << "unknown command:" << cmd << std::endl;
			break;
		}
	}

	void save(std::ostream& out, const std::string& name)
	{
		out << name << std::endl;
		for (size_t i=0; i<m_bitmap.height(); ++i) {
			for (size_t j=0; j<m_bitmap.width(); ++j) {
				out << m_bitmap.get(j, i);
			}
			out << std::endl;
		}
	}
	
	void fill_mask(int x, int y, char ch, bitmap_t* mask)
	{
		if (x < 0 || m_bitmap.width()  <= size_t(x) ||
				y < 0 || m_bitmap.height() <= size_t(y)) {
			return;
		}

		//std::cout << "mark:" << x << "," << y << "," << ch << std::endl;
		if (ch == m_bitmap.get(x, y) && 'M' != mask->get(x, y)) {
			mask->set(x, y, 'M');
			fill_mask(x-1, y+0, ch, mask);
			fill_mask(x+1, y+0, ch, mask);
			fill_mask(x+0, y-1, ch, mask);
			fill_mask(x+0, y+1, ch, mask);
		}
	}

	void fill(int x, int y, char ch)
	{
		int x0 = x-1;
		int y0 = y-1;
		bitmap_t mask(m_bitmap.width(), m_bitmap.height());
		fill_mask(x0, y0, m_bitmap.get(x0, y0), &mask);

		for (size_t i=0; i<m_bitmap.height(); ++i) {
			for (size_t j=0; j<m_bitmap.width(); ++j) {
				if ('M' == mask.get(j, i)) {
					m_bitmap.set(j, i, ch);
				}
			}
		}
	}
private:
	bitmap_t m_bitmap;
};

int main()
{
	interpreter_t interp;

	do {
		std::string line = read_line(std::cin);
		if ("X" == line) {
			break;
		}

		interp.execute(line);
	} while (true);

	return 0;
}
