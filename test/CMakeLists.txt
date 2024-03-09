# for each "test/x.cpp", generate target "x"
file(GLOB_RECURSE all_tests *.cpp)
foreach(v ${all_tests})
    string(REGEX MATCH "test/.*" relative_path ${v})
    # message(${relative_path})
    string(REGEX REPLACE "test/" "" target_name ${relative_path})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    add_executable(${target_name} ${v} ../include/client.h ../include/common.h ../include/panic.h ../include/panic.h ../include/gtream.h ../include/bytes.h ../include/file.h ../include/list.h ../include/gand.h ../include/heap.h ../include/hashtable.h ../include/avl.h ../include/gset.h ../include/blocking_queue.h ../include/c_thread_pool.h ../include/threadsafe_queue.h ../include/cpp_thread_pool.h ../include/connect.h ../include/execute.h ../include/server.h)
endforeach()
