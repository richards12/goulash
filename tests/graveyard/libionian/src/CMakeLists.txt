cmake_minimum_required( VERSION 3.24.0)

set(IONIAN_VERSION "1.0" CACHE STRING "Ionian Version")

project( "Ionian"
        LANGUAGES CXX HIP
        VERSION ${IONIAN_VERSION} )

find_package(HIP)

set( IONIAN_HIP_FLAGS "" CACHE STRING "Flags passed to HIP compiler")

add_library( ionian STATIC)

set_source_files_properties(SOURCE ionian.cpp PROPERTIES LANGUAGE HIP)
set_source_files_properties(SOURCE ionian.cpp PROPERTIES COMPILE_OPTIONS "${IONIAN_HIP_FLAGS}")

target_sources( ionian PRIVATE
              ionian.cpp)

install( TARGETS ionian DESTINATION lib EXPORT IonianTargets)
install( FILES ionian.h DESTINATION include)
install( EXPORT IonianTargets DESTINATION lib/cmake)
