add_library(VectorClass INTERFACE)
target_include_directories(VectorClass INTERFACE include)

install(TARGETS VectorClass DESTINATION lib)
install(FILES include/vectorclass.hpp DESTINATION include/)
