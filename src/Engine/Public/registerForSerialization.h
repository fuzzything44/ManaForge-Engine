
#ifndef SAVE_DATA
#	error please define SAVE_DATA in the format <name_of_class>, <name_of_parent_class or 0 if no parent>, savedval1, savedval2, savedval2, ...
#endif

#if BOOST_PP_VARIADIC_SIZE(SAVE_DATA) >= 1

#	define BOOST_PP_LOCAL_LIMITS (2, BOOST_PP_VARIADIC_SIZE(SAVE_DATA) - 1)

#	define CLASS_NAME BOOST_PP_VARIADIC_ELEM(0, SAVE_DATA)
#	define PARENT_NAME BOOST_PP_VARIADIC_ELEM(1, SAVE_DATA)

template<typename Archive>
void CLASS_NAME::serialize(Archive& ar, const unsigned int version)
{
#	if PARENT_NAME != 0
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(PARENT_NAME);
#	endif

#	define BOOST_PP_LOCAL_MACRO(n)\
			ar & BOOST_SERIALIZATION_NVP(BOOST_PP_VARIADIC_ELEM(n, SAVE_DATA)); \
		/**/

#	include BOOST_PP_LOCAL_ITERATE()

}

// IF YOU GET A COMPILE TIME ERROR HERE, YOU FORGOT TO PLACE THE INIT_SAVED_ACTOR MACRO IN THE BEGINNING OF THE CLASS
template void CLASS_NAME::serialize<>(boost::archive::polymorphic_oarchive&, const unsigned int version);
template void CLASS_NAME::serialize<>(boost::archive::polymorphic_iarchive&, const unsigned int version);

BOOST_CLASS_EXPORT_IMPLEMENT(CLASS_NAME)

#	undef CLASS_NAME
#	undef PARENT_NAME

#endif


#undef SAVE_DATA
