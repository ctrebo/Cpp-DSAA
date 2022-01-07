add_library(ArrayClass INTERFACE)
target_include_directories(ArrayClass INTERFACE include)

install(TARGETS ArrayClass DESTINATION lib)
install(FILES include/arrayClass.hpp DESTINATION include/)
