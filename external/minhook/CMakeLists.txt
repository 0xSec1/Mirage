add_library(minhook STATIC
    src/buffer.c
    src/hook.c
    src/trampoline.c
    src/hde/hde32.c
    src/hde/hde64.c
)

target_include_directories(minhook PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
