#ifndef _MYSTL_CONSTRUCT_H_
#define _MYSTL_CONSTRUCT_H_

#include "util.hpp"
#include <iterator>
#include <iostream>

namespace mystl
{
	//construct
	template <class Ty>
	void construct(Ty* ptr)
	{
		::new((void*)ptr) Ty();
		//std::cout << "construct" << std::endl;
	}
	
	template <class Ty1, class Ty2>
	void construct(Ty1* ptr, const Ty2& value)
	{
		::new ((void*)ptr) Ty1(value);
		//std::cout << "construct" << std::endl;
	}
	
	template <class Ty, class... Args>
	void construct(Ty* ptr, Args&&... args)
	{
		::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
		//std::cout << "construct" << std::endl;
	}
	
	//destroy
	template <class Ty>
	void destroy_one(Ty*, std::true_type){ std::cout << "inefficient destroy" << std::endl; };
	
	template <class Ty>
	void destroy_one(Ty* pointer, std::false_type)
	{
  		if (pointer != nullptr)
  		{
   			pointer->~Ty();
  		}
	}
	
	template <class ForwardIter>
	void destroy_cat(ForwardIter, ForwardIter, std::true_type){ std::cout << "inefficient destroy" << std::endl; }
	
	template <class ForwardIter>
	void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
	{
		for(; first != last; first++){
			destroy(&*first);
		}
	}
	
	template <class Ty>
	void destroy(Ty* pointer)
	{
		destroy_one(pointer, std::is_trivially_destructible<Ty>());
	}
	
	template <class ForwardIter>
	void destroy(ForwardIter first, ForwardIter last)
	{
		typedef std::is_trivially_destructible<typename std::iterator_traits<ForwardIter>::value_type> temp;
		destroy_cat(first, last, temp());
	}
}

#endif