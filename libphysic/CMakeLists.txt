cmake_minimum_required(VERSION 2.6)

project (ZELOS_LIBPHYSIC)

include_directories (
	${ZELOS_LIBPOS_SOURCE_DIR}/include
	${ZELOS_LIBPHYSIC_SOURCE_DIR}/include
	${ZELOS_LIBPHYSIC_SOURCE_DIR}/include/physic
)

add_library (zelos_physic
	src/line.cpp
	src/model.cpp
	src/model_manager.cpp
	src/model_mesh.cpp
	src/model_mesh_collision.cpp
	src/navigation_cell.cpp
	src/navigation_entity.cpp
	src/navigation_mesh.cpp
	src/rectangle.cpp
	src/rtree.cpp
)

target_link_libraries (zelos_physic
	zelos_pos
)
