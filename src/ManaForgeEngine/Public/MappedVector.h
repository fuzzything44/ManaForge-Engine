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
		iterator(
			std::vector<size_t>::iterator indexIter = std::vector<size_t>::iterator{},
			typename std::vector<T>::iterator dataIter = std::vector<T>::iterator{})
			: indexIter{ indexIter }
			, dataIter{ dataIter }
		{}

		std::vector<size_t>::iterator indexIter;
		typename std::vector<T>::iterator dataIter;

		std::pair<size_t&, T&> dereference() const
		{
			return { *indexIter, *dataIter };
		}

		bool equal(const iterator& other) const
		{
			return
				indexIter == other.indexIter &&
				dataIter == other.dataIter;
		}

		void increment()
		{
			++indexIter;
			++dataIter;
		}

		void decrement()
		{
			--indexIter;
			--dataIter;
		}

		void advance(size_t num)
		{
			std::advance(indexIter, num);
			std::advance(dataIter, num);
		}

		size_t distance_to(const iterator& iter) const
		{
			return std::distance(indexIter, iter.indexIter);
		}
	};

	iterator begin()
	{
		return iterator{ indicies.begin(), data.begin() };
	}

	iterator end()
	{
		return iterator{ indicies.end(), data.end() };
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
		return std::binary_search(indicies.begin(), indicies.end(), elem);
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
	iterator emplace(size_t location, Args&&... args)
	{
		auto&& iter = std::lower_bound(indicies.begin(), indicies.end(), location);

		auto indexInData = std::distance(indicies.begin(), iter);
		auto iterInData = data.begin();
		std::advance(iterInData, indexInData);


		// if the location doesn't exist
		if (iter == indicies.end() || *iter != location)
		{
			auto&& indiciesRetIter = indicies.emplace(iter, location);
			
			auto&& dataRetIter = data.emplace(iterInData, std::forward<Args>(args)...);

			return iterator{ indiciesRetIter, dataRetIter };
		}

		// and if it does...
		*iterInData = T(std::forward<Args>(args)...);

		return iterator{ iter, iterInData };
	}

	void erase(iterator it)
	{
		indicies.erase(it.indexIter);
		data.erase(it.dataIter);
	}

	iterator get_elem_at(size_t ID)
	{
		auto&& indiciesIter = std::lower_bound(indicies.begin(), indicies.end(), ID);
		
		if (indiciesIter == indicies.end() || *indiciesIter != ID) return end(); // if it doesn't exist, then quit!

		size_t loc = std::distance(indicies.begin(), indiciesIter); // get the index in either vector
		auto&& dataIter = data.begin();  std::advance(dataIter, loc); // get the iterator in data
		return iterator{ indiciesIter, dataIter };
	}

	std::vector<size_t> indicies;
	std::vector<T> data;
};
