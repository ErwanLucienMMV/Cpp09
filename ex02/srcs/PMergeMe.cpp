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
            // first wins
            first->looser = second;

            first->defeated.push_back(second);

            // absorb second's history
            for (std::vector<VElement*>::iterator it = second->defeated.begin();
                 it != second->defeated.end();
                 ++it)
            {
                first->defeated.push_back(*it);
            }

            winners.push_back(first);
        }
        else
        {
            // second wins
            second->looser = first;

            second->defeated.push_back(first);

            // absorb first's history
            for (std::vector<VElement*>::iterator it = first->defeated.begin();
                 it != first->defeated.end();
                 ++it)
            {
                second->defeated.push_back(*it);
            }

            winners.push_back(second);
        }
    }


    // odd element
    if (current.size() % 2)
        winners.push_back(current.back());

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

void PmergeMe::extractChain(
    VElement* winner,
    std::vector<VElement*>& mainChain,
    std::vector<VElement*>& pending)
{
    if (!winner)
        return;


    mainChain.push_back(winner);


    if (winner->looser)
        pending.push_back(winner->looser);


    for (std::vector<VElement*>::iterator it = winner->defeated.begin();
         it != winner->defeated.end();
         ++it)
    {
        if (*it != winner->looser)
            pending.push_back(*it);
    }
}

std::vector<std::size_t> PmergeMe::jacobsthalOrder(std::size_t size)
{
    std::vector<std::size_t> order;

    if (size == 0)
        return order;

    std::vector<std::size_t> jacob;

    jacob.push_back(0);
    jacob.push_back(1);

    while (jacob.back() < size)
    {
        std::size_t n = jacob.size();

        jacob.push_back(jacob[n - 1] + 2 * jacob[n - 2]);
    }


    std::size_t previous = 1;

    for (std::size_t i = 2; i < jacob.size(); i++)
    {
        std::size_t limit = jacob[i];

        while (limit > previous && limit <= size)
        {
            order.push_back(limit - 1);
            limit--;
        }

        previous = jacob[i];
    }

    return order;
}

void PmergeMe::insertSorted(std::vector<VElement*>& chain, VElement* element)
{
    std::vector<VElement*>::iterator pos = chain.begin();


    while (pos != chain.end() &&
           *(*pos)->value < *element->value)
    {
        ++pos;
    }

    chain.insert(pos, element);
}

void PmergeMe::fordJohnsonInsert(std::vector<VElement*>& mainChain, std::vector<VElement*>& pending)
{
    if (pending.empty())
        return;


    insertSorted(mainChain, pending[0]);


    std::vector<std::size_t> order =
        jacobsthalOrder(pending.size());


    for (std::size_t i = 0; i < order.size(); i++)
    {
        std::size_t index = order[i];


        if (index < pending.size())
        {
            insertSorted(mainChain, pending[index]);
        }
    }
}

void PmergeMe::buildResult(std::vector<VElement*>& mainChain)
{
    this->resVec.clear();

    for (std::vector<VElement*>::iterator it = mainChain.begin();
         it != mainChain.end();
         ++it)
    {
        this->resVec.push_back(*((*it)->value));
    }
}

bool isSorted(const std::vector<int>& vec)
{
    if (vec.size() < 2)
        return true;

    for (std::size_t i = 0; i + 1 < vec.size(); i++)
    {
        if (vec[i] > vec[i + 1])
        {
            std::cout << "BROKEN AT INDEX "
                      << i
                      << ": "
                      << vec[i]
                      << " > "
                      << vec[i + 1]
                      << std::endl;

            return false;
        }
    }

    return true;
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
	{
	clock_t start = clock();

	std::vector<VElement> pairs(this->Vec.size());

	std::vector<int>::const_iterator vecIt = this->Vec.begin();

    for (std::size_t i = 0; i < this->Vec.size(); i++, vecIt++)
    {
        pairs[i].value = vecIt;
		pairs[i].looser = NULL;
		pairs[i].defeated.clear();
    }
	std::vector<VElement*> current;
	for (std::size_t i = 0; i < pairs.size(); i++)
	{
    	current.push_back(&pairs[i]);
	}
	VElement* winner = TournamentBracket(current);
	std::vector<VElement*> mainChain;
	std::vector<VElement*> pending;

	extractChain(winner, mainChain, pending);
	fordJohnsonInsert(mainChain, pending);
	buildResult(mainChain);
	std::cout << "Sorted result: " << resVec << std::endl;
	if (isSorted(resVec))
    	std::cout << "Vector is sorted correctly" << std::endl;
	else
    	std::cout << "ERROR: Vector is not sorted" << std::endl;
    clock_t end = clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;

	std::cout << "Time to sort: " << elapsed * 1000000 << " us" << std::endl;
	}
	//List sorting
	clock_t start = clock();
	
	clock_t end = clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;

	std::cout << "Time to sort: " << elapsed * 1000000 << " us" << std::endl;
}