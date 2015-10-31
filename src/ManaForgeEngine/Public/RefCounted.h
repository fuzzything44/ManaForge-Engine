#pragma once
#include <cassert>

template<typename Derived>
struct RefCounted
{
	explicit RefCounted(nullptr_t)
		:refCount{ new size_t{ 1 } }
	{
	}

	RefCounted(const Derived& other)
		:refCount{other.refCount}
	{

		assert(refCount);
		++(*refCount);
	}

	RefCounted(Derived&& other)
		:refCount{other.refCount}
	{

	}

	RefCounted& operator=(Derived&& other)
	{
		if(refCount) --(*refCount);
		if (*refCount == 0)
		{
			other.destroy();
		}

		refCount = other.refCount;

		other.refCount = nullptr;
	}
	RefCounted& operator=(const Derived& other)
	{
		if (refCount) --*(refCount);
		if (*refCount == 0)
		{
			other.destroy();
		}

		refCount = other.refCount;

		++(*refCount);
	}



	~RefCounted()
	{
		if (refCount)
		{
			--(*refCount);

			// if refCount == 0
			if (!*refCount)
			{
				static_cast<Derived&>(*this).destroy();
				delete refCount;
			}
		}
	}



protected:
	size_t* refCount;
};
