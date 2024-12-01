find_package(Boost COMPONENTS algorithm REQUIRED)
find_package(tinyxml2 REQUIRED)
add_executable(sitelist main.cpp)
target_link_libraries(sitelist PRIVATE Boost::algorithm tinyxml2::tinyxml2)
