add_library(winhttp SHARED
    mio-modding-api/dllmain.cpp
    mio-modding-api/mio-modding_api.h
)

find_package(PolyHook_2 CONFIG REQUIRED)
set_property(TARGET winhttp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
target_link_libraries(winhttp PRIVATE PolyHook_2::PolyHook_2)