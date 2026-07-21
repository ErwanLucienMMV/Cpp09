#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <list>
#include <vector>

struct VElement
{
	std::vector<int>::const_iterator value;
	VElement* winner;
    std::vector<VElement*> defeated;
} ;

struct LElement
{
	const std::list<int>::iterator value;
    std::list<LElement*> defeated;
} ;

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const std::vector<int>& vec, const std::list<int>& lis);
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void sort();

private:
    const std::vector<int> Vec;
    const std::list<int> Lis;
    std::vector<int> resVec;
    std::list<int> resLis;

	std::vector<VElement*> PmergeMe::Pairing(std::vector<VElement*>& current);
	VElement* TournamentBracket(std::vector<VElement*>& current);
};

#endif
