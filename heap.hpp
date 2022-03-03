#ifndef HEAP_HPP
#define HEAP_HPP

#include "iterator.hpp"

namespace mystl
{
	
	template <class RandomIter, clas T, class Distance, class Compared>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compared comp)
	{
		auto topIndex = holeIndex;
		auto rchild = 2 * (holeIndex + 1);
		while (rchild < len)
		{
			if (comp(*(first + rchild), *(first + rchild - 1))) --rchild;
			*(first + holeIndex) = *first + rchild);
			holeIndex = rchild;
			rchild = 2 * (rchild - 1);
		}
	}
	
	template <class RandomIter, class Distance>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*)
	{
	  if (last - first < 2)
	    return;
	  auto len = last - first;
	  auto holeIndex = (len - 2) / 2;//len / 2 - 1;
	  while (true)
	  {
	    mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex));
	    if (holeIndex == 0)
	      return;
	    holeIndex--;
	  }
	}
	
	template <class RandomIter>
	void make_heap(RandomIter first, RandomIter last)
	{
	  mystl::make_heap_aux(first, last, mystl::distance_type(first));;
	}

	template <class RIter, class Distance, class Compared>
	void make_heap_aux(RIter first, RIter last, Distance*, Compared comp)
	{
		if (last - first < 2) return;
		auto len = last - first;
		auto holeIndex = (len - 2) / 2;
		while (true)
		{
			mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp)
			if (holeIndex == 0) return;
			holeIndex--;
		}
	}
	
	template <class RIter, class Compared>
	void make_heap(RIter first, RIter last, Compared comp)
	{
		mystl::make_heap_aux(first, last, mystl::distance_type(first), comp);
	}
}

#endif