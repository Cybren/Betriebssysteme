#pragma once

#ifdef __clang__
# define UNUSED_STRUCT_FIELD __attribute__((unused))
#else


# define UNUSED_STRUCT_FIELD
#endif

