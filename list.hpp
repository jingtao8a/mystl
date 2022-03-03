#ifndef _MYSTL_LIST_H
#define _MYSTL_LIST_H

#include "allocator.hpp"
#include "iterator.hpp"
#include <cstddef>

namespace mystl
{
	template <class T> struct list_node_base;
	template <class T> struct list_node;

	template <class T>
	struct node_traits
	{
		typedef list_node_base<T>* base_ptr;
		typedef list_node<T>*	node_ptr;
	};

	template <class T>
	struct list_node_base
	{
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;

		base_ptr prev;
		base_ptr next;

		list_node_base() = default;

		base_ptr self()
		{
			return static_cast<base_ptr>(&*this);
		}

		node_ptr as_node()
		{
			return static_cast<node_ptr>(self());
		}

		void unlink()
		{
			prev = next = self();
		}

	};

	template <class T>
	struct list_node : public list_node_base<T>
	{
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;

		T value;

		list_node() = default;
		list_node(const T& v) : value(v) {}
		//list_node(T&& v):value(mystl::move(v)){}

		node_ptr self()
		{
			std::cout << "yy" << std::endl;
			return static_cast<node_ptr>(&*this);
		}

		base_ptr as_base()
		{
			return static_cast<base_ptr>(&*this);
		}
	};

	template <class T>
	struct list_iterator : public mystl::iterator<bidrectional_iterator_tag, T>
	{
		typedef T		value_type;
		typedef T*		pointer;
		typedef T&		reference;
		typedef typename node_traits<T>::base_ptr	base_ptr;
		typedef typename node_traits<T>::node_ptr	node_ptr;
		typedef	list_iterator<T>	self;

		base_ptr	node_;

		list_iterator() = default;
		list_iterator(base_ptr x) : node_(x) {}
		list_iterator(node_ptr x) : node_(x->as_base()) {}
		list_iterator(const self& rhs) : node_(rhs.node_) {}//iterator needs kaobei gouzao

		reference operator*() const
		{
			return node_->as_node()->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self& operator++()
		{
			node_ = node_->next;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}

		self& operator--()
		{
			node_ = node_->prev;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }

	};

	template <class T>
	struct list_const_iterator : public iterator<bidrectional_iterator_tag, T>
	{
		typedef T		value_type;
		typedef const T*		pointer;
		typedef const T&		reference;
		typedef typename node_traits<T>::base_ptr		base_ptr;
		typedef typename node_traits<T>::node_ptr		node_ptr;
		typedef list_const_iterator<T>		self;

		base_ptr node_;

		list_const_iterator() = default;
		list_const_iterator(base_ptr x) : node_(x) {}
		list_const_iterator(node_ptr x) : node_(x) {}
		list_const_iterator(const list_iterator<T>& rhs) : node_(rhs.node_) {}
		list_const_iterator(const list_const_iterator& rhs) : node_(rhs.node_) {}

		reference operator*() const { return node_->as_node()->value; }
		pointer operator->() const { return &(operator*()); }

		self& operator++()
		{
			node_ = node_->next;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}

		self& operator--()
		{
			node_ = node_->prev;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
	};

	template <class T>
	class list
	{
	public:
		typedef mystl::allocator<T>						allocator_type;
		typedef mystl::allocator<T>						data_allocator;
		typedef mystl::allocator<list_node_base<T>>		base_allocator;
		typedef mystl::allocator<list_node<T>>			node_allocator;

		typedef T						value_type;
		typedef T*						pointer;
		typedef const T*				const_pointer;
		typedef T&						reference;
		typedef const T&				const_reference;
		typedef size_t					size_type;
		typedef ptrdiff_t				difference_type;

		typedef list_iterator<T>							iterator;
		typedef list_const_iterator<T>						const_iterator;
		typedef mystl::reverse_iterator<iterator>			reverse_iterator;
		typedef mystl::reverse_iterator<const_iterator>		const_reverse_iterator;

		typedef typename node_traits<T>::base_ptr			base_ptr;
		typedef typename node_traits<T>::node_ptr			node_ptr;

	private:
		base_ptr node_;
		size_type size_;

