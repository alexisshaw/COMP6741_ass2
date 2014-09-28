//Combination.h
//Generates a new combination of the underlying object
//The object must have a begin and end methods;
#include <utility>
#include <vector>

#pragma once

template <typename T>
class combination {
public:
    typedef std::vector<T> TVect;
    typedef decltype(std::declval<TVect>().begin()) TVectIt;

    TVect *baseContainer;
    size_t r;
    size_t n;

    class combinationIterator{
    public:
        class combinationResult
        {
        public:
            combinationIterator* parent;

            combinationResult() = default;

            combinationResult(combinationIterator* p);

            TVectIt begin();
            TVectIt end();

            std::vector<int> c(){
                return parent->c;
            };

            T lastOut();
            T lastIn();
        };

        combinationIterator(combination * p, bool isEnd);
        combinationResult operator*();
        bool operator!=(const combinationIterator& other) const;
        const combinationIterator& operator++();

    private:
        std::vector<int> c;
        std::vector<int> e;
        std::vector<int> Last;

        bool end = false;


        combination * parent;

    public:
        TVect combinationValue;
    private:
        T lastOut;
        T lastIn;

        int t=0;
        int r=0;
        int j=0;
        int M0 = 0;
        int Mj = 0;
        int S = 0;
        int n=0;
    };

    combination(TVect& base, size_t choose);
    combinationIterator begin();
    combinationIterator end();

};

template <typename T>
typename combination<T>::combinationIterator const& combination<T>::combinationIterator::operator++()
{
    if(end) return *this;
    if((n >= r) || (j == 0)) {
        end = true;
        return *this;
    }

    S= c[j];
    Mj = M0 + j;
    e[n+1] = n;

    lastOut = combinationValue[j-1];
    if ((j%2) == 1){
        if(c[j] == Mj){
            combinationValue[j-1] = (*parent->baseContainer)[c[j-1]];
            c[j] = c[j-1] + 1;
            Last[j+1] = c[j] + 1;
        } else {
            combinationValue[j-1] = (*parent->baseContainer)[c[j]];
            c[j] = c[j] + 1;
        }
    } else {
        if(c[j] == (c[j-1] + 1)){
            combinationValue[j-1] = (*parent->baseContainer)[Mj - 1];
            c[j] = Mj;
        } else {
            Last[j+1] = c[j];
            combinationValue[j-1] = (*parent->baseContainer)[c[j] - 2];
            c[j] = c[j] - 1;
        }
    }
    lastIn = combinationValue[j-1];

    if (c[j] == Last[j]) {
        Last[j] = S;
        e[j+1] = e[j];
        e[j] = j-1;
    }

    if ((j < n) && (c[j] == Mj)){
        t = j;
        j = e[t+1];
        e[t+1] = t;
    } else {
        if (t == j){
            t++;
        }
        if (t < e[n+1]){
            j = t;
        } else{
            j = e[n+1];
        }
    }

    return *this;
}

template <typename T>
combination<T>::combination(TVect& base, size_t choose):baseContainer(&base), n(choose)
{
    r = baseContainer->end() - baseContainer->begin();
}

template <typename T>
typename combination<T>::combinationIterator combination<T>::begin()
{
    if (r >= n) {
        return combinationIterator(this, false);
    } else {
        return end();
    }
}

template <typename T>
typename combination<T>::combinationIterator combination<T>::end()
{
    return combinationIterator(this, true);
}

template <typename T>
combination<T>::combinationIterator::combinationIterator(combination<T> * p, bool isEnd): parent(p)
{
    end = isEnd;
    if (isEnd)
    {
        n = p->n;
        r = p->r;
    } else {
        n = p->n;
        r = p->r;

        c.resize((unsigned long)n+5);
        e.resize((unsigned long)n+5);
        Last.resize((unsigned long)n+5);   //positive going array
        combinationValue.resize((unsigned long)n);

        M0   = r - n;
        t    = n + 1;
        c[0] = 0;

        j = 0;
        do {
            j++;
            c[j] = j;
            e[j] = j - 1;
            if ((j % 2) == 1) {
                Last[j] = M0 + j;
            } else {
                Last[j] = j + 1;
            }
        } while(j!= n);

        for (int i=0 ; i < n; i++ ){
            combinationValue[i] = (*parent->baseContainer)[i];
        }

        lastIn = combinationValue[0];
        lastOut = combinationValue[0];
    }
}

template <typename T>
typename combination<T>::combinationIterator::combinationResult combination<T>::combinationIterator::operator*()
{
    return combinationResult(this);
}

template <typename T>
bool combination<T>::combinationIterator::operator!=(combinationIterator const& other) const
{
    return  (end != other.end)||
            parent != other.parent ||
            n != other.n ||
            !end && (lastIn != other.lastIn ||
                lastOut != other.lastOut ||
                !std::equal(c.begin(), c.end(), other.c.begin()));
}

template <typename T>
combination<T>::combinationIterator::combinationResult::combinationResult(combinationIterator* p): parent(p)
{}

template <typename T>
typename combination<T>::TVectIt combination<T>::combinationIterator::combinationResult::begin()
{ return (parent->combinationValue).begin(); }

template <typename T>
typename combination<T>::TVectIt combination<T>::combinationIterator::combinationResult::end()
{ return (parent->combinationValue).end(); }

template <typename T>
T combination<T>::combinationIterator::combinationResult::lastOut()
{ return parent->lastOut; }

template <typename T>
T combination<T>::combinationIterator::combinationResult::lastIn()
{ return parent->lastIn; }
