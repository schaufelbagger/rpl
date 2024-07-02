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
                 model/rpl-header-option.cc
                 model/rpl-objective-function.cc
                 model/rpl-node.cc
                 model/rpl-routing-table.cc
                 model/rpl-ipv6-header.cc
    HEADER_FILES model/rpl.h
                 helper/rpl-helper.h
                 model/rpl-header.h
                 model/rpl-header-option.h
                 model/rpl-objective-function.h
                 model/rpl-node.h
                 model/rpl-routing-table.h
                 model/rpl-ipv6-header.h
    LIBRARIES_TO_LINK ${libinternet}
                      ${libwifi}
                      ${liblr-wpan}
                      ${libinternet-apps}
                      ${libsixlowpan}
                      ${libapplications}
    TEST_SOURCES test/rpl-test-suite.cc
                 test/rpl-regression-test-suite.cc
                 test/rpl-header-regression-test.cc
                 test/rpl-header-option-regression-test.cc
                 test/rpl-line-test.cc
                 ${examples_as_tests_sources}
)
    
