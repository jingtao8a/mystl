#ifndef _MYTINY_UTIL_H
#define _MYTINY_UTIL_H

#include "type_traits.hpp"
#include <cstddef>

namespace mystl
{
	//left refer -> right refer
	template <class T>
	constexpr typename std::remove_reference<T>::type&& move(T&& arg)
	{
		return static_cast<typename std::remove_reference<T>::type&&> (arg);
	}
	
	template <class T>
	constexpr T&& forward(typename std::remove_reference<T>::type& arg)
	{
		return static_cast<T&&>(arg);
	}
	
	template <class T>
	constexpr T&& forward(typename std::remove_reference<T>::type&& arg)
	{
		return static_cast<T&&>(arg);
	}
	//swap
	template <class Tp>
	void swap(Tp& lhs, Tp& rhs)
	{
		auto tmp(mystl::move(lhs));
		lhs = mystl::move(rhs);
		rhs = mystl::move(tmp);
	}
	
	template<class ForwardIter1, class ForwardIter2>
	ForwardIter1 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
	{
		for(; first1 != last1; ++first1, (void)++first2)
			mystl::swap(*first1, *first2);
		return first2;
	}
	
	template <class Tp, size_t N>
	void swap(Tp(&a)[N], Tp(&b)[N])
	{
		mystl::swap_range(a, a + N, b);
	}
	///////////////////////////////////////////
	template <class Ty1, class Ty2>
	struct pair
	{
		typedef Ty1		first_type;
		typedef Ty2		second_type;
		
		first_type first;//NO.1 data
		second_type second;//NO.2 data
		 // default constructiable
		 /*
		 template <bool _Test, class _Ty = void>
		 struct enable_if {};
		 
		 template <class _Ty>
		 struct enable_if<true, _Ty>
		 {
		 	using type = _Ty;
		 };
		 */
  		template <class Other1 = Ty1, class Other2 = Ty2,
    	typename = typename std::enable_if<std::is_default_constructible<Other1>::value &&std::is_default_constructible<Other2>::value, void>::type>
    	constexpr pair():first(), second()
  		{}

	  // implicit constructiable for this type
	  	template <class U1 = Ty1, class U2 = Ty2,
	    typename std::enable_if<std::is_copy_constructible<U1>::value &&std::is_copy_constructible<U2>::value &&
		std::is_convertible<const U1&, Ty1>::value &&std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
	    constexpr pair(const Ty1& a, const Ty2& b) : first(a), second(b)
		{
		}

		// explicit constructible for this type
		template <class U1 = Ty1, class U2 = Ty2,
		typename std::enable_if<
		std::is_copy_constructible<U1>::value &&
		std::is_copy_constructible<U2>::value &&
		(!std::is_convertible<const U1&, Ty1>::value ||
		!std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
		explicit constexpr pair(const Ty1& a, const Ty2& b) : first(a), second(b)
		{
		}

  		pair(const pair& rhs) = default;
  		pair(pair&& rhs) = default;

	  	// implicit constructiable for other type
	  	template <class Other1, class Other2,
	    typename std::enable_if<
	    std::is_constructible<Ty1, Other1>::value &&
	    std::is_constructible<Ty2, Other2>::value &&
	    std::is_convertible<Other1&&, Ty1>::value &&
	    std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
	    constexpr pair(Other1&& a, Other2&& b)
	    : first(mystl::forward<Other1>(a)),
	    second(mystl::forward<Other2>(b))
	  	{
	  	}

		// explicit constructiable for other type
		template <class Other1, class Other2,
		typename std::enable_if<
		std::is_constructible<Ty1, Other1>::value &&
		std::is_constructible<Ty2, Other2>::value &&
		(!std::is_convertible<Other1, Ty1>::value ||
		!std::is_convertible<Other2, Ty2>::value), int>::type = 0>
		explicit constexpr pair(Other1&& a, Other2&& b) : first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b))
		{
		}

	  	// implicit constructiable for other pair
	 	template <class Other1, class Other2,
	    typename std::enable_if<
	    std::is_constructible<Ty1, const Other1&>::value &&
	    std::is_constructible<Ty2, const Other2&>::value &&
	    std::is_convertible<const Other1&, Ty1>::value &&
	    std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
	    constexpr pair(const pair<Other1, Other2>& other)
	    : first(other.first),
	    second(other.second)
	  	{
	  	}

	  	// explicit constructiable for other pair
	  	template <class Other1, class Other2,
	    typename std::enable_if<
	    std::is_constructible<Ty1, const Other1&>::value &&
	    std::is_constructible<Ty2, const Other2&>::value &&
	    (!std::is_convertible<const Other1&, Ty1>::value ||
	     !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
	    explicit constexpr pair(const pair<Other1, Other2>& other)	: first(other.first),
	    second(other.second)
	  	{
	  	}

  		// implicit constructiable for other pair
  		template <class Other1, class Other2,
    	typename std::enable_if<
    	std::is_constructible<Ty1, Other1>::value &&
    	std::is_constructible<Ty2, Other2>::value &&
    	std::is_convertible<Other1, Ty1>::value &&
    	std::is_convertible<Other2, Ty2>::value, int>::type = 0>
    	constexpr pair(pair<Other1, Other2>&& other) : first(mystl::forward<Other1>(other.first)), second(mystl::forward<Other2>(other.second))
		{
		}

	  	// explicit constructiable for other pair
	  	template <class Other1, class Other2,
	    typename std::enable_if<
	    std::is_constructible<Ty1, Other1>::value &&
	    std::is_constructible<Ty2, Other2>::value &&
	    (!std::is_convertible<Other1, Ty1>::value ||
	     !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
	    explicit constexpr pair(pair<Other1, Other2>&& other) : first(mystl::forward<Other1>(other.first)), second(mystl::forward<Other2>(other.second))
	  	{
	  	}

		// copy assign for this pair
		pair& operator=(const pair& rhs)
		{
			if (this != &rhs)
		    {
		    	first = rhs.first;
		    	second = rhs.second;
		    }
		    return *this;
		}

	  	// move assign for this pair
	  	pair& operator=(pair&& rhs)
	  	{
	    	if (this != &rhs)
	    	{
	      		first = mystl::move(rhs.first);
	      		second = mystl::move(rhs.second);
	    	}
	    	return *this;
	  	}

	  	// copy assign for other pair
	  	template <class Other1, class Other2>
	  	pair& operator=(const pair<Other1, Other2>& other)
	  	{
	    	first = other.first;
	    	second = other.second;
	    	return *this;
	  	}

	  	// move assign for other pair
	  	template <class Other1, class Other2>
	  	pair& operator=(pair<Other1, Other2>&& other)
	  	{
	    	first = mystl::forward<Other1>(other.first);
	    	second = mystl::forward<Other2>(other.second);
	    	return *this;
	  	}

  		~pair() = default;

		void swap(pair& other)
		{
			if (this != &other)
		    {
		      mystl::swap(first, other.first);
		      mystl::swap(second, other.second);
		    }
		}		
	};
}

#endif