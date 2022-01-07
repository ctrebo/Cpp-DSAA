add_library(lList INTERFACE)
target_include_directories(lList INTERFACE include)

install(TARGETS lList DESTINATION lib)
install(FILES include/lList.hpp DESTINATION include/)
