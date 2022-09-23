build_lib_example(
    NAME rpl-example
    SOURCE_FILES rpl-example.cc
    LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
    ${libenergy}

)

build_lib_example(
  NAME rpl-3-nodes-example
  SOURCE_FILES rpl-3-nodes-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-3-nodes-ping-example
  SOURCE_FILES rpl-3-nodes-ping-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-line-example
  SOURCE_FILES rpl-line-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-tree-example
  SOURCE_FILES rpl-tree-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-alt-path-example
  SOURCE_FILES rpl-alt-path-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-many-nodes-example
  SOURCE_FILES rpl-many-nodes-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-moving-node-example
  SOURCE_FILES rpl-moving-node-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
)

build_lib_example(
  NAME rpl-energy-example
  SOURCE_FILES rpl-energy-example.cc
  LIBRARIES_TO_LINK
    ${librpl}
    ${libnetwork}
    ${libsixlowpan}
    ${libinternet}
    ${libinternet-apps}
    ${libenergy}
)