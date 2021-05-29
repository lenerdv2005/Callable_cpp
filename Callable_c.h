#include "equals.h"
#include <utility>

namespace concepts_and_traits {
	namespace Callable_c_detail {
		template<typename Type, typename... Arg_types>
		constexpr bool is_function = requires(Type function) {
			function(std::declval<Arg_types>()...);
		};

		template<bool, typename, typename...> struct Return_type_of_detail;
		template<typename Type, typename... Arg_types>
		struct Return_type_of_detail<false, Type, Arg_types...> { using type = Type; };
		template<typename Type, typename... Arg_types>
		struct Return_type_of_detail<true, Type, Arg_types...> {
			using type = decltype(std::declval<Type>()(std::declval<Arg_types>()...));
		};

		template<typename Type, typename... Arg_types>
		using Return_type_of = typename Return_type_of_detail<is_function<Type, Arg_types...>, Type, Arg_types...>::type;

		template<typename Type, typename> constexpr bool Callable_c = false;
		template<typename Type, typename Return_type, typename... Arg_types>
		constexpr bool Callable_c<Type, Return_type(Arg_types...)> =
			requires(Type function) {
				function(std::declval<Arg_types>()...);
			} &&
			equals<
				Return_type_of<Type, Arg_types...>,
				Return_type
			>
		;
	}

	template<typename Type, typename Return_type, typename... Arg_types>
	concept Callable_c =
		Callable_c_detail::Callable_c<Type, Return_type(Arg_types...)>
	;
}
