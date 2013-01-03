project (ZELOS_LIBPOS)

include_directories (
	${ZELOS_LIBPOS_SOURCE_DIR}/include
)

add_library (zelos_pos
	src/coordinates.cpp
)
