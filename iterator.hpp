#ifndef _MYSTL_ITERATOR_H
#define _MYSTL_ITERATOR_H

#include <cstddef>
#include "type_traits.hpp"

namespace mystl
{
	//five iterator type
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag : public input_iterator_tag{};
	struct bidrectional_iterator_tag : public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidrectional_iterator_tag{};
	//iterator template
	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator{
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
		typedef Distance		difference_type;
	};
	//iterator traits
	template <class Iterator>
	struct iterator_traits{
		typedef typename Iterator::iterator_category 	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::difference_type		difference_type;
	};
	//for native pointer
	template <class T>
	struct iterator_traits<T*>{
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t 						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};
	//for native pointer (pointer to const)
	template <class T>
	struct iterator_traits<const T*>{
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t 						difference_type;
		typedef const T*								pointer;
		typedef const T&								reference;
	};
	//make sure the category
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&){
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}
	//make sure the distance type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&){
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}
	//make sure the value type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&){
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


	//distance
	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while(first != last){
			++first;
			++n;
		}
		return n;
	}

	template <class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
		return last - first;
	}

	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last){
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}

	template <class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag){
		while(n--){
			++i;
		}
	}

	template <class BidrectionalIterator, class Distance>
	inline void __advance(BidrectionalIterator& i, Distance n, BidrectionalIterator){
		if(n >= 0){
			while(n--) ++i;
		}else{
			while(n++) --i;
		}
	}

	template <class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n){
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		__advance(i, n, category());
	}
	
	//reverse_iterator
	template <class Iterator>
	class reverse_iterator{
		private:
			Iterator current;
		public:
			typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
  			typedef typename iterator_traits<Iterator>::value_type        value_type;
  			typedef typename iterator_traits<Iterator>::difference_type   difference_type;
  			typedef typename iterator_traits<Iterator>::pointer           pointer;
 			typedef typename iterator_traits<Iterator>::reference         reference;	
			
			typedef Iterator                                              iterator_type;
  			typedef reverse_iterator<Iterator>                            self;	
		
		public:
			reverse_iterator() = default;
			explicit reverse_iterator(iterator_type i):current(i){}
			
			reverse_iterator(const self& rhs):current(rhs.current){}
			
		public:
			iterator_type base() const
			{ return current; }
			
			reference operator*() const
			{ 
				iterator_type tmp = current;
				return *(--tmp);
			}
			
			pointer operator->() const
			{
				return &(operator*());
			}
			
			//++ -> --
			self& operator++()
			{
				--current;
				return *this;
			}
			
			self operator++(int)
			{
				self tmp = *this;
				--current;
				return tmp;
			}
			//-- -> ++
			self& operator--()
			{
				++current;
				return *this;
			}
			self operator--(int)
			{
				self tmp = *this;
				++current;
				return tmp;
			}
	};
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
	operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}
	
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() < lhs.base();
	}

	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs < lhs;
	}
	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs < rhs);
	}
}

#endif
