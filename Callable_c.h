#include <type_traits>
#include <utility>

namespace concepts_and_types {
	namespace Callable_c_detail {
		// checks if Type is a function that can take Arg_types
		template<typename Type, typename... Arg_types>
		constexpr bool do_arg_types_match =
			// basically: can i pass Arg_types to Type as if it were a function and not get a compiler error?
			requires(Type function, Arg_types... arguments) {
				function(arguments...);
			}
		;

		template<bool, typename, typename...> struct Return_type_of_detail;
		template<typename Type, typename... Arg_types>
		struct Return_type_of_detail<false, Type, Arg_types...> {
			using type = Type; // just some placeholder type to avoid compiler errors; should not be used
		};
		template<typename Type, typename... Arg_types>
		struct Return_type_of_detail<true, Type, Arg_types...> {
			using type =
				decltype(std::declval<Type>()(std::declval<Arg_types>()...))
			;
			/* what it does:
				decltype( get the type of the following expression
					std::declval<Type>() creates a placeholder "instance" of the function to fake-call (doesn't get evaluated, it's just for examining the return type)
					(
						std::declval<Arg_types>()... creates placeholder values for each argument
						basically says:
							std::declval<Arg_types>() -> create a placeholder value
							... -> for each element in the pack (Arg_types), insert it in the previous expression and put them all together with commas between them
					)
				)
			*/
		};

		template<typename Type, typename... Arg_types>
		using Return_type_of = typename Return_type_of_detail<
			do_arg_types_match<Type, Arg_types...>, // needed to act as lazy evaluation because there is no "ternary operator" for template metaprogramming
			Type,
			Arg_types...
		>::type;

		template<typename Type, typename> constexpr bool Callable_c = false;
		template<typename Type, typename Return_type, typename... Arg_types>
		constexpr bool Callable_c<Type, Return_type(Arg_types...)> =
			do_arg_types_match<Type, Arg_types...> &&
			std::is_same_v<
				Return_type_of<Type, Arg_types...>,
				Return_type
			>
		;
	}

	// defaults to false, and may only be true for function types (here "function type" is used more semantically than literally)
	// adding one more layer to allow for easy overloading
	template<typename, typename>
	constexpr bool Callable_c_impl = false;
	template<typename Type, typename Return_type, typename... Arg_types>
	constexpr bool Callable_c_impl<Type, Return_type(Arg_types...)> =
		Callable_c_detail::Callable_c<Type, Return_type(Arg_types...)>
	;

	template<typename Type_to_check, typename Function_type>
	concept Callable_c =
		Callable_c_impl<Type_to_check, Function_type>
	;
}
