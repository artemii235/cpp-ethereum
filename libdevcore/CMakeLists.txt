file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(devcore ${sources} ${headers})

add_dependencies(devcore BuildInfo.h)

# Needed to prevent including system-level boost headers:
target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIR} PRIVATE ../utils)

target_link_libraries(devcore Boost::system)
