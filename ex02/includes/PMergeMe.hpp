#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <list>
#include <vector>
#include <iostream>
#include <deque>
#include <algorithm>

struct VElement
{
    std::vector<int>::const_iterator value;
    VElement* looser;
    std::vector<VElement*> defeated;
};

struct QElement
{
	std::deque<int>::const_iterator value;
	QElement* looser;
    std::deque<QElement*> defeated;
} ;

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const std::vector<int>& vec, const std::deque<int>& lis);
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void sort();

private:
    const std::vector<int> Vec;
    const std::deque<int> deq;
    std::vector<int> resVec;
    std::deque<int> resdeq;

	std::vector<std::size_t> jacobsthalOrder(std::size_t size);
	std::vector<VElement*> Pairing(std::vector<VElement*>& current);
	void extractChain(VElement* node, std::vector<VElement*>& mainChain, std::vector<VElement*>& pending);
	void insertSorted(std::vector<VElement*>& chain, VElement* element);
	void fordJohnsonInsert(std::vector<VElement*>& mainChain, std::vector<VElement*>& pending);
	void buildResult(std::vector<VElement*>& mainChain);
	VElement* TournamentBracket(std::vector<VElement*>& current);

	std::deque<std::size_t> QjacobsthalOrder(std::size_t size);
	std::deque<QElement*> Pairing(std::deque<QElement*>& current);
	void extractChain(QElement* node, std::deque<QElement*>& mainChain, std::deque<QElement*>& pending);
	void insertSorted(std::deque<QElement*>& chain, QElement* element);
	void fordJohnsonInsert(std::deque<QElement*>& mainChain, std::deque<QElement*>& pending);
	void buildResult(std::deque<QElement*>& mainChain);
	QElement* TournamentBracket(std::deque<QElement*>& current);
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
	os << "\nNombre d'elements: " << vec.size();
    return os;
}

#endif
