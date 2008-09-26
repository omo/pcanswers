
// http://www.programming-challenges.com/pg.php?page=downloadproblem&probid=110103&format=html

#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

#define LINE_BUF_LEN (1024*16)

typedef std::vector<int> int_vector_t;

std::string read_line(std::istream& in)
{
	static char buf[LINE_BUF_LEN+1];
	in.getline(buf, LINE_BUF_LEN);
	return std::string(buf);
}

int to_cents(const std::string& in)
{
	int d, c;
	char ch;
	std::stringstream s(in);
	s >> d >> ch >> c;
	return d*100 + c;
}

std::string to_dollars(int cents)
{
	std::stringstream s;
	s << cents / 100 << "." ;
	s << (cents%100)/10;
	s << (cents%100)%10;
	return s.str();
}

int accumulate(const int_vector_t& vec)
{
	int n = 0;
	for (int_vector_t::const_iterator i=vec.begin();
			 i != vec.end(); ++i)	{
		n += *i;
	}
	return n;
}

int to_exchange_slow(const int_vector_t& spents)
{
	if (spents.empty()) {
		return 0;
	}

	int total = accumulate(spents);
	float avg = float(total)/float(spents.size());

	int moved = 0;
	int_vector_t balancing = spents;
	do {
		std::sort(balancing.begin(), balancing.end());
		int min = balancing[0];
		int max = balancing[balancing.size()-1];
		if (max - min <= 1) {
			break;
		}

		int diff = std::max(1, static_cast<int>(static_cast<float>(max) - avg));
		balancing[0] += diff;
		balancing[balancing.size()-1] -= diff;
		moved += diff;
	} while (true);
	
	return moved;
}

int to_exchange(const int_vector_t& spents)
{
	int total = accumulate(spents);
	float avg = float(total)/float(spents.size());
	float avg_frac  = avg - static_cast<float>(static_cast<int>(avg));
	int avg_floor = static_cast<int>(avg);
	int avg_ceil  = avg_floor + ((0.0 < avg_frac) ? 1 : 0);

	int nover = 0;
	int nshort = 0;
	for (int_vector_t::const_iterator i=spents.begin();
			 i != spents.end(); ++i) {
		nover  += std::max(0, *i - avg_ceil);
		nshort += std::max(0, avg_floor - *i);
	}

	return nover + std::max(0, nshort - nover);
}

int main()
{
	do {
		int n;
		std::vector<int> spents;		

		std::string head = read_line(std::cin);
		std::stringstream h(head);
		h >> n;
		if (0 ==n) {
			break;
		}

		for (int i=0; i<n; ++i) {
			spents.push_back(to_cents(read_line(std::cin)));
		}
		
		std::cout << "$" << to_dollars(to_exchange(spents)) << std::endl;;
	} while (true);

	return 0;
}
