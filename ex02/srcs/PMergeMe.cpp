#include "PMergeMe.hpp"

#include <ctime>
#include <iostream>

//Orthodox canonical form part

PmergeMe::PmergeMe()
    : Vec(), Lis(), resVec(), resLis() {}

PmergeMe::PmergeMe(const std::vector<int>& vec, const std::list<int>& lis)
    : Vec(vec), Lis(lis), resVec(), resLis() {}

PmergeMe::PmergeMe(const PmergeMe& other)
    : Vec(other.Vec), Lis(other.Lis), resVec(other.resVec), resLis(other.resLis) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        resVec = other.resVec;
        resLis = other.resLis;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

//Sorting part

VElement *PmergeMe::TournamentBracket()
{
	std::vector<VElement> pairs(this->Vec.size());

	std::vector<int>::const_iterator vecIt = this->Vec.begin();

    for (std::size_t i = 0; i < this->Vec.size(); i++, vecIt++)
    {
        pairs[i].value = vecIt;
    }

	return (NULL); //Temp
}

void PmergeMe::sort()
{
    std::cout << "Before: ";
    for (std::vector<int>::const_iterator it = Vec.begin(); it != Vec.end(); ++it) {
        if (it != Vec.begin()) {
            std::cout << ' ';
        }
        std::cout << *it;
    }
    std::cout << std::endl;
	std::time_t now = std::time(NULL);
    (void)now;
}