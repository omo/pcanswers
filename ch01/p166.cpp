
// http://www.programming-challenges.com/pg.php?page=studenthome
// NOT solved!!
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>

#define LINE_BUF_LEN (1024*16)

typedef unsigned int word_t;
typedef unsigned char char_t;

std::string read_line(std::istream& in)
{
	static char buf[LINE_BUF_LEN+1];
	in.getline(buf, LINE_BUF_LEN);
	return std::string(buf);
}

struct code_t { char_t op, i1, i2; };

class machine_t
{
public:
	enum { ram_size = 1000, reg_size = 10 };
	typedef std::vector<word_t> word_vector_t;

	machine_t() : m_reg(reg_size, 0), m_ram(ram_size, 0) {}

	void store(size_t index, word_t w)  { m_ram[index] = w%1000; }

	size_t execute()
	{
		size_t nstep = 0;
		size_t pc    = 0;

		while (true) {
			word_t w = m_ram[pc++];
			code_t c = decode(w);
			nstep++;

			//std::cout << "op:" << c.op << std::endl;
			switch (c.op) {
			case 1:
				if (100 == w) {
					return nstep;
				} else {
					assert(false);
				}
			case 2:
				m_reg[c.i1] = c.i2;
				break;
			case 3:
				m_reg[c.i1] = (m_reg[c.i1] + c.i2)%1000;
				break;
			case 4:
				m_reg[c.i1] = (m_reg[c.i1] * c.i2)%1000;
				break;
			case 5:
				m_reg[c.i1] = m_reg[c.i2];
				break;
			case 6:
				m_reg[c.i1] = (m_reg[c.i1] + m_reg[c.i2])%1000;
				break;
			case 7:
				m_reg[c.i1] = (m_reg[c.i2] * m_reg[c.i1])%1000;
				break;
			case 8:
				m_reg[c.i1] = m_ram[m_reg[c.i2]];
				break;
			case 9:
				m_ram[m_reg[c.i2]] = m_reg[c.i1];
				break;
			case 0:
				if (0 != m_reg[c.i2]) {
					pc = m_reg[c.i1];
				}
				break;
			default:
				assert(false);
				break;
			}
		}

		return nstep;
	}

	code_t decode(word_t w)
	{
		code_t r;
		r.op = (w/100)%10;
		r.i1 = (w/ 10)%10;
		r.i2 = (w/  1)%10;
		return r;
	}

	void dump(std::ostream& out) const
	{
		out << "reg:" << std::endl;
		for (word_vector_t::const_iterator i=m_reg.begin();
				 i!=m_reg.end(); ++i) {
			out << *i << std::endl;
		}

		out << "ram:" << std::endl;
		for (word_vector_t::const_iterator i=m_ram.begin();
				 i!=m_ram.end(); ++i) {
			out << *i << std::endl;
		}
	}

	word_vector_t m_reg;
	word_vector_t m_ram;
};

int main()
{
	std::string first = read_line(std::cin);
	read_line(std::cin); // skip first blank
	int ncases = 0;
	ncases = atoi(first.c_str());

	for (int i=0; i<ncases; ++i) {
		if (0 < i) { std::cout << std::endl; }

		machine_t m;
		size_t nwords = 0;
		do {
			std::string line = read_line(std::cin);
			if (line.empty()) {
				break;
			}

			std::stringstream ls(line);
			word_t w;
			ls >> w;
			m.store(nwords++, w);
		} while (true);

		std::cout << m.execute() << std::endl;
	}

	return 0;
}
