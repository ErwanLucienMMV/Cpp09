#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <list>
#include <vector>
#include <iostream>

struct VElement
{
    std::vector<int>::const_iterator value;

    VElement* looser;

    std::vector<VElement*> defeated;
};

struct LElement
{
	const std::list<int>::iterator value;
	LElement* looser;
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

	std::vector<std::size_t> jacobsthalOrder(std::size_t size);
	std::vector<VElement*> Pairing(std::vector<VElement*>& current);
	void extractChain(VElement* node, std::vector<VElement*>& mainChain, std::vector<VElement*>& pending);
	void insertSorted(std::vector<VElement*>& chain, VElement* element);
	void fordJohnsonInsert(std::vector<VElement*>& mainChain, std::vector<VElement*>& pending);
	void buildResult(std::vector<VElement*>& mainChain);
	VElement* TournamentBracket(std::vector<VElement*>& current);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    for (typename std::vector<T>::const_iterator it = vec.begin();
         it != vec.end();
         ++it)
    {
        if (it != vec.begin())
            os << " ";

        os << *it;
    }

    return os;
}

#endif
