#pragma once
#include <cassert>

template<typename Derived>
struct RefCounted
{
	explicit RefCounted()
		:refCount{ new size_t{ 1 } }
	{
	}

	RefCounted(const RefCounted<Derived>& other)
		:refCount{other.refCount}
	{

		assert(refCount);
		++(*refCount);
	}

	RefCounted(RefCounted<Derived>&& other)
		:refCount{other.refCount}
	{
		other.refCount = nullptr;
	}

	RefCounted<Derived>& operator=(RefCounted<Derived>&& other)
	{
		this->~RefCounted();

		refCount = other.refCount;

		other.refCount = nullptr;

		return *this;
	}
	RefCounted<Derived>& operator=(const RefCounted<Derived>& other)
	{
		this->~RefCounted();

		refCount = other.refCount;

		++(*refCount);

		return *this;
	}



	~RefCounted()
	{
		if (refCount)
		{
			--(*refCount);

			// if refCount == 0
			if (!*refCount)
			{
				static_cast<Derived&>(*this).Derived::destroy();
				delete refCount;
			}
		}
	}



protected:
	size_t* refCount;
};
