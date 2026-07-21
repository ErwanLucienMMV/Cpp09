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

std::vector<VElement*> PmergeMe::Pairing(std::vector<VElement*>& current)
{
    std::vector<VElement*> winners;

    for (std::size_t i = 0; i + 1 < current.size(); i += 2)
    {
        VElement* first = current[i];
        VElement* second = current[i + 1];

        if (*(first->value) >= *(second->value))
        {
            // first defeated second or is strictly equal if I do not use sets
            first->defeated.push_back(second);
            second->winner = first;

            winners.push_back(first);
        }
        else
        {
            // second defeated first, so it should get swallowed and not who beat him
            second->defeated.push_back(first);
            first->winner = second;

            winners.push_back(second);
        }
    }

    // Odd element: no fight, no winner, the one nobody wants
    if (current.size() % 2 != 0)
    {
        winners.push_back(current.back());
    }

    return winners;
}

VElement* PmergeMe::TournamentBracket(std::vector<VElement*>& current)
{
	if (current.empty())
    	return NULL;

	if (current.size() == 2)
	{
    		std::vector<VElement*> winners = Pairing(current);
    		return winners[0];
	}
    else if (current.size() == 1)
        return current[0];

    std::vector<VElement*> winners = Pairing(current);

    return TournamentBracket(winners);
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
	
	//Vec sorting
	std::time_t now = std::time(NULL);

	std::vector<VElement> pairs(this->Vec.size());

	std::vector<int>::const_iterator vecIt = this->Vec.begin();

    for (std::size_t i = 0; i < this->Vec.size(); i++, vecIt++)
    {
        pairs[i].value = vecIt;
		pairs[i].winner = NULL;
		pairs[i].defeated.clear();
    }
	std::vector<VElement*> current;
	for (std::size_t i = 0; i < pairs.size(); i++)
	{
    	current.push_back(&pairs[i]);
	}
	VElement* winner = TournamentBracket(current);
    (void)now;
}