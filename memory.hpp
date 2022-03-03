#ifndef _MYSTL_MEMORY_H_
#define _MYSTL_MEMORY_H_

namespace mystl
{
	//get address
	template <class Tp>
	constexpr Tp* address_of(Tp& value) 
	{
		return &value;
	}
	
	template <class T>
	class auto_ptr
	{
		public:
			typedef T elem_type;
		private:
			T* m_ptr;
		public:
			explicit auto_ptr(T* p = nullptr):m_ptr(p){}
			auto_ptr(auto_ptr& rhs) : m_ptr(rhs.release()){}
			template <class U>
			auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()){}
			
			auto_ptr& operator=(auto_ptr& rhs)
			{
				if(this != &rhs){
					delete m_ptr;
					m_ptr = rhs.release();
				}
				return *this;
			}
			template <class U>
			auto_ptr& operator=(auto_ptr<U>& rhs)
			{
				if(this->get() != rhs.get()){
					delete m_ptr;
					m_ptr = rhs.release();
				}
				return *this;
			}
			~auto_ptr(){ delete m_ptr; }
			
		public:
			T& operator*() const { return *m_ptr; }
			T* operator->() const { return m_ptr; }
			
			T* get() const{ return m_ptr; }
			
			T* release()
			{
				T* tmp = m_ptr;
				m_ptr = nullptr;
				return tmp;
			}
			void reset(T* p = nullptr)
			{
				if(m_ptr != p)
				{
					delete m_ptr;
					m_ptr = p;
				}
			}
	};
}

#endif