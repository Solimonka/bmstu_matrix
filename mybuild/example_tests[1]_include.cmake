if(EXISTS "/home/sonik/CLionProjects/matrix/mybuild/example_tests[1]_tests.cmake")
  include("/home/sonik/CLionProjects/matrix/mybuild/example_tests[1]_tests.cmake")
else()
  add_test(example_tests_NOT_BUILT example_tests_NOT_BUILT)
endif()