#pragma once

#define EPS 1.0e-08
//  Utility for filling data
template<class CONT, class T, class IT = CONT::iterator>
//  Returns filled container 
//      has all original points
//      plus additional ones if requested
//      plus additional ones so maxDx is not exceeded
//  Original container and addPOints must be sorted
//  Returned container is sorted
inline CONT
fillData(
    //  The original data, sorted
    const CONT&                     original,
    //  The maximum spacing allowed
    const T&                        maxDx,
    //  Specific points to add, by iterator, sorted
    IT                              addBegin = IT(),
    IT                              addEnd = IT(),
    //  Minimum distance for equality
    const T&                        minDx = T(0.0))
{
    //  Results
    CONT filled;

    //  Add points?
    CONT added;
    const size_t addPoints = distance(addBegin, addEnd);
    if (addPoints > 0)
    {
        set_union(
            original.begin(),
            original.end(),
            addBegin,
            addEnd,
            back_inserter(added),
            [minDx](const T x, const T y) { return x < y - minDx; });
    }
    const CONT& sequence = addPoints > 0 ? added : original;

    //  Position on the start, add it
    auto it = sequence.begin();
    filled.push_back(*it);
    ++it;

    while (it != sequence.end())
    {
        auto current = filled.back();
        auto next = *it;
        //  Must supplement?
        if (next - current > maxDx)
        {
            //  Number of points to add
            int addPoints = int((next - current) / maxDx - EPS) + 1;
            //  Spacing between supplementary points
            auto spacing = (next - current) / addPoints;
            //  Add the steps
            auto t = current + spacing;
            while (t < next)
            {
                filled.push_back(t);
                t += spacing;
            }
        }
        //  Push the next step on the product timeline and advance
        filled.push_back(*it);
        ++it;
    }

    return filled;
}
