
// http://www.programming-challenges.com/pg.php?page=downloadproblem&probid=110107&format=html

#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cctype>
#include <map>

#define LINE_BUF_LEN (1024*16)

typedef unsigned int word_t;
typedef unsigned char char_t;

std::string read_line(std::istream& in)
{
	static char buf[LINE_BUF_LEN+1];
	in.getline(buf, LINE_BUF_LEN);
	return std::string(buf);
}

enum side_e {
	side_black,
	side_white,
	side_none
};

side_e to_side(char ch)
{
	if ('a' <= ch && ch <= 'z') {
		return side_black;
	} else if ('A' <= ch && ch <= 'Z') {
		return side_white;
	} else if ('.') {
		return side_none;
	} else {
		assert(false);
		return side_none;
	}
}

class loc_t : public std::pair<int, int>
{
public:
	enum { size = 8 };
	typedef std::pair<int, int> base_type;
	loc_t(int x, int y) : base_type(x, y) {}
	int x() const { return first; }
	int y() const { return second; }

	loc_t opposite() const { return loc_t(x(), size-y()-1); }
	bool inside() const { return 0 <= x() && x() < size && 0 <= y() && y() < size; }
};

loc_t operator+(const loc_t& a, const loc_t& b)
{
	return loc_t(a.x() + b.x(), a.y() + b.y());
}

typedef std::vector<loc_t> path_t;
typedef std::vector<path_t> path_list_t;

typedef std::map<loc_t, char> board_t;
typedef std::map<char, path_list_t> path_map_t;

class judgement_t
{
public:
	judgement_t()	{	register_paths();	}

	void register_paths()
	{
		/*
		 * pawn
		 */
		path_list_t p;
		path_t p0, p1;
		p0.push_back(loc_t(-1, +1));
		p.push_back(p0);
		p1.push_back(loc_t(+1, +1));
		p.push_back(p1);
		m_paths.insert(std::make_pair('p', p));

		/*
		 * rook
		 */
		path_list_t r;
		path_t r0, r1, r2, r3;
		for (int i=1; i<loc_t::size; ++i) {
			r0.push_back(loc_t( 0,  i));
			r1.push_back(loc_t( 0, -i));
			r2.push_back(loc_t( i,  0));
			r3.push_back(loc_t(-i,  0));
		}
		r.push_back(r0);
		r.push_back(r1);
		r.push_back(r2);
		r.push_back(r3);
		m_paths.insert(std::make_pair('r', r));

		/*
		 * bishop
		 */
		path_list_t b;
		path_t b0, b1, b2, b3;
		for (int i=1; i<loc_t::size; ++i) {
			b0.push_back(loc_t( i,  i));
			b1.push_back(loc_t( i, -i));
			b2.push_back(loc_t(-i,  i));
			b3.push_back(loc_t(-i, -i));
		}

		b.push_back(b0);
		b.push_back(b1);
		b.push_back(b2);
		b.push_back(b3);
		m_paths.insert(std::make_pair('b', b));

		/*
		 * queen
		 */
		path_list_t q;
		path_t q0, q1, q2, q3, q4, q5, q6, q7;
		for (int i=1; i<loc_t::size; ++i) {
			q0.push_back(loc_t( i, i));
			q1.push_back(loc_t( i,-i));
			q2.push_back(loc_t(-i, i));
			q3.push_back(loc_t(-i,-i));
			q4.push_back(loc_t( 0, i));
			q5.push_back(loc_t( 0,-i));
			q6.push_back(loc_t( i, 0));
			q7.push_back(loc_t(-i, 0));
		}
		q.push_back(q0);
		q.push_back(q1);
		q.push_back(q2);
		q.push_back(q3);
		q.push_back(q4);
		q.push_back(q5);
		q.push_back(q6);
		q.push_back(q7);
		m_paths.insert(std::make_pair('q', q));

		/*
		 * king
		 */
		path_list_t k;
		path_t k0, k1, k2, k3, k4, k5, k6, k7;
		k0.push_back(loc_t( 1, 1));
		k1.push_back(loc_t( 1,-1));
		k2.push_back(loc_t(-1, 1));
		k3.push_back(loc_t(-1,-1));
		k4.push_back(loc_t( 0, 1));
		k5.push_back(loc_t( 0,-1));
		k6.push_back(loc_t( 1, 0));
		k7.push_back(loc_t(-1, 0));
		k.push_back(k0);
		k.push_back(k1);
		k.push_back(k2);
		k.push_back(k3);
		k.push_back(k4);
		k.push_back(k5);
		k.push_back(k6);
		k.push_back(k7);
		m_paths.insert(std::make_pair('k', k));

		/*
		 * knight
		 */
		path_list_t n;
		path_t n0, n1, n2, n3, n4, n5, n6, n7;
		n0.push_back(loc_t( 1, 2));
		n1.push_back(loc_t( 1,-2));
		n2.push_back(loc_t(-1, 2));
		n3.push_back(loc_t(-1,-2));
		n4.push_back(loc_t( 2, 1));
		n5.push_back(loc_t( 2,-1));
		n6.push_back(loc_t(-2, 1));
		n7.push_back(loc_t(-2,-1));
		n.push_back(n0);
		n.push_back(n1);
		n.push_back(n2);
		n.push_back(n3);
		n.push_back(n4);
		n.push_back(n5);
		n.push_back(n6);
		n.push_back(n7);
		m_paths.insert(std::make_pair('n', n));
	}