	public:
		list(){ std::cout << "list gouzao1" << std::endl; fill_init(0, value_type()); }
		explicit list(size_type n) { std::cout << "list gouzao2" << std::endl; fill_init(n, value_type()); }
		list(size_type n, const T& value) { std::cout << "list gouzao3" << std::endl; fill_init(n, value); }
		
		list(const list& rhs)
		{
			std::cout << "list kaobei gouzao1" << std::endl; 
			copy_init(rhs.begin(), rhs.end()); 
		}
		list(list&& rhs) : node_(rhs.node_), size_(rhs.size_)
		{
			std::cout << "list kaobei gouzao2" << std::endl; 
			rhs.node_ = nullptr;
			rhs.size_ = 0;
		}

		list& operator=(const list& rhs)
		{
			std::cout << "list operator= 1" << std::endl; 
			if (this != &rhs)
			{
				assign(rhs.begin(), rhs.end());
			}
			return *this;
		}

		list& operator=(list&& rhs)
		{
			std::cout << "list operator= 2" << std::endl;
			clear();
			splice(end(), rhs);
			return *this;
		}

		~list()
		{
			std::cout << "list xigou" << std::endl;
			clear();
			base_allocator::deallocate(node_);
			node_ = nullptr;
			size_ = 0;
		}
	public://be related to iterator
		iterator	begin()
		{ return iterator(node_->next); }
		const_iterator begin() const
		{ return const_iterator(node_->next); }
		iterator end()
		{ return iterator(node_); }
		const_iterator end() const
		{ return const_iterator(node_); }

		reverse_iterator rbegin()
		{ return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const
		{ return reverse_iterator(end()); }
		reverse_iterator rend()
		{ return reverse_iterator(begin()); }
		const_reverse_iterator rend() const
		{ return reverse_iterator(begin()); }
	private:
		//initialize / link / unlink
		template <class Iter>
		void copy_init(Iter first, Iter last);
		void fill_init(size_type n, const value_type& value);
		void link_nodes_at_front(base_ptr first, base_ptr last);
		void link_nodes_at_back(base_ptr first, base_ptr last);
		void unlink_nodes(base_ptr first, base_ptr last);
		void link_nodes(base_ptr pos, base_ptr first, base_ptr last);
	public:
		//void assign(size_type n, const value_type& value)
		//{ fill_assign(n, value); }

		template <class Iter>
		void assign(Iter first, Iter last)
		{
			copy_assign(first, last);
		}

		template <class Iter>
		void copy_assign(Iter f2, Iter l2);
		
		iterator erase(const_iterator first, const_iterator last);

		void destroy_node(node_ptr p);

		template <class Iter>
		iterator insert(const_iterator pos, Iter first, Iter last);

		template <class Iter>
		iterator copy_insert(const_iterator pos, size_type n, Iter first);

		template <class ...Args>
		node_ptr create_node(Args&& ...args);

		void clear();

		void splice(const_iterator pos, list& x);

		bool empty()
		{
			return (node_->prev == node_ && node_->next == node_);
		}
		
		size_type size()
		{
			return size_;
		}
		
		void push_front(const value_type& value)
		{
			node_ptr p = create_node(value);
			link_nodes_at_front(p->as_base(), p->as_base());
			++size_;
		}
		void push_back(const value_type& value)
		{
			node_ptr p = create_node(value);
			link_nodes_at_back(p->as_base(), p->as_base());
			++size_;
		}
		void pop_front()
		{
			base_ptr p = node_->next;
			unlink_nodes(p, p);
			destroy_node(p->as_node());
			--size_;
		}
		void pop_back()
		{
			base_ptr p = node_->prev;
			unlink_nodes(p, p);
			destroy_node(p->as_node());
			--size_;
		}
	};

	template <class T>
	template <class Iter>
	void list<T>::copy_assign(Iter f2, Iter l2)
	{
		auto f1 = begin();
		auto l1 = end();
		for (; f1 != l1 && f2 != l2; ++f1, ++f2)
		{
			*f1 = *f2;
		}
		if (f2 == l2)
		{
			erase(f1, l1);
		}
		else
		{
			insert(l1, f2, l2);
		}
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last)
	{
		if (first != last)
		{
			unlink_nodes(first.node_, last.node_->prev);
			while (first != last)
			{
				auto cur = first.node_;
				++first;
				destroy_node(cur->as_node());
				--size_;
			}
		}
		return iterator(last.node_);
	}

