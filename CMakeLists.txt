check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources    
        #test/rpl-examples-test-suite.cc
        )
endif()    

build_lib(
    LIBNAME rpl
    SOURCE_FILES model/rpl.cc
                 helper/rpl-helper.cc 
                 model/rpl-header.cc
    HEADER_FILES model/rpl.h
                 helper/rpl-helper.h
                 model/rpl-header.h
    LIBRARIES_TO_LINK ${libinternet}
    TEST_SOURCES test/rpl-test-suite.cc
                 test/rpl-regression-test-suite.cc
                 test/rpl-hello-regression-test.cc
                 test/rpl-two-nodes-test.cc
                 ${examples_as_tests_sources}
)
    
