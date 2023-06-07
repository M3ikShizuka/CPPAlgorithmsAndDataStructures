#pragma once

#if __cpp_constexpr >= 201907L	// Check for a specific version of a feature
#	define CONSTEXPR20 constexpr
#else
#	define CONSTEXPR20 inline
#endif

#ifdef __has_cpp_attribute							// Check if __has_cpp_attribute is present
//#	if __has_cpp_attribute(nodiscard) >= 201907L	// Check for an attribute
//#   	define NODISCARD(msg) [[nodiscard(msg)]]
//# elif
#	if __has_cpp_attribute(nodiscard) >= 201603L
#		define NODISCARD [[nodiscard]]
#	else
#		define NODISCARD
#	endif
#endif