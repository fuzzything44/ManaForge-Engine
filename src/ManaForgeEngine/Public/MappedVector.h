#pragma once

#include <vector>
#include <algorithm>
#include <exception>


template<typename T>
struct MappedVector
{
	MappedVector() = default;
	
	template<typename...Args
	void emplace(Args&& args)
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
		if (*iter == index)
		{
			auto indexInData = std::distance(indicies.begin(), iter);
			return data[indexInData];

		}
		throw std::runtime_exception("index dosn't exist");
		
	}
	template<typename Args...>
	emplace(size_t location, Args&&... args)
	{
		auto&& iter = std::lower_bound(indicies.begin(), indicies.end(), index);

		(*iter != )

		indicies.emplace(iter, index);
		auto indexInData = std::distance(indicies.begin(), iter);
		auto iterInData = std::advance(data.begin(), indexInData);
		data.emplace()
	}

private:
	std::vector<size_t> indicies;
	std::vector<T> data;
};
