add_executable(server main.cc server.cc worker.cc)
target_link_libraries(
    server
    tools
)
