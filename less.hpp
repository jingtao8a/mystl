#ifnde LESS_HPP
#define LESS_HPP

namespace mystl{
	
	template <class T>
	struct less {
		bool operator() (const T& x, const T& y) const 
		{
			return x < y;	
		}
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
	};
	
}