#include "Enum.h"

MAKE_ENUM(TestEnum
	, (First, Second)
	, ("First", "Second"));

MAKE_ENUM(FindBoardMode
	, (Raw
		, Optimized
		, Assistant)
	, ("Raw"
		, "Optimized"
		, "Assistant"));

void testEnum() {
	TestEnum instance;
}
