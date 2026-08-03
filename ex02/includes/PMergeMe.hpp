#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <list>
#include <vector>
#include <iostream>
#include <deque>
#include <algorithm>


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

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::deque<T>& vec)
{
    for (typename std::deque<T>::const_iterator it = vec.begin();
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
