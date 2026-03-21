project(AeroSoft C)

## Libs
# Avl Map Lib
add_library(map-avl-lib map-avl.c)
target_include_directories(map-avl-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# List Map Lib
add_library(map-list-lib map-list.c)
target_include_directories(map-list-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Airport Manager Lib
add_library(airport-manager-lib airport-manager.c)
target_include_directories(airport-manager-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Executables
# Main (avl)
add_executable(aerosoft main.c)
target_link_libraries(aerosoft PRIVATE airport-manager-lib map-avl-lib)

# Old (list)
add_executable(aerosoft-old main.c)
target_link_libraries(aerosoft-old PRIVATE airport-manager-lib map-list-lib)

# Tests
# Test map avl implementation
add_executable(test-map-avl test-map.c)
target_link_libraries(test-map-avl PRIVATE map-avl-lib)
add_test(NAME TestMapAvl COMMAND test-map-avl)

# Test map list implementation
add_executable(test-map-list test-map.c)
target_link_libraries(test-map-list PRIVATE map-list-lib)
add_test(NAME TestMapList COMMAND test-map-list)

# Test Airport Manager
add_executable(test-airport-manager test-airport-manager.c)
target_link_libraries(test-airport-manager PRIVATE airport-manager-lib map-avl-lib)
add_test(NAME TestAirportManager COMMAND test-airport-manager)
