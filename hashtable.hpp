#ifndef MYSTL_HASHTABLE_HPP
#define MYSTL_HASHTABLE_HPP

#include <vector>

namespace mystl{
	template <class T>
	struct hashtable_node{
		hashtable_node* next;
		T		value;
		hashtable_node()=default;
		hashtable_node(const T& n) : next(nullptr), value(n) {}
		
		hashtable_node(const hashtable_node& node) : 
			next(node.next), value(node.value) {}
		hashtable_node(hashtable_node&& node) :
			next(node.next), value(std::move(node.value))
		{
			node.next = nullptr;
		}
	};
	
	template <class T, bool>
	struct ht_value_traits_imp{
		typedef T key_type;
		typedef T mapped_type;
		typedef T value_type;
		
		template <class Ty>
		static const key_type& get_key(const Ty& value)
		{
			return value;
		}
		
		template <class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value;
		}
	};
	
	template <class T>
	struct ht_value_traits_imp<T, true>{
		typedef typename std::remove_cv<typename T::first_type>::type key_type;
		typedef typename T::second_type		mapped_type;
		typedef T			value_type;
		
		template <class Ty>
		static const key_type& get_key(const Ty& value)
		{
			return value.first;
		}
		
		template <class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value;
		}
	};
	
	template <class T>
	struct ht_value_traits{
		static constexpr bool is_map = mystl::is_pair<T>::value;
		typedef ht_value_traits_imp<T, is_map> value_traits_type;
		
		typedef typename value_traits_type::key_type    key_type;
  		typedef typename value_traits_type::mapped_type mapped_type;
  		typedef typename value_traits_type::value_type  value_type;
		
		template <class T>
		static const key_value& get_key(const Ty& value)
		{
			return value_traits_type::get_key(value);
		}
		
		static const key_value& get_value(const Ty& value)
		{
			return value_traits_type::get_value(value);
		}
	};
	
	template <class T, class Hash, class KeyEqual>
	class hashtable
	{
		public:
			typedef ht_value_traits<T> value_traits;
			typedef typename value_traits::key_type 	key_type;
			typedef typename value_traits::mapped_type 	mapped_type;
			typedef typename value_traits::value_type 	value_type;
			typedef Hash								hasher;
			typedef KeyEqual							key_equal;
			
			typedef hashtable_node<T>					node_type;
			typedef node_type*							node_ptr;
			typedef std::vector<node_ptr>				bucket_type;
			
			typedef mystl::allocator<T>					allocator_type;
			typedef mystl::allocator<T>					data_allocator;
			typedef mystl::allocator<node_type>			node_allocator;
			
			typedef typename allocator_type::pointer            pointer;
  			typedef typename allocator_type::const_pointer      const_pointer;
  			typedef typename allocator_type::reference          reference;
  			typedef typename allocator_type::const_reference    const_reference;
  			typedef typename allocator_type::size_type          size_type;
 		 	typedef typename allocator_type::difference_type    difference_type;
			
		private:
			bucket_type buckets_;
			size_type bucket_size_;
			size_tyep size_;
			float mlf_;
			hasher hash_;
			key_qual equal;
			
			
	};
}