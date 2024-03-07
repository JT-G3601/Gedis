# for each "src/x.cpp", generate target "x"
file(GLOB_RECURSE all_srcs CONFIGURE_DEPENDS *.cpp)
foreach(v ${all_srcs})
    string(REGEX MATCH "src/.*" relative_path ${v})
    # message(${relative_path})
    string(REGEX REPLACE "src/" "" target_name ${relative_path})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    add_executable(${target_name} ${v} ../include/client.h ../include/common.h ../include/panic.h ../include/panic.h ../include/gtream.h ../include/bytes.h ../include/file.h ../include/list.h ../include/gand.h ../include/heap.h ../include/hashtable.h ../include/avl.h ../include/gset.h)
endforeach()
