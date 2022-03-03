#ifndef _MYSTL_VECTOR_H_
#define _MYSTL_VECTOR_H_

#include "allocator.hpp"
#include <iostream>
#include "iterator.hpp"

namespace mystl{
	template <class _Ty, class _Alloc = mystl::allocator<_Ty>>
	class vector{
		public:
			typedef _Ty	value_type;
			typedef _Ty*		pointer;
			typedef const _Ty* 	const_pointer;
			typedef _Ty&		reference;
			typedef const _Ty& 	const_reference;
			typedef size_t		size_type;
			typedef ptrdiff_t 	difference_type;
		
			typedef value_type*		iterator;
			typedef const value_type*	const_iterator;
			
			typedef mystl::reverse_iterator<iterator> reverse_iterator;
			typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;
		private:
			iterator begin_;
			iterator end_;
			iterator cap_;
		public:
			vector()
			{
				begin_ = _Alloc::allocate(16);
				end_ = begin_;
				cap_ = begin_ + 16; 
				//std::cout << "vector gouzao" << std::endl;
			}
			
			explicit vector(size_type n)
			{ 	begin_ = _Alloc::allocate(n);
				end_ = begin_;
				cap_ = begin_ + n;
				//std::cout << "vector gouzao" << std::endl;
			}
			
			vector(const vector<_Ty, _Alloc>& rhs); 
			
			vector<_Ty, _Alloc>& operator=(const vector<_Ty, _Alloc>& rhs);
			//vector<_Ty, _Alloc>& operator=(vector<_Ty, _Alloc>&& rhs);
			
			~vector()
			{
				_Alloc::destroy(begin_, end_);
				_Alloc::deallocate(begin_, cap_ - begin_);
				begin_ = end_ = cap_ = nullptr;
				//std::cout << "vector xigou" << std::endl;	
			}
			//******************
			iterator begin()
			{ return begin_; }
			iterator end() 
			{ return end_; };
			const_iterator begin() const
			{ return begin_; }
			const_iterator end() const
			{ return end_; }
			
			reverse_iterator rbegin()
			{ return reverse_iterator(end_); }
			reverse_iterator rend()
			{ return reverse_iterator(begin_); }
			const_reverse_iterator rbegin() const
			{ return const_reverse_iterator(end_); }
			const_reverse_iterator rend() const
			{ return const_reverse_iterator(begin_); }
			//*****************
			bool empty() const
			{ return begin_ == end_; };
			
			size_type size() const
			{ return static_cast<size_type>(end_ - begin_); }
			size_type capacity() const
			{ return static_cast<size_type>(cap_ - begin_); }
			
			reference operator[](size_type n)
			{ return begin_[n]; }
			const_reference operator[](size_type n) const
			{ return begin_[n]; }
			reference front()
			{ return *begin_; }
			const_reference front() const
			{ return *begin_; }
			reference back()
			{ return *(end_ - 1); }
			const_reference back() const
			{ return *(end_ - 1); }	
			pointer data()
			{ return begin_; }
			const_pointer data() const
			{ return begin_; }
			//*****************
			//push_back/pop_back
			void push_back(const value_type& value);
			void pop_back();
			
			//resize
			void resize(size_type new_size);
	};
	
	template <class _Ty, class _Alloc>
	vector<_Ty, _Alloc>::vector(const vector<_Ty, _Alloc>& rhs)
	{
		if(this != &rhs){
			size_type tmp = rhs.size();
			difference_type capacity = rhs.capacity();
		/*	_Alloc::destroy(begin(), end());
			_Alloc::deallocate(begin());
			*/
			begin_ = _Alloc::allocate(capacity);
			end_ = begin_ + tmp;
			cap_ = begin_ + capacity;
			
			auto begin_rhs = rhs.begin();
			int i;
			for(i = 0; i < tmp; i++){
				_Alloc::construct(begin_ + i, begin_rhs[i]); 
			}
		}
		//std::cout << "vector kaobei gouzao" << std::endl;
	}
	
	template <class _Ty, class _Alloc>
	vector<_Ty, _Alloc>& vector<_Ty, _Alloc>::operator=(const vector<_Ty, _Alloc>& rhs)
	{
		if(this != &rhs){
			size_type size = rhs.size();
			difference_type capacity = rhs.capacity();
			_Alloc::destroy(begin(), end());
			_Alloc::deallocate(begin());
			
			begin_ = _Alloc::allocate(capacity);
			end_ = begin_ + size;
			cap_ = begin_ + capacity;
			
			auto begin_rhs = rhs.begin();
			int i;
			for(i = 0; i < size; i++){
				_Alloc::construct(begin_ + i, begin_rhs[i]); 
			}
		}
		//std::cout << "vector operator =" << std::endl;
		return *this;
	}
	template <class _Ty, class _Alloc>
	void vector<_Ty, _Alloc>::push_back(const  value_type& value){
		if(end_ == cap_){
			resize(2 * size() + 1);
		}
		_Alloc::construct(end_, value);
		end_++;
	}
	template <class _Ty, class _Alloc>
	void vector<_Ty, _Alloc>::pop_back(){
		_Alloc::destroy(end_);
		end_--;
	}
	template <class _Ty, class _Alloc>
	void vector<_Ty, _Alloc>::resize(size_type new_size)
	{
		pointer ptr1 = begin_, ptr2 = end_;
		const size_type temp = size();
		
		begin_ = _Alloc::allocate(new_size);
		end_ = begin_ + temp;
		cap_ = begin_ + new_size;
		int i;
		for(i = 0; i < temp; i++){
			_Alloc::construct(begin_ + i, ptr1[i]);
		}
		
		_Alloc::destroy(ptr1, ptr2);
		_Alloc::deallocate(ptr1);
	}
}

#endif