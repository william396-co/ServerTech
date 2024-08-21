1.5 Summary
• Move semantics allows us to optimize the copying of objects, where we no longer need the value. It can be
	used implicitly (for unnamed temporary objects or local return values) or explicitly (with std::move()).

• std::move() means I no longer need this value here. It marks the object as movable. An object marked
	with std::move() is not (partially) destroyed (the destructor still will be called).

• By declaring a function with a non-const rvalue reference (such as std::string&&), you define an
	interface where the caller semantically claims that it no longer needs the passed value. The implementer
	of the function can use this information to optimize its task by “stealing” the value or do any other
	modification with the passed argument. Usually, the implementer also has to ensure that the passed
	argument is in a valid state after the call.


• Moved-from objects of the C++ standard library are still valid objects, but you no longer know their
	value.
• Copy semantics is used as a fallback for move semantics (if copy semantics is supported). If there is no
	implementation taking an rvalue reference, any implementation taking an ordinary const lvalue reference
	(such as const std::string&) is used. This fallback is then used even if the object is explicitly marked
	with std::move().

• Calling std::move() for a const object usually has no effect.

• If you return by value (not by reference), do not declare the return value as a whole to be const.
