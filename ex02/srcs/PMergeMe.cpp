#include "PMergeMe.hpp"

#include <ctime>
#include <iostream>
#include <cmath>

//Orthodox canonical form part

PmergeMe::PmergeMe()
    : Vec(), deq(), resVec(), resdeq() {}

PmergeMe::PmergeMe(const std::vector<int>& vec, const std::deque<int>& lis)
    : Vec(vec), deq(lis), resVec(), resdeq() {
    }

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

int compA = 0;

struct Item
{
    int value;
    int id;

    Item() : value(0), id(-1) {}
    Item(int v, int i) : value(v), id(i) {}
};

struct Pair
{
    Item low;
    Item high;

    Pair(const Item& first, const Item& second)
    {
        compA+=1;
        if (first.value <= second.value)
        {
            low = first;
            high = second;
        }
        else
        {
            low = second;
            high = first;
        }
    }
};

static int lowerBound(const std::vector<Item>& values, int end, int value)
{
    int left = 0;
    int right = end;

    while (left < right)
    {
        compA +=1;
        int middle = left + (right - left) / 2;

        if (values[middle].value < value)
            left = middle + 1;
        else
            right = middle;
    }
    return left;
}

static int findPositionById(const std::vector<Item>& values, int id)
{
    std::size_t i = 0;

    while (i < values.size())
    {
        if (values[i].id == id)
            return static_cast<int>(i);
        ++i;
    }
    return -1;
}

static Item findLowForHigh(const std::vector<Pair>& pairs, int highId)
{
    std::size_t i = 0;

    while (i < pairs.size())
    {
        if (pairs[i].high.id == highId)
            return pairs[i].low;
        ++i;
    }

    // never reached just for the flags
    return Item(0, -1);
}

static void insertBeforePartner(std::vector<Item>& chain,
                                const Item& item,
                                int partnerId)
{
    int partnerPosition = findPositionById(chain, partnerId);
    int insertPosition;

    if (partnerPosition < 0)
        partnerPosition = static_cast<int>(chain.size());

    insertPosition = lowerBound(chain, partnerPosition, item.value);
    chain.insert(chain.begin() + insertPosition, item);
}

static std::vector<Item> fordJohnsonItems(const std::vector<Item>& input)
{
    std::vector<Pair> pairs;
    std::vector<Item> maxima;
    std::vector<Item> sortedMaxima;
    std::vector<Item> chain;
    bool hasOdd;
    Item odd;
    std::size_t pairCount;
    std::size_t i;

    if (input.size() <= 1)
        return input;

    hasOdd = (input.size() % 2 != 0);
    pairCount = input.size() / 2;

    if (hasOdd)
        odd = input[input.size() - 1];

    // pairing
    for (i = 0; i < pairCount; ++i)
    {
        Pair pair(input[i * 2], input[i * 2 + 1]);

        pairs.push_back(pair);
        maxima.push_back(pair.high);
    }

    // Recursively sort the maxima.
    sortedMaxima = fordJohnsonItems(maxima);

    // Begin with b1, a1.
   // Start with b1, followed by ALL sorted maxima: b1, a1, a2 cause it will be my two base bounds
chain.push_back(findLowForHigh(pairs, sortedMaxima[0].id));

for (i = 0; i < sortedMaxima.size(); ++i)
{
    chain.push_back(sortedMaxima[i]);
}
    {
        std::size_t previousBoundary = 1;
        std::size_t boundary = 3;

        while (previousBoundary < pairCount)
        {
            std::size_t current = boundary;

            if (current > pairCount)
                current = pairCount;

            while (current > previousBoundary)
            {
                Item high = sortedMaxima[current - 1];
                Item low = findLowForHigh(pairs, high.id);

                insertBeforePartner(chain, low, high.id);
                --current;
            }

            previousBoundary = boundary;
            boundary = boundary * 2 + 1;
        }
    }

    if (hasOdd)
    {
        int position = lowerBound(
            chain,
            static_cast<int>(chain.size()),
            odd.value
        );

        chain.insert(chain.begin() + position, odd);
    }

    return chain;
}