	bool in_check(const board_t& offence, const board_t& defence) const
	{
		for (board_t::const_iterator i=offence.begin();
				 i != offence.end(); ++i) {
			char ch = tolower(i->second);
			loc_t loc = i->first;
			assert(m_paths.end() != m_paths.find(ch));
			path_list_t pl = m_paths.find(ch)->second;
			for (path_list_t::const_iterator pli=pl.begin(); pli!=pl.end(); ++pli) {
				for (path_t::const_iterator pi=pli->begin(); pi != pli->end(); ++pi) {
					loc_t here = (loc + *pi);
					if (!here.inside()) {
						break;
					}

					board_t::const_iterator oi = offence.find(here);
					if (oi != offence.end()) {
						break; // guarded
					}

					loc_t there = here.opposite();
					board_t::const_iterator di = defence.find(there);
					if (di != defence.end()) {
						char dch = tolower(di->second);
						if (dch == 'k') {
							return true;
						} else {
							break; // guarded
						}
					}
				}
			}
		}

		return false;
	}

private:
	path_map_t m_paths;
};

class game_t
{
public:
	board_t m_whites;
	board_t m_blacks;

	bool empty() const { return m_whites.empty() && m_blacks.empty(); }

	void dump(std::ostream& out, const board_t& b)
	{
		for (int i=0; i<loc_t::size; ++i) {
			for (int j=0; j<loc_t::size; ++j) {
				board_t::const_iterator p = b.find(loc_t(j, i));
				if (b.end() != p)  {
					out << p->second;
				} else {
					out << ".";
				}
			}
			out << std::endl;
		}
		out << std::endl;
	}

	void dump(std::ostream& out)
	{
		out << "white" << std::endl;
		dump(out, m_whites);
		out << "black" << std::endl;
		dump(out, m_blacks);
	}

};

game_t read_game(std::istream& in)
{
	game_t ret;

	for (size_t i=0; i<loc_t::size; ++i) {
		std::string line = read_line(in);
		assert(line.size() == loc_t::size);

		for (size_t x=0; x<line.size(); ++x) {
			char ch = line[x];
			side_e side = to_side(ch);
			switch (side) {
			case side_white:
				ret.m_whites[loc_t(int(x), int(i)).opposite()] =  ch;
				break;
			case side_black:
				ret.m_blacks[loc_t(int(x), int(i))] = ch;
				break;
			default:
				break;
			}
		}
	}

	return ret;
}

int main()
{
	judgement_t judge;
	size_t ngame = 0;
	do {
		game_t g = read_game(std::cin);
		if (g.empty()) {
			break;
		}

		ngame++;

#if 0
		std::cout << "read:" << std::endl;
		g.dump(std::cout);
#else
		std::cout << "Game #" << ngame << ": ";
		if (judge.in_check(g.m_whites, g.m_blacks)) {
			std::cout << "black king is in check." << std::endl;
		} else if (judge.in_check(g.m_blacks, g.m_whites)) {
			std::cout << "white king is in check." << std::endl;
		} else {
			std::cout << "no king is in check." << std::endl;
		}
#endif
		read_line(std::cin); // skip blank
	} while (true);
	return 0;
}
