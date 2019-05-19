
## Catch2 CMakeLists.txt understood flags

### Coverage

Use `CATCH_ENABLE_COVERAGE` to enable the coverage test.

### Compiler Version

By default it compiles with C++11.

Use `-DUSE_CPP17` to make it compile with C++17.

Use `-DUSE_CPP14` to make it compile with C++14.

## Patch

In version 2.7.2 there is a bug in the CMakeLists.txt so I created a patch:

    LC_ALL=C TZ=UTC0 diff -Naur Catch2-2.7.2 Catch2-2.7.2a >Catch2-2.7.2.patch

To build, I run:

    rm -rf Catch2-2.7.2
    tar xf catch2-2.7.2.tar.gz
    patch Catch2-2.7.2/projects/CMakeLists.txt <Catch2-2.7.2.patch
    cd Catch2-2.7.2
    cmake -DUSE_CPP14 -DCMAKE_INSTALL_PREFIX=... .
    make install

## Files found in the Ubuntu official version of catch (v1.x)

catch: /usr/include/catch.hpp
catch: /usr/share/doc/catch/README.md
catch: /usr/share/doc/catch/changelog.Debian.gz
catch: /usr/share/doc/catch/copyright
catch: /usr/share/doc/catch/docs/Readme.md
catch: /usr/share/doc/catch/docs/assertions.md
catch: /usr/share/doc/catch/docs/build-systems.md
catch: /usr/share/doc/catch/docs/command-line.md.gz
catch: /usr/share/doc/catch/docs/configuration.md.gz
catch: /usr/share/doc/catch/docs/contributing.md
catch: /usr/share/doc/catch/docs/logging.md
catch: /usr/share/doc/catch/docs/own-main.md
catch: /usr/share/doc/catch/docs/slow-compiles.md
catch: /usr/share/doc/catch/docs/test-cases-and-sections.md
catch: /usr/share/doc/catch/docs/test-fixtures.md
catch: /usr/share/doc/catch/docs/tostring.md
catch: /usr/share/doc/catch/docs/tutorial.md.gz
catch: /usr/share/doc/catch/docs/whats-changed.md
catch: /usr/share/doc/catch/docs/why-catch.md


