#ifndef STACK_HPP
#define STACK_HPP

#include <deque>
#include <cstddef>
namespace mystl{
	template <class T, class Container = std::deque<T>>
	class stack
	{
		public:
			typedef Container                           container_type;
	  		typedef typename Container::value_type      value_type;
	  		typedef typename Container::size_type       size_type;
	  		typedef typename Container::reference       reference;
	  		typedef typename Container::const_reference const_reference;
	  	
		private:
	  		Container_type c_;//deque
		public:
			stack() = default;
			explicit stack(size_type n):c_(n){}	
			stack(size_type n, const value_type& value):c_(n, value){}
			stack& operator=(const stack& rhs)
	  		{
	   			c_ = rhs.c_;
	    		return *this;
	  		}
	  		~stack() = default;
	  		
	  		reference top() 
			{
				return c_.back();
			}
			
			bool empty() { return c_.empty; }
			
			void push(const value_type& value)
			{
				c_.push_back(value);
			}
			
			void pop()
			{
				c_.pop_back();
			}
			void clear()
			{
				while (!empty()) {
					pop();
				}
			}
	}
}
#endif