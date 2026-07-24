#include "PMergeMe.hpp"

#include <ctime>
#include <iostream>

//Orthodox canonical form part

PmergeMe::PmergeMe()
    : Vec(), deq(), resVec(), resdeq() {}

PmergeMe::PmergeMe(const std::vector<int>& vec, const std::deque<int>& lis)
    : Vec(vec), deq(lis), resVec(), resdeq() {}

PmergeMe::PmergeMe(const PmergeMe& other)
    : Vec(other.Vec), deq(other.deq), resVec(other.resVec), resdeq(other.resdeq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        resVec = other.resVec;
        resdeq = other.resdeq;
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
			second->winner = first;

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
			first->winner = second;

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
    order.push_back(0);
	
    std::vector<std::size_t> jacob;

    jacob.push_back(1);
    jacob.push_back(3);

    while (jacob.back() < size)
    {
        std::size_t n = jacob.size();
        jacob.push_back(jacob[n - 1] + 2 * jacob[n - 2]);
    }


    std::size_t previous = 1;

    for (std::size_t i = 1; i < jacob.size(); ++i)
    {
        std::size_t current = jacob[i];

        if (current > size)
            current = size;

        // insérer en descendant dans le groupe
        for (std::size_t j = current; j > previous; --j)
        {
            order.push_back(j - 1);
        }

        previous = current;

        if (previous == size)
            break;
    }

	//les elements qui disparaissaient
    for (std::size_t i = 0; i < size; ++i)
    {
        bool found = false;

        for (std::size_t j = 0; j < order.size(); ++j)
        {
            if (order[j] == i)
            {
                found = true;
                break;
            }
        }

        if (!found)
            order.push_back(i);
    }


    return order;
}

bool compareValue(VElement* a, VElement* b)
{
    return *(a->value) < *(b->value);
}

void PmergeMe::insertSorted(
    std::vector<VElement*>& chain,
    VElement* element)
{
    std::vector<VElement*>::iterator pos;

    pos = std::lower_bound(
        chain.begin(),
        chain.end(),
        element,
        compareValue
    );

    chain.insert(pos, element);
}

void PmergeMe::fordJohnsonInsert(
    std::vector<VElement*>& mainChain,
    std::vector<VElement*>& pending)
{
    std::vector<std::size_t> order =
        jacobsthalOrder(pending.size());

    for (std::size_t i = 0; i < order.size(); ++i)
    {
        insertSorted(mainChain, pending[order[i]]);
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

//deque sorting

bool isSorted(const std::deque<int>& deque)
{
    if (deque.size() < 2)
        return true;

    for (std::size_t i = 0; i + 1 < deque.size(); i++)
    {
        if (deque[i] > deque[i + 1])
        {
            std::cout << "BROKEN AT INDEX "
                      << i
                      << ": "
                      << deque[i]
                      << " > "
                      << deque[i + 1]
                      << std::endl;

            return false;
        }
    }

    return true;
}


std::deque<QElement*> PmergeMe::Pairing(std::deque<QElement*>& current)
{
    std::deque<QElement*> winners;

    for (std::size_t i = 0; i + 1 < current.size(); i += 2)
    {
        QElement* first = current[i];
        QElement* second = current[i + 1];


        if (*(first->value) >= *(second->value))
        {
            // first wins
            first->looser = second;
			second->winner = first;

            first->defeated.push_back(second);

            // absorb second's history
            for (std::deque<QElement*>::iterator it = second->defeated.begin();
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
			first->winner = second;

            second->defeated.push_back(first);

            // absorb first's history
            for (std::deque<QElement*>::iterator it = first->defeated.begin();
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

QElement* PmergeMe::TournamentBracket(std::deque<QElement*>& current)
{
	if (current.empty())
    	return NULL;

	if (current.size() == 2)
	{
    		std::deque<QElement*> winners = Pairing(current);
    		return winners[0];
	}
    else if (current.size() == 1)
        return current[0];

    std::deque<QElement*> winners = Pairing(current);

    return TournamentBracket(winners);
}

void PmergeMe::extractChain(
    QElement* winner,
    std::deque<QElement*>& mainChain,
    std::deque<QElement*>& pending)
{
    if (!winner)
        return;


    mainChain.push_back(winner);


    if (winner->looser)
        pending.push_back(winner->looser);


    for (std::deque<QElement*>::iterator it = winner->defeated.begin();
         it != winner->defeated.end();
         ++it)
    {
        if (*it != winner->looser)
            pending.push_back(*it);
    }
}

std::deque<std::size_t> PmergeMe::QjacobsthalOrder(std::size_t size)
{
    std::deque<std::size_t> order;

    if (size == 0)
        return order;
    order.push_back(0);
	
    std::deque<std::size_t> jacob;

    jacob.push_back(1);
    jacob.push_back(3);

    while (jacob.back() < size)
    {
        std::size_t n = jacob.size();
        jacob.push_back(jacob[n - 1] + 2 * jacob[n - 2]);
    }


    std::size_t previous = 1;

    for (std::size_t i = 1; i < jacob.size(); ++i)
    {
        std::size_t current = jacob[i];

        if (current > size)
            current = size;

        // insérer en descendant dans le groupe
        for (std::size_t j = current; j > previous; --j)
        {
            order.push_back(j - 1);
        }

        previous = current;

        if (previous == size)
            break;
    }

	//les elements qui disparaissaient
    for (std::size_t i = 0; i < size; ++i)
    {
        bool found = false;

        for (std::size_t j = 0; j < order.size(); ++j)
        {
            if (order[j] == i)
            {
                found = true;
                break;
            }
        }

        if (!found)
            order.push_back(i);
    }


    return order;
}

bool compareQValue(QElement* a, QElement* b)
{
    return *(a->value) < *(b->value);
}

void PmergeMe::insertSorted(
    std::deque<QElement*>& chain,
    QElement* element)
{
    std::deque<QElement*>::iterator upper;

    upper = std::find(
        chain.begin(),
        chain.end(),
        element->winner
    );

    std::deque<QElement*>::iterator pos =
        std::lower_bound(
            chain.begin(),
            upper,
            element,
            compareQValue
        );

    chain.insert(pos, element);
}

void PmergeMe::fordJohnsonInsert(
    std::deque<QElement*>& mainChain,
    std::deque<QElement*>& pending)
{
    std::deque<std::size_t> order =
        QjacobsthalOrder(pending.size());

    for (std::size_t i = 0; i < order.size(); ++i)
    {
        insertSorted(mainChain, pending[order[i]]);
    }
}

void PmergeMe::buildResult(std::deque<QElement*>& mainChain)
{
    this->resdeq.clear();

    for (std::deque<QElement*>::iterator it = mainChain.begin();
         it != mainChain.end();
         ++it)
    {
        this->resdeq.push_back(*((*it)->value));
    }
}

void PmergeMe::sort()
{
    std::cout << "Before: ";
    std::cout << Vec << std::endl;
	
	//Vec sorting
	{

	std::vector<VElement> pairs(this->Vec.size());

	std::vector<int>::const_iterator vecIt = this->Vec.begin();

    for (std::size_t i = 0; i < this->Vec.size(); i++, vecIt++)
    {
        pairs[i].value = vecIt;
		pairs[i].looser = NULL;
    }
	std::vector<VElement*> current;
	for (std::size_t i = 0; i < pairs.size(); i++)
	{
    	current.push_back(&pairs[i]);
	}
	std::vector<VElement*> mainChain;
	std::vector<VElement*> pending;


	clock_t start = clock();
	VElement* winner = TournamentBracket(current);
	extractChain(winner, mainChain, pending);
	fordJohnsonInsert(mainChain, pending);
	buildResult(mainChain);
    clock_t end = clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	std::cout << "Sorted result: " << resVec << std::endl;
	std::cout << "Time to sort with vector: " << elapsed * 1000000 << " us" << std::endl;
	}
	//deque sorting
	{

	std::deque<QElement> pairs(this->deq.size());

	std::deque<int>::const_iterator vecIt = this->deq.begin();

    for (std::size_t i = 0; i < this->deq.size(); i++, vecIt++)
    {
        pairs[i].value = vecIt;
		pairs[i].looser = NULL;
		pairs[i].winner = NULL;
    }
	std::deque<QElement*> current;
	for (std::size_t i = 0; i < pairs.size(); i++)
	{
    	current.push_back(&pairs[i]);
	}
	std::deque<QElement*> mainChain;
	std::deque<QElement*> pending;
	
	clock_t start = clock();
	QElement* winner = TournamentBracket(current);
	extractChain(winner, mainChain, pending);
	fordJohnsonInsert(mainChain, pending);
	buildResult(mainChain);
	clock_t end = clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;

	std::cout << "Time to sort with deque: " << elapsed * 1000000 << " us" << "\n" << resdeq << std::endl;
	}
}