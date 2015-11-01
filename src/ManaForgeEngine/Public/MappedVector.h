#pragma once

#include <vector>
#include <algorithm>
#include <exception>

#include <boost/iterator/iterator_facade.hpp>



template<typename T>
struct MappedVector
{
	friend struct iterator;

	struct iterator :
		boost::iterators::iterator_facade
		<
			iterator
			, std::pair<size_t, T>
			, boost::iterators::random_access_traversal_tag
			, std::pair<size_t&, T&>
		>
	{
		iterator(size_t index, MappedVector<T>& vector)
			:index{ index }
			, vector{ vector }
		{}

		size_t index;

		MappedVector<T>& vector;

		std::pair<size_t&, T&> dereference() const
		{
			return { vector.indicies[index], vector.data[index] };
		}

		bool equal(const iterator& other) const
		{
			return index == other.index && &vector == &other.vector;
		}

		void increment()
		{
			++index;
		}

		void decrement()
		{
			--index;
		}

		void advance(size_t num)
		{
			index += num;
		}

		size_t distance_to(const iterator& iter) const
		{
			assert(&vector == &iter.vector);

			return iter.index - index;
		}
	};

	iterator begin()
	{
		return iterator{ 0, *this };
	}

	iterator end()
	{
		return iterator{ indicies.size(), *this };
	}



	MappedVector() = default;
	
	template<typename...Args>
	void emplace_back(Args&&... args)
	{
		auto&& index = indicies[indicies.size() - 1] + 1;

		data.emplace_back(std::forward<Args>(args)...);
		indicies.emplace_back(index);
	}

	bool elemExists(size_t elem)
	{
		return indicies.end() != std::binary_search(indicies.begin(), indicies.end(), elem);
	}

	T& operator[](size_t index)
	{
		auto&& iter = std::lower_bound(indicies.begin(), indicies.end(), index);

		// if it exists
		if (iter != indicies.end() && *iter == index)
		{
			auto indexInData = std::distance(indicies.begin(), iter);
			return data[indexInData];

		}
		throw std::runtime_error("index dosn't exist");
		
	}

	template<typename... Args>
	void emplace(size_t location, Args&&... args)
	{
		auto&& iter = std::lower_bound(indicies.begin(), indicies.end(), location);

		auto indexInData = std::distance(indicies.begin(), iter);
		auto iterInData = data.begin();
		std::advance(iterInData, indexInData);

		// if the location doesn't exist
		if (iter == indicies.end() || *iter != location)
		{
			indicies.emplace(iter, location);
			
			data.emplace(iterInData, std::forward<Args>(args)...);

			return;
		}

		// and if it does...
		*iterInData = T(std::forward<Args>(args)...);
	}

private:
	std::vector<size_t> indicies;
	std::vector<T> data;
};
