
// http://www.programming-challenges.com/pg.php?page=downloadproblem&probid=110107&format=html

#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <list>
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

int read_int(std::istream& in)
{
	return atoi(read_line(in).c_str());
}

struct rating_t
{
public:
	void add(size_t id) { m_list.push_back(id); } 

	std::list<size_t> m_list;
};

struct election_t
{
public:
	election_t() {}
	
	void add_rating(const rating_t& r)
	{
		m_ratings.push_back(r);
	}

	void add_candidate(const std::string name)
	{
		m_candidates.insert(std::make_pair(m_candidates.size()+1, name));
	}

	void elect()
	{

		while (true) {

			std::map<size_t, size_t> votes;
			for (std::map<size_t, std::string>::const_iterator ci = m_candidates.begin();
					 ci != m_candidates.end(); ++ci) {
				votes.insert(std::make_pair(ci->first, 0U));
			}

			for (std::vector<rating_t>::const_iterator ri=m_ratings.begin();
					 ri != m_ratings.end(); ++ri) {
				size_t who = ri->m_list.front();

				std::map<size_t, size_t>::iterator v = votes.find(who);
				assert(votes.end() != v);
				v->second++;
			}
		

			for (std::map<size_t, size_t>::iterator vi=votes.begin();
					 vi != votes.end(); ++vi) {
				if ((m_ratings.size()/2) < vi->second) {
					m_elected.push_back(vi->first);
					return;
				}
			}

			size_t lowerst = m_ratings.size();
			for (std::map<size_t, size_t>::const_iterator vi=votes.begin();
					 vi !=votes.end(); ++vi) {
				//std::cout << "vote:" << lowerst << "," << vi->first << "->" << vi->second << " " << m_candidates[vi->first] << std::endl;
				lowerst = std::min(lowerst, vi->second);
			}

			std::vector<size_t> losers;
			for (std::map<size_t, size_t>::const_iterator vi=votes.begin();
					 vi !=votes.end(); ++vi) {
				if (lowerst == vi->second) {
					losers.push_back(vi->first);
					//std::cout << "loser:" << vi->first << "|" << vi->second << " " << m_candidates[vi->first] << std::endl;
				}
			}

			if (losers.size() == m_candidates.size()) { // tied
				m_elected = losers;
				return;
			}

			for (std::vector<size_t>::const_iterator li = losers.begin();
					 li != losers.end(); ++li) {
				for (std::vector<rating_t>::iterator ri=m_ratings.begin();
						 ri != m_ratings.end(); ++ri) {
					assert(!ri->m_list.empty());
					ri->m_list.erase(std::find(ri->m_list.begin(), ri->m_list.end(), *li));
				}

				m_candidates.erase(*li);
			}

		}

		return;
	}

	std::vector<rating_t> m_ratings;
	std::map<size_t, std::string> m_candidates;
	std::vector<size_t> m_elected;
};

int main()
{
	size_t nels = read_int(std::cin);
	read_line(std::cin); // skip brank

	for (size_t ei=0; ei<nels; ++ei) {

		if (0 < ei) {
			std::cout << std::endl;
		}

		election_t el;
		size_t ncands =  read_int(std::cin);
		
		for (size_t ci=0; ci<ncands; ++ci) {
			el.add_candidate(read_line(std::cin));
		}

		do {
			std::string rline = read_line(std::cin);
			if (rline.empty()) {
				break;
			}

			std::stringstream rin(rline);
			rating_t r;

			for (size_t ri=0; ri<ncands; ++ri) {
				int x = 0;
				rin >> x;
				r.add(x);
			}

			el.add_rating(r);
		} while (true);

		el.elect();
		if (!el.m_elected.empty()) {
			for (std::vector<size_t>::const_iterator ei=el.m_elected.begin();
					 ei != el.m_elected.end(); ++ei) {
				std::cout << el.m_candidates[*ei] << std::endl;
			}
		}

	}

	return 0;
}
