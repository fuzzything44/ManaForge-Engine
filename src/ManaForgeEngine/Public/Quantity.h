#include <ratio>

#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/plus.hpp>

namespace qty
{

// basic types
using mass = boost::mpl::vector_c<int, 1, 0, 0, 0, 0>;
using position = boost::mpl::vector_c<int, 0, 1, 0, 0, 0>;
using time = boost::mpl::vector_c<int, 0, 0, 1, 0, 0>;
using temperature = boost::mpl::vector_c<int, 0, 0, 0, 1, 0>;
using angle = boost::mpl::vector_c<int, 0, 0, 0, 0, 1>;

// compound types
using energy = boost::mpl::vector_c<int, 1, 2, -2, 0, 0>;
using velocity = boost::mpl::vector_c<int, 0, 1, -1, 0, 0>;
using acceleration = boost::mpl::vector_c<int, 0, 1, -2, 0, 0>;
using force = boost::mpl::vector_c<int, 1, 1, -2, 0, 0>;

// actual units, use these
using gram = boost::mpl::vector<mass, std::ratio<1>>;
using meter = boost::mpl::vector<position, std::ratio<1>>;
using second = boost::mpl::vector<time, std::ratio<1>>;
using joule = boost::mpl::vector<energy, std::kilo>;
using deg_c = boost::mpl::vector<temperature, std::ratio<1>>;
using radian = boost::mpl::vector<angle, std::ratio<1>>;
using newton = boost::mpl::vector<force, std::ratio<1>>;
using meter_per_second = boost::mpl::vector<velocity, std::ratio<1>>;
using meter_per_second_squared = boost::mpl::vector<acceleration, std::ratio<1>>;

using nanogram = boost::mpl::vector<mass, std::nano>;
using nanometer = boost::mpl::vector<position, std::nano>;
using nanonewton = boost::mpl::vector<force, std::nano>;
using nanojoule = boost::mpl::vector<energy, std::micro>;

using microgram = boost::mpl::vector<mass, std::micro>;
using micrometer = boost::mpl::vector<position, std::micro>;
using micronewton = boost::mpl::vector<force, std::micro>;
using microjoule = boost::mpl::vector<energy, std::milli>;

using milligram = boost::mpl::vector<mass, std::milli>;
using millimeter = boost::mpl::vector<position, std::milli>;
using millinewton = boost::mpl::vector<force, std::milli>;
using millijoule = boost::mpl::vector<energy, std::ratio<1>>;

using centigram = boost::mpl::vector<mass, std::centi>;
using centimeter = boost::mpl::vector<position, std::centi>;
using centinewton = boost::mpl::vector<force, std::centi>;
using centijoule = boost::mpl::vector<energy, std::deca>;

using decigram = boost::mpl::vector<mass, std::deci>;
using decimeter = boost::mpl::vector<position, std::deci>;
using decinewton = boost::mpl::vector<force, std::deci>;
using decijoule = boost::mpl::vector<energy, std::hecto>;

using decagram = boost::mpl::vector<mass, std::deca>;
using decameter = boost::mpl::vector<position, std::deca>;
using decanewton = boost::mpl::vector<force, std::deca>;
using decajoule = boost::mpl::vector<energy, std::ratio<10000>>;

using hectogram = boost::mpl::vector<mass, std::hecto>;
using hectometer = boost::mpl::vector<position, std::hecto>;
using hectonewton = boost::mpl::vector<force, std::hecto>;
using hectojoule = boost::mpl::vector<energy, std::ratio<100000>>;

using kilogram = boost::mpl::vector<mass, std::kilo>;
using kilometer = boost::mpl::vector<position, std::kilo>;
using kilonewton = boost::mpl::vector<force, std::kilo>;
using kilojoule = boost::mpl::vector<energy, std::mega>;

using megagram = boost::mpl::vector<mass, std::mega>;
using megameter = boost::mpl::vector<position, std::mega>;
using meganewton = boost::mpl::vector<force, std::mega>;
using megajoule = boost::mpl::vector<energy, std::giga>;

using gigagram = boost::mpl::vector<mass, std::giga>;
using gigameter = boost::mpl::vector<position, std::giga>;
using giganewton = boost::mpl::vector<force, std::giga>;
using gigajoule = boost::mpl::vector<energy, std::tera>;

using teragram = boost::mpl::vector<mass, std::tera>;
using terameter = boost::mpl::vector<position, std::tera>;
using teranewton = boost::mpl::vector<force, std::tera>;
using terajoule = boost::mpl::vector<energy, std::peta>;

using minute = boost::mpl::vector<time, std::ratio<60>>;
using hour = boost::mpl::vector<time, std::ratio<60 * 60>>;
using day = boost::mpl::vector<time, std::ratio<60 * 60 * 24>>;
using week = boost::mpl::vector<time, std::ratio<60 * 60 * 24 * 7>>;
}

