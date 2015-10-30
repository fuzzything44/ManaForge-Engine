#pragma once

#include <tuple>

namespace detail
{
	template<typename ThisType, typename ManagerToGet>
	struct GetRefToManager
	{
		static ManagerToGet& apply(ThisType& data)
		{
			static_assert(ThisType::template isManager<ManagerToGet>(), "ManagerToGet must be a manager");

			constexpr auto managerID = ThisType::template getManagerExceptThisID<ManagerToGet>();

			return *std::get<managerID>(data.basePtrStorage);
		}
	};

	template<typename ThisType>
	struct GetRefToManager<ThisType, ThisType>
	{
		static ThisType& apply(ThisType& data)
		{
			static_assert(ThisType::template isManager<ThisType>(), "ManagerToGet must be a manager");

			return data;
		}
	};

}
