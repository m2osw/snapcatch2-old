
<p align="center">
<img alt="advgetopt" title="CMake Module Extensions to Build Snap! C++."
src="https://snapwebsites.org/sites/snapwebsites.org/files/images/catch2-logo-small.png" width="502" height="242"/>
</p>

# Snap! Catch2

This package is used so we can have the latest version of Catch to run
along Snap! Websites. We  tried to move to Ubuntu 18.04 and had problems
to support 16.04 and 18.04 because it changes so much so quickly.

This package will allow us to upgrade all our projects to the latest
available quickly and have a consistant version of Catch through various
versions of Ubuntu.

# Additions

## Command Line Options

The snapcatch2 extension adds support for a few additional command line flags
that we do not have to manage in each one of our package:

* `-p` or `--progress` -- show progress when entering a section
* `--verbose` -- make the test more verbose
* `-S <value>` or `--seed <value>` -- force the random generator seed
* `-V` or `--version` -- print out version and exit

Note that the seed may not be used if the test never uses a random number.

## Initialization

By default, catch2 gives you a lot of freedom in the initialization process.
This is great, but at the same time, we noticed that we were repeating many
things (copy/paste is not too bad once, even twice, but when you have 20
projects, that's not good).

For that reason, we offer an initialization function. A snapcatch `main()`
if you'd like.

    int snap_catch2_main(
          char const * project_name
        , char const * project_version
        , int argc
        , char * argv[]
        , void (*init_callback)() = nullptr
        , Catch::clara::Parser (*add_user_options)(Catch::clara::Parser const & cli) = nullptr
        , int (*callback)(Catch::Session & session) = nullptr
        , void (*finished_callback)() = nullptr)

This allows us to initialize our Snap! tests in a symetrical way. We have
to specify a project name, the project version, the command line arguments,
and four different callbacks.

The `init_callback()` happens immediately upon calling this function.

After the system adds its options (see above) you have the ability to tweak
the CLI options further as your `add_user_options()` function gets called.
For example, if you need to support a path where temporary files are to be
created, you could add such in this callback.

The `callback()` is a function you'd like to run before we print the project
name and version. Most often it is used to add complementary initialization
to the test session.

Once the tests ran, `finished_callback()` is called. Here you can do any
kind of cleanup you'd like to do. For example, you could delete temporary
files.

### Verbose Flag

When the `--verbose` flag is used on the command line, the corresponding
variable is set to true.

To check whether the variable is true or false from your tests, use the
`g_verbose()` function.

Note that the `g_verbose()` function returns a reference to the variable,
meaning you can modify it if useful (i.e. you can forcibly eliminate
verbosity in a test). You are responsible for restoring the value once
your test is done.

### Progress Flag

When the `--progress` flag is used on the command line, the corresponding
variable is set to true.

To check whether the variable is true or false from your tests, use the
`g_progress()` function.

Note that the `g_progress()` function returns a reference to the variable,
meaning you can modify it if useful. You are responsible for restoring the
value once your test is done.

## Namespace

The snapcatch2 header adds a namespace for you to put your variable
declarations and pretty much anything else you want to have in your
tests.

Here is how you use it:

    namespace SNAP_CATCH2_NAMESPACE
    {
      ...
      int g_my_var = 3;
    }

    # In your tests
    SNAP_CATCH2_NAMESPACE::g_my_var

By default, the snapcatch2.hpp header offers:

    SNAP_CATCH2_NAMESPACE::g_progress()
    SNAP_CATCH2_NAMESPACE::g_verbose()
    SNAP_CATCH2_NAMESPACE::snap_catch2_main()
    SNAP_CATCH2_NAMESPACE::catch_compare_long_strings()

in the namespace.

The `g_progress()` and `g_verbose()` are flags handled through a function.

## Additional Macros

### Start/End Sections

We added a section start/end delimiter like so:

    CATCH_START_SECTION(name)
    {
        // your usual catch2 section
    }
    CATCH_END_SECTION()

This allows us to verbose on each section so we know exactly where the errors
we get happen. Without that and when you have many sections, it's really
difficult to find your way quickly.

### Long Strings

We often manage very long strings, especially when dealing with HTML and XML.
The default catch macro handling string is going to diplay the entire strings
(the good and the bad ones) and good luck to find what's wrong with it.

We added a function which compares the strings and only displays differences
and a few characters before after. This makes it really easy to find the
differences, even if it's just one period, a space at the end of a line, etc.

    CATCH_REQUIRE_LONG_STRING(a, b)

This is the same as `CATCH_REQUIRE(a, b)` when `a` and `b` are long strings.
Note that you can also use this with short strings. It's probably not as
useful with such, though.

## Exception Watcher

The `ExceptionWatcher` class is used to check the message of exceptions.
The default Catch2 exception handler does not offer a way to compare the
exception message and the exception type at the same time. Not only that
we expect some messages to be very long so we use the
`catch_compare_long_strings()` to display the message in verbose mode.

# Building

## Within the Snap! Websites Environment

In our environment, we have the source in a tarball and a patch.
We extract the tarball, run the make of Catch, then the install target.
Once that is done, our own CMakeLists.txt can install the results so
it can get installed in our Debian environment.

## cmake files

Note that the Catch people somehow install the the cmake files under
`/usr/lib/cmake/...`. I moved those to `/usr/shared/cmake/...` because
that's where I see those files under Ubuntu.

## Compiler Version

By default it compiles with C++11.

Use `-DUSE_CPP17` to make it compile with C++17.

Use `-DUSE_CPP14` to make it compile with C++14.

## Patch

At first, I had [two problems compiling version
2.7.2](https://github.com/catchorg/Catch2/issues/1636).

1. Building in an outside BUILD folder would fail with some "could not find
this or that".
2. Building in the root source folder would also fail (although that in itself
was not a bad thing).

The only way to resolve the problem was to create a build directory inside
the source tree and run `cmake` there. Newer version (post 2.7.2) will work
as expected (I've already tested the newest from the git and it worked.)

So, I resolved the two problems by following the steps from an old Catch2
documentation file which was to create a `projects/Generated` sub-directory
and building in there.

### How to Create a Patch

Just in case I wanted to keep the command line to create a patch since
our build knows how to automatically apply it from a catch2 tarball:

    LC_ALL=C TZ=UTC0 diff -Naur Catch2-2.7.2 Catch2-2.7.2a >Catch2-2.7.2.patch

Change the version to the current Catch2 version tarball, of course.


## Build & Install of Catch itself

To build, I run:

    rm -rf Catch2-2.7.2
    tar xf catch2-2.7.2.tar.gz
    patch Catch2-2.7.2/projects/CMakeLists.txt <Catch2-2.7.2.patch
    cd Catch2-2.7.2
    cmake -DCATCH_BUILD_TESTING=off-DCATCH_INSTALL_HELPERS=on -DCATCH_INSTALL_DOCS=on -DCMAKE_INSTALL_PREFIX=... .
    make install

The `patch` command is run only if a patch is defined. The version can change
over time.


# Bugs

Submit bug reports and patches to the packaging on
[snapcatch2 issues on github](https://github.com/m2osw/snapcatch2/issues).

Bugs and patches to the Catch2 environment itself, please make sure
to use the catch2 issue queue:
[catch2 issues on github](https://github.com/catchorg/Catch2/issues)


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