template <typename Dimensions> class Quantity
{
	float data;

public:
	Quantity(float f = 0.f)
		: data(f)
	{
	}

	template <typename DimensionsU,
		typename = std::enable_if<boost::mpl::equal<boost::mpl::at_c<DimensionsU, 0>::type,
			boost::mpl::at_c<Dimensions, 0>::type>::type::value>>
	Quantity(const Quantity<DimensionsU>& other)
	{
		constexpr float multiplier = ((float)boost::mpl::at_c<DimensionsU, 1>::type::num
										 / (float)boost::mpl::at_c<DimensionsU, 1>::type::den)
									 / ((float)boost::mpl::at_c<Dimensions, 1>::type::num
										   / (float)boost::mpl::at_c<Dimensions, 1>::type::den);

		data = other.get() * multiplier;
	}

	float& get() { return data; }
	float get() const { return data; }

	template <typename DimensionsU,
		typename = std::enable_if<boost::mpl::equal<boost::mpl::at_c<DimensionsU, 0>::type,
			boost::mpl::at_c<Dimensions, 0>::type>::type::value>>
	const Quantity<Dimensions>& operator=(const Quantity<DimensionsU>& other)
	{
		constexpr float multiplier =
			((float)boost::mpl::at_c<DimensionsU, 1>::num / (float)boost::mpl::at_c<DimensionsU, 1>::den)
			/ ((float)boost::mpl::at_c<Dimensions, 1>::num / (float)boost::mpl::at_c<Dimensions, 1>::den);

		data = other.get() * multiplier;
	}

	template <typename DimensionsU> auto operator*(const Quantity<DimensionsU>& other)
	{
		Quantity<boost::mpl::vector<boost::mpl::at_c<DimensionsU, 0>::type,
			boost::mpl::at_c<Dimensions, 1>::type>> otherConverted = other;

		using outputDims = boost::mpl::transform<boost::mpl::at_c<Dimensions, 0>::type,
			boost::mpl::at_c<DimensionsU, 0>::type,
			boost::mpl::plus<boost::mpl::placeholders::_1, boost::mpl::placeholders::_2>>;

		return Quantity<boost::mpl::vector<outputDims, boost::mpl::at_c<Dimensions, 1>::type>>(
			get() * otherConverted.get());
	}

	template <typename DimensionsU> auto operator/(const Quantity<DimensionsU>& other)
	{
		// make sure we have the same multiplier
		Quantity<boost::mpl::vector<boost::mpl::at_c<DimensionsU, 0>::type,
			boost::mpl::at_c<Dimensions, 1>::type>> otherConverted = other;

		using outputDims = boost::mpl::transform<boost::mpl::at_c<Dimensions, 0>::type,
			boost::mpl::at_c<DimensionsU, 0>::type,
			boost::mpl::minus<boost::mpl::placeholders::_1, boost::mpl::placeholders::_2>>;

		return Quantity<boost::mpl::vector<outputDims, boost::mpl::at_c<Dimensions, 1>::type>>(
			get() / otherConverted.get());
	}
};

// TODO: literals quite boring might want to write a script
