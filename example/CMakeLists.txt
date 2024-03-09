# for each "example/x.cpp", generate target "x"
file(GLOB_RECURSE all_examples *.cpp)
foreach(v ${all_examples})
    string(REGEX MATCH "example/.*" relative_path ${v})
    # message(${relative_path})
    string(REGEX REPLACE "example/" "" target_name ${relative_path})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    add_executable(${target_name} ${v} ../include/common.h ../include/common.h ../include/client.h ../include/common.h ../include/panic.h ../include/panic.h ../include/panic.h ../include/gtream.h ../include/bytes.h ../include/file.h ../include/list.h ../include/gand.h ../include/heap.h ../include/hashtable.h ../include/avl.h ../include/gset.h ../include/blocking_queue.h ../include/c_thread_pool.h ../include/threadsafe_queue.h ../include/cpp_thread_pool.h ../include/connect.h ../include/execute.h ../include/server.h)
endforeach()
