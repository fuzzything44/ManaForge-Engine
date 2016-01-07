#pragma once

#include <tuple>

#include <boost/hana.hpp>

namespace detail
{
	template<typename ThisType, typename ManagerToGet>
	struct GetRefToManager
	{
		static ManagerToGet& apply(ThisType& data)
		{
			BOOST_HANA_CONSTANT_CHECK(ThisType::template isManager(boost::hana::type_c<ManagerToGet>));

			constexpr auto managerID = ThisType::template getManagerExceptThisID(boost::hana::type_c<ManagerToGet>);

			return *data.basePtrStorage[managerID];
		}
	};

	template<typename ThisType>
	struct GetRefToManager<ThisType, ThisType>
	{
		static ThisType& apply(ThisType& data)
		{
			return data;
		}
	};

}
