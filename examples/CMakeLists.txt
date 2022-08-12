build_lib_example(
    NAME rpl-example
    SOURCE_FILES rpl-example.cc
    LIBRARIES_TO_LINK ${librpl}
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
  NAME rpl-10-nodes-line-example
  SOURCE_FILES rpl-10-nodes-line-example.cc
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
  NAME rpl-on-off-example
  SOURCE_FILES rpl-on-off-example.cc
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