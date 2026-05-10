#include <concepts>
#include "longest_inrceasing_prefix.hpp"

int main()
{
	using path = lis<1, 2, 3, 4, 5, 1, 2, 3>;

	static_assert(std::same_as<path, value_sequence<1, 2, 3, 4, 5>>, "x");

	return 0;
}
