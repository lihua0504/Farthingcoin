Farthing Coin
===============================

https://www.farthingcoin.one/


What is FarthingCoin?
----------------


Farthing Coin, English name FarthingCoin, codename FT,
It is a new generation of digital currency based on blockchain
technology. It is an open source cryptocurrency.
FarthingCoin is a digital bank known by the UK
as a financial unicorn in OakNorth
And the high protection, high benchmark,
A high-value, preservable digital currency.



For more information, as well as an immediately useable, binary version of the
Futuro core software, see https://www.farthingcoin.one/.


License
-------

Futuro core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is meant to be stable. Development is normally done in separate branches.
[Tags](https://github.com/lihua0504/Farthingcoin/tags) are created to indicate new official,
stable release versions of Farthingcoin Core.


Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](/doc/unit-tests.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`


### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

