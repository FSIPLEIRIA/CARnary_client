# Check if this file has already been included.
if(NOT TARGET CARnary_lib)
    # Define the target.
    add_library(CARnary_lib SHARED IMPORTED)
    set_target_properties(CARnary_lib PROPERTIES IMPORTED_LOCATION /usr/lib/CARnary_lib/libCARnary_lib.so)

    target_include_directories(CARnary_lib INTERFACE /usr/include/CARnary_lib)
endif()