	template <class T>
	void list<T>::destroy_node(node_ptr p)
	{
		data_allocator::destroy(&(p->value));
		node_allocator::deallocate(p);
	}
	template <class T>
	template <class Iter>
	typename list<T>::iterator list<T>::insert(const_iterator pos, Iter first, Iter last)
	{
		size_type n = mystl::distance(first, last);
		return copy_insert(pos, n, first);
	}

	template <class T>
	template <class Iter>
	typename list<T>::iterator list<T>::copy_insert(const_iterator pos, size_type n, Iter first)
	{
		iterator r(pos.node_);
		if (n != 0)
		{
			const auto add_size = n;
			auto node = create_node(*first);
			node->prev = nullptr;
			r = iterator(node);
			iterator end = r;
			for (--n, ++first; n > 0; --n, ++first, ++end)
			{
				auto next = create_node(*first);
				end.node_->next = next->as_base();  // link node
				next->prev = end.node_;
			}
			size_ += add_size;

			link_nodes(pos.node_, r.node_, end.node_);
		}
		return r;
	}

	template <class T>
	template <class ...Args>
	typename list<T>::node_ptr list<T>::create_node(Args&& ...args)
	{
		node_ptr p = node_allocator::allocate(1);

		data_allocator::construct(&(p->value), mystl::forward<Args>(args)...);
		p->prev = nullptr;
		p->next = nullptr;
		return p;
	}

	template <class T>
	template <class Iter>
	void list<T>::copy_init(Iter first, Iter last)
	{
		node_ = base_allocator::allocate(1);
		node_->unlink();
		size_type n = mystl::distance(first, last);
		size_ = n;
		while (n > 0) {
			node_ptr p = node_allocator::allocate(1);
			p->prev = nullptr;
			p->next = nullptr;
			data_allocator::construct(&(p->value), (*first));
			link_nodes_at_back(p->as_base(), p->as_base());
			++first;
			--n;
		}
	}

	template <class T>
	void list<T>::fill_init(size_type n, const value_type& value)
	{
		node_ = base_allocator::allocate(1);
		node_->unlink();
		size_ = n;
		while (n > 0) {
			node_ptr p = node_allocator::allocate(1);
			data_allocator::construct(&(p->value), value);
			p->prev = nullptr;
			p->next = nullptr;
			link_nodes_at_back(p->as_base(), p->as_base());
			--n;
		}
	}

	template <class T>
	void list<T>::link_nodes_at_front(base_ptr first, base_ptr last)
	{
		first->prev = node_;
  		last->next = node_->next;
  		last->next->prev = last;
  		node_->next = first;
	}

	template <class T>
	void list<T>::link_nodes_at_back(base_ptr first, base_ptr last)
	{
		last->next = node_;
  		first->prev = node_->prev;
    	first->prev->next = first;
  		node_->prev = last;
	}
	template <class T>
	void list<T>::unlink_nodes(base_ptr first, base_ptr last)
	{
		first->prev->next = last->next;
		last->next->prev = first->prev;
	}
	template <class T>
	void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last)
	{
		first->prev = pos;
  		last->next = pos->next;
  		last->next->prev = last;
  		pos->next = first;
	}
	template <class T>
	void list<T>::clear()
	{
		if (size_ != 0)
		{
			auto cur = node_->next;
			for (base_ptr next = cur->next; cur != node_; cur = next, next = cur->next)
			{
				destroy_node(cur->as_node());
			}
			node_->unlink();
			size_ = 0;
		}
	}

	template <class T>
	void list<T>::splice(const_iterator pos, list& x)
	{
		if (!x.empty())
		{

			auto f = x.node_->next;
			auto l = x.node_->prev;

			x.unlink_nodes(f, l);
			link_nodes(pos.node_, f, l);

			size_ = x.size_;
			x.size_ = 0;
		}
	}
}

#endif