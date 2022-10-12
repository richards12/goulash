target_sources( hlib PRIVATE
              suskevi.cpp)

#set_source_files_properties(SOURCE suskevi.cpp PROPERTIES LANGUAGE HIP)
#set_source_files_properties(SOURCE suskevi.cpp PROPERTY COMPILE_FLAGS ${HIP_COMPILE_FLAGS})
