#pragma once
#include "MPLHelp.h"

namespace detail
{
	template<typename SequenceOfManagers, bool needsExit = false>
	struct FindManagers
	{
		static_assert(boost::mpl::is_sequence<SequenceOfManagers>::value, "SequenceOfManagers must be a sequence");

		template<typename... Args>
		using FindUpperManagers =
			CatSequences_t<typename Args::MyBases...>;

		using ExpandedManagers =
			Unduplicate_t
			<
				CatSequences_t
				<
					SequenceOfManagers,
					ExpandSequenceToVaraidic_t
					<
						SequenceOfManagers
						, FindUpperManagers
					>
				>
			>
			;

		static constexpr const bool needsExitNext =
			boost::mpl::equal_to
			<
				typename boost::mpl::size<SequenceOfManagers>::type
				, typename boost::mpl::size<ExpandedManagers>::type
			>::type::value
			;

		using type =
			typename FindManagers
			<
				ExpandedManagers
				, needsExitNext
			>::type
			;
	};
	template<typename SequenceOfManagers>
	struct FindManagers<SequenceOfManagers, true>
	{
		using type = SequenceOfManagers;
		static_assert(boost::mpl::is_sequence<type>::value, "SequenceOfManagers needs to be a sequence.");
	};

}