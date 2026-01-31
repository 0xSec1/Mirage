add_executable(MirageLoader loader/main.cpp)
add_library(MirageCore SHARED 
    core/dllmain.cpp
    core/HookEngine.cpp
    core/detours.cpp
)

target_link_libraries(MirageCore PRIVATE minhook)
