# RPL

A module which adds the IPv6 Routing Protocol for Low-Power and Lossy Networks (RPL) to ns-3.
The module was developed in the ns-3 version 3.41.
An overview of the module and results of the examples are provided in this [thesis](https://repositum.tuwien.at/handle/20.500.12708/198289).

## How to use

Just clone it into the ns-3 src or contrib folder with:
```bash
git clone git@bitbucket.org:tu_works/rpl.git
```
and run a ns-3 configure like:
```bash
./ns3 configure --build-profile=debug --enable-examples --enable-tests --disable-python
```

To check if RPL works the tests can be run with:
```bash
./ns3 run "test-runner --suite=rpl-reg --out=./test_result.txt"
./ns3 run "test-runner --suite=rpl --out=./test_result.txt"
```

To run the examples the following commands can be used:
```bash
NS_LOG="Rpl=level_warn|prefix_all" ./ns3 run "rpl-line-example --run=1 --numberOfNodes=3" --cwd=contrib/tests/rpl-line-example
NS_LOG="Rpl=level_warn|prefix_all" ./ns3 run "rpl-moving-node-example --run=1" --cwd=contrib/tests/moving_node
NS_LOG="Rpl=level_warn|prefix_all" ./ns3 run "rpl-alt-path-example --run=1" --cwd=contrib/tests/alt-path
NS_LOG="Rpl=level_warn|prefix_all" ./ns3 run "rpl-many-nodes-example --run=1 --numberOfNodes=200 --appSetup=1" --cwd=contrib/tests/many_nodes
NS_LOG="Rpl=level_warn|prefix_all" ./ns3 run "rpl-energy-example --run=1" --cwd=contrib/tests/energy
NS_LOG="Rpl=level_warn|prefix_all" ./ns3 run "rpl-tree-example --run=1" --cwd=contrib/tests/tree
```

## Limitations

The ICMP RPL Packet header is still split into multiple ns-3 headers, which leads to the generation of wrong checksums, if not set to zero.

Also this implementation is missing a few things from the [RFC 6550](https://www.rfc-editor.org/rfc/rfc6550.txt).
The biggest ones are:
* the DODAG version does not increase
* multi-path, multicast and non-storing mode are not implemented.
* more optional option headers are not handled and currently the program then aborts when received.

## License
[GPL2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt)