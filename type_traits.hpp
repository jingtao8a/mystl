#ifndef MYTINY_TYPE_TRAITS_H_
#define MYTINY_TYPE_TRAITS_H_

#include <type_traits>

namespace mystl
{
	template <class T, T v>
	struct m_integral_constant
	{
		public:
			static constexpr T value = v;
	};
	
	typedef m_integral_constant<bool, true> m_true_type;
	typedef m_integral_constant<bool, false> m_false_type;
	
	//is_same
	template <class T, class U>
	struct is_same : public mystl::m_false_type{};
	
	template <class T>
	struct is_same<T, T> : public mystl::m_true_type{};
	
}

#endif