void fordJohnsonSort(const std::vector<int>& values, std::vector<int>& resVec)
{
    std::vector<Item> items;
    std::vector<Item> sorted;
    std::size_t i;

    for (i = 0; i < values.size(); ++i)
        items.push_back(Item(values[i], static_cast<int>(i)));

    sorted = fordJohnsonItems(items);

	for (i = 0; i < values.size(); ++i) 
		resVec.push_back(sorted[i].value);
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

static int lowerBound(const std::deque<Item>& values, int end, int value)
{
    int left = 0;
    int right = end;

    while (left < right)
    {
        int middle = left + (right - left) / 2;

        if (values[middle].value < value)
            left = middle + 1;
        else
            right = middle;
    }
    return left;
}

static int findPositionById(const std::deque<Item>& values, int id)
{
    std::size_t i = 0;

    while (i < values.size())
    {
        if (values[i].id == id)
            return static_cast<int>(i);
        ++i;
    }
    return -1;
}

static Item findLowForHigh(const std::deque<Pair>& pairs, int highId)
{
    std::size_t i = 0;

    while (i < pairs.size())
    {
        if (pairs[i].high.id == highId)
            return pairs[i].low;
        ++i;
    }
    return Item(0, -1);
}

static void insertBeforePartner(std::deque<Item>& chain,
                                const Item& item,
                                int partnerId)
{
    int partnerPosition = findPositionById(chain, partnerId);
    int insertPosition;

    if (partnerPosition < 0)
        partnerPosition = static_cast<int>(chain.size());

    insertPosition = lowerBound(chain, partnerPosition, item.value);
    chain.insert(chain.begin() + insertPosition, item);
}

static std::deque<Item> fordJohnsonItems(const std::deque<Item>& input)
{
    std::deque<Pair> pairs;
    std::deque<Item> maxima;
    std::deque<Item> sortedMaxima;
    std::deque<Item> chain;
    bool hasOdd;
    Item odd;
    std::size_t pairCount;
    std::size_t i;

    if (input.size() <= 1)
        return input;

    hasOdd = (input.size() % 2 != 0);
    pairCount = input.size() / 2;

    if (hasOdd)
        odd = input[input.size() - 1];

    // Create pairs where low.value <= high.value.
    for (i = 0; i < pairCount; ++i)
    {
        Pair pair(input[i * 2], input[i * 2 + 1]);

        pairs.push_back(pair);
        maxima.push_back(pair.high);
    }

    // Recursively sort all maximum values.
    sortedMaxima = fordJohnsonItems(maxima);

    /*
     * Initial chain:
     * b1, a1, a2, a3, ...
     */
    chain.push_back(findLowForHigh(pairs, sortedMaxima[0].id));

    for (i = 0; i < sortedMaxima.size(); ++i)
        chain.push_back(sortedMaxima[i]);

    /*
     * Insert remaining small values in Jacobsthal order:
     * 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...
     */
    {
        std::size_t previousBoundary = 1;
        std::size_t boundary = 3;

        while (previousBoundary < pairCount)
        {
            std::size_t current = boundary;

            if (current > pairCount)
                current = pairCount;

            while (current > previousBoundary)
            {
                Item high = sortedMaxima[current - 1];
                Item low = findLowForHigh(pairs, high.id);

                insertBeforePartner(chain, low, high.id);
                --current;
            }

            previousBoundary = boundary;
            boundary = boundary * 2 + 1;
        }
    }

    // Insert the unpaired value, if there is one.
    if (hasOdd)
    {
        int position = lowerBound(
            chain,
            static_cast<int>(chain.size()),
            odd.value
        );

        chain.insert(chain.begin() + position, odd);
    }

    return chain;
}

void fordJohnsonSort(const std::deque<int>& values, std::deque<int>& resDeq)
{
    std::deque<Item> items;
    std::deque<Item> sorted;
    std::size_t i;

    for (i = 0; i < values.size(); ++i)
        items.push_back(Item(values[i], static_cast<int>(i)));

    sorted = fordJohnsonItems(items);

    for (i = 0; i < values.size(); ++i)
    	resDeq.push_back(sorted[i].value);
}

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



unsigned long expectedComparisons(unsigned int n)
{
    unsigned long comparisons = 0;

    for (unsigned int k = 1; k <= n; ++k)
        comparisons += (unsigned long)std::ceil(std::log((3 * k) / 4.0) / std::log(2));

    return comparisons;
}

void PmergeMe::sort()
{
    std::cout << "Before: ";
    std::cout << Vec << std::endl;
	
	//Vec sorting
	{

	


	clock_t start = clock();
	fordJohnsonSort(Vec, resVec);
    clock_t end = clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	std::cout << "Sorted result: " << resVec << std::endl;
	std::cout << "Time to sort with vector: " << elapsed * 1000000 << " us" << std::endl;
	unsigned long numberofcomp = expectedComparisons(Vec.size());
	float percentdiff = ((double)compA / numberofcomp - 1) * 100;
	std::cout << "Number of comparison: " << compA << " expected at best: " << numberofcomp << std::endl;
	std::cout << "Actual diff with best possible result: " << percentdiff << "%" <<std::endl;
	}
	// //deque sorting
	{

	
	clock_t start = clock();
	fordJohnsonSort(deq, resdeq);
	clock_t end = clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;

	std::cout << "Time to sort with deque: " << elapsed * 1000000 << " us" << "\n" << resdeq << std::endl;
	}
}