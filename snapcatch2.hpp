/*
 * Copyright (c) 2019  Made to Order Software Corp.  All Rights Reserved
 *
 * https://snapwebsites.org/
 * contact@m2osw.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#pragma once

#include <catch2/catch.hpp>

// C++ lib
//
#include <iostream>

// C lib
//
#include <string.h>


/** \brief Namespace declaration.
 *
 * You are expected to reuse this name each time you create a common
 * function, class, etc. For example the following creates a counter
 * which is common to all your tests:
 *
 * \code
 *     namespace SNAP_CATCH2_NAMESPACE
 *     {
 *     
 *     int g_counter = 0;
 *     
 *     }
 * \endcode
 */
#define SNAP_CATCH2_NAMESPACE       unittest



/** \brief The snapcatch2 definitions.
 *
 * This namespace encompasses the extensions we offer to catch2. We reuse
 * these in pretty much all our tests so to make it easy we made it a
 * common header file.
 */
namespace SNAP_CATCH2_NAMESPACE
{



/** \brief Print out progress as a section gets executed.
 *
 * This flag is used along the CATCH_START_SECTION() and CATCH_END_SECTION()
 * macros. The flag gets set whenever the `--progress` command line option
 * is used and the macros print out the name of the section being run
 * (assuming you use our macros).
 *
 * This is very useful to have whenever you have very many and rather
 * slow tests to see which one fails because otherwise you may get
 * some invalid line number/names especially when you have an unexpected
 * exception or some similar error.
 *
 * You are welcome to use the parameter in your own code. Just make sure
 * to use the parenthesis:
 *
 * \code
 *     if(g_progress())
 *     {
 *         std::cout << "we're moving!" << std::endl;
 *     }
 * \endcode
 *
 * \return A read-write reference to the `progress` parameter.
 */
inline bool& g_progress()
{
    static bool progress = false;

    return progress;
}


/** \brief Print out information to let programmers know what tests are doing.
 *
 * This flag is used whenever useful information could be outputted. The
 * flag let the test know whether to print that information or not.
 *
 * You are welcome to use the parameter in your own code. Just make sure
 * to use the parenthesis:
 *
 * \code
 *     if(g_verbose())
 *     {
 *         std::cout << "More info about this test!" << std::endl;
 *     }
 * \endcode
 *
 * \return A read-write reference to the `verbose` parameter.
 */
inline bool& g_verbose()
{
    static bool verbose = false;

    return verbose;
}


#ifdef CATCH_CONFIG_RUNNER
/** \brief The main function to initialize and run the unit tests.
 *
 * This inline function initializes and runs the snapcatch2 tests.
 *
 * The \p init_callback parameter is used to initialize your test before
 * we parse the command line. Use the \p callback function to do further
 * initialization after the command line is parsed and basic functions
 * run (i.e. the `--version` command line option will run and the
 * function exit before \p callback gets called. So when the user uses
 * that command line option, you do not get \p callback called, however,
 * \p init_callback will always be called.)
 *
 * We do it this way because the function will try/catch std::exception
 * so if an exception occurs in your \p init_callback, it will always
 * be captured and a message printed out before the application
 * exits cleanly with an error code (i.e. `exit(1)`).
 *
 * The \p init_callback can be set to a nullptr if you do not need to
 * initialize anything futher early on.
 *
 * One thing we use the \p init_callback for is to prevent our
 * libexcept exceptions from generating a stack trace because that's
 * very slow and in a test which generate thousands if not million
 * of exception cases, it makes it really very slow.
 *
 * \code
 *     snap_catch2_main(
 *           "libutf8"
 *         , "1.2.3"
 *         , argc
 *         , argv
 *         , []() { libexcept::set_collect_stack(false); }
 *     );
 * \endcode
 *
 * The \p add_user_options parameter is a pointer to a function which
 * adds command line options supported by your test. This pointer can
 * be set to nullptr, For example, if you wanted to support a path to
 * a temporary directory, you would write:
 *
 * \code
 *     namespace SNAP_CATCH2_NAMESPACE
 *     {
 *
 *     std::string g_tmpdir= std::string("/tmp");
 *
 *     Catch::clara::Parser add_command_line_options(Catch::clara::Parser const & cli)
 *     {
 *         return cli
 *              | Catch::clara::Opt(g_tmpdir, "tmpdir")
 *                   ["--tmpdir"]
 *                   ("a path to a temporary directory used by the test.");
 *     }
 *
 *     }
 *
 *     ...snip...
 *
 *     snap_catch2_main(
 *           "libutf8"
 *         , "1.2.3"
 *         , argc
 *         , argv
 *         , nullptr
 *         , &add_command_line_options
 *     );
 * \endcode
 *
 * \warning
 * It is possible to add a one letter command line option such as ["-t"],
 * however, catch2 already uses most of these options. The `-t` is used
 * to list tags, for example.
 *
 * The \p callback function gets called just before the tests get run.
 * In other words, we give you one more chance to verify command line
 * parameters or do additional initilalizations, now that you have access
 * to the user defined command line parameters. We pass the session to
 * the function, although in many cases you will not use it because your
 * parameters are already set as expected.
 *
 * \code
 *     namespace SNAP_CATCH2_NAMESPACE
 *     {
 *
 *     int finish_init(Catch::Session & session)
 *     {
 *         std::string cmd("rm -rf ");
 *         cmd += g_tmpdir;
 *         cmd += "/my-project";
 *         if(system(cmd.c_str()) != 0)
 *         {
 *             perror(cmd.c_str());
 *             return 1;
 *         }
 *
 *         return 0;
 *     }
 *
 *     }
 *
 *     ...snip...
 *
 *     snap_catch2_main(
 *           "libutf8"
 *         , "1.2.3"
 *         , argc
 *         , argv
 *         , nullptr
 *         , nullptr
 *         , &finish_init
 *     );
 * \endcode
 *
 * Once the tests ran, we call the \p finished_callback as well. This gives
 * you the ability to test futher things such as making sure that everything
 * was cleaned up, resources released, etc. For example, in our advgetopt
 * we use a FIFO to handle error messages. If an error never occurs that
 * FIFO will not be empty on exit. This is actually an error in the test.
 * Only errors that do occur have to be added to the FIFO. So on exit we
 * want to verify that the FIFO is indeed empty.
 *
 * \param[in] project_name  The name of the project (i.e. "libutf8").
 * \param[in] project_version  The version string of the project
 *                     (i.e. "LIBUTF8_VERSION_STRING").
 * \param[in] argc  The number of arguments on the command line.
 * \param[in] argv  An array of strings with the command line arguments.
 * \param[in] init_callback  Callback called before anything else.
 * \param[in] add_user_options  A callback giving you the opportunity to add
 *                              command line options before argv gets parsed.
 * \param[in] callback  A callback called just before we run the tests.
 * \param[in] finished_callback  The callback called just after all the tests
 *                               ran.
 *
 * \return The exit code, usually 0 on success and 1 on an error.
 */
inline int snap_catch2_main(
          char const * project_name
        , char const * project_version
        , int argc
        , char * argv[]
        , void (*init_callback)() = nullptr
        , Catch::clara::Parser (*add_user_options)(Catch::clara::Parser const & cli) = nullptr
        , int (*callback)(Catch::Session & session) = nullptr
        , void (*finished_callback)() = nullptr)
{
    typedef unsigned int seed_t; // see `man srand(3)` for type

    try
    {
        if(init_callback != nullptr)
        {
            init_callback();
        }

        Catch::Session session;

        bool version(false);
        seed_t seed(static_cast<seed_t>(time(NULL)));

        auto cli = session.cli()
                 | Catch::clara::Opt(seed, "seed")
                    ["-S"]["--seed"]
                    ("value to seed the randomizer, if not specified, randomize")
                 | Catch::clara::Opt(g_progress())
                    ["-p"]["--progress"]
                    ("print name of test section being run")
                 | Catch::clara::Opt(g_verbose())
                    ["--verbose"]
                    ("print additional information from within our own tests")
                 | Catch::clara::Opt(version)
                    ["-V"]["--version"]
                    ("print out the libutf8 library version these unit tests pertain to");

        if(add_user_options != nullptr)
        {
            cli = add_user_options(cli);
        }

        session.cli(cli);

        if(session.applyCommandLine(argc, argv) != 0)
        {
            std::cerr << "Error in command line." << std::endl;
            return 1;
        }

        if(version)
        {
            std::cout << project_version << std::endl;
            return 0;
        }

        // by default we get a different seed each time; that really helps
        // in detecting errors! At least it helped me many times.
        //
        srand(seed);

        if(callback != nullptr)
        {
            int const e(callback(session));
            if(e != 0)
            {
                return e;
            }
        }

        std::cout << project_name
                  << " v"
                  << project_version
                  << " ["
                  << getpid()
                  << "]:unittest: seed is "
                  << seed
                  << std::endl;

        auto const r(session.run());

        if(finished_callback != nullptr)
        {
            finished_callback();
        }

        return r;
    }
    catch(std::logic_error const & e)
    {
        std::cerr << "fatal error: caught a logic error in "
                  << project_name
                  << " unit tests: "
                  << e.what()
                  << "\n";
        return 1;
    }
    catch(std::runtime_error const & e)
    {
        std::cerr << "fatal error: caught a run-time error in "
                  << project_name
                  << " unit tests: "
                  << e.what()
                  << "\n";
        return 1;
    }
    catch(std::exception const & e)
    {
        std::cerr << "fatal error: caught a standard exception in "
                  << project_name
                  << " unit tests: "
                  << e.what()
                  << "\n";
        return 1;
    }
}
#endif



inline void catch_compare_long_strings(std::string const & a, std::string const & b)
{
    auto print_char = [](char c)
    {
        if(c == 0x20)
        {
            // Possible characters to make spaces visible are:
            //      U+23B5      BOTTOM SQUARE BRACKET
            //      U+2420      SYMBOL FOR SPACE
            //      U+2423      OPEN BOX
            //
            std::cout << "\xE2\x90\xA3";
        }
        else if(static_cast<unsigned char>(c) < 0x20)
        {
            // control character
            //
            std::cout << "^" << static_cast<char>(c + 0x40);
        }
        else if(static_cast<unsigned char>(c) < 0x80 || static_cast<unsigned char>(c) > 0x9F)
        {
            // standard character
            //
            std::cout << c;
        }
        else
        {
            // graphical control
            //
            std::cout << "@" << static_cast<char>(c - 0x40);
        }
    };

    if(a == b)
    {
        return;
    }

    std::cout << "error: long strings do not match.\n"
              << "---------------------------------------------------\n";

    // TODO: we may want to look into supporting UTF-8 properly
    //
    size_t const max(std::min(a.length(), b.length()));
    bool err(false);
    for(size_t idx(0); idx < max; ++idx)
    {
        if(a[idx] == b[idx])
        {
            if(err)
            {
                err = false;
                std::cout << "\033[0m";
            }
            std::cout << a[idx];
        }
        else
        {
            if(!err)
            {
                err = true;
                std::cout << "\033[7m";
            }
            std::cout << "[";
            print_char(a[idx]);
            std::cout << "/";
            print_char(b[idx]);
            std::cout << "]";
        }
    }
    if(err)
    {
        err = false;
        std::cout << "\033[0m";
    }

    std::cout << std::endl
              << "---------------------------------------------------" << std::endl;

    if(a.length() > b.length())
    {
        std::cout << "Left hand side string is longer ("
                  << a.length()
                  << " versus "
                  << b.length()
                  << ")."
                  << std::endl;
    }
    else if(b.length() > a.length())
    {
        std::cout << "Right hand side string is longer ("
                  << a.length()
                  << " versus "
                  << b.length()
                  << ")."
                  << std::endl;
    }

    // to generate the standard error too
    //
    CATCH_REQUIRE(a == b);
}




} // SNAP_CATCH2_NAMESPACE namespace


/** \brief Start a new section.
 *
 * This macro is an _overload_ of the CATCH_SECTION() macro defined in
 * catch2.hpp. It is used so we can print out the name of the section
 * whenever it executes and g_progress is true (i.e. the `--progress`
 * command line option was specified.)
 *
 * \code
 *     CATCH_START_SECTION("my section name/description here")
 *         break_something(1, 2, 3);
 *     CATCH_END_SECTION()
 * \endcode
 *
 * As you can see, you want to end the section with the CATCH_END_SECTION()
 * macro.
 *
 * \note
 * You do not need to add a block like you have to do with CATCH since
 * the macros add them for you.
 *
 * \note
 * The older version of Catch supported a description for a section.
 * Now that's useless so we only offer a name here.
 *
 * \param[in] name  The name of the section.
 */
#define CATCH_START_SECTION(name) \
    CATCH_SECTION(name) \
    { \
        if(SNAP_CATCH2_NAMESPACE::g_progress()) \
        { \
            std::cout << "SECTION: " << name << std::endl; \
        }

/** \brief End a section.
 *
 * This macro ends a section. The CATCH_START_SECTION() creates a
 * block which this macro closes.
 */
#define CATCH_END_SECTION() \
    }


/** \brief Require that two long strings be equal.
 *
 * When testing with really long strings, the compare failing will give
 * you both output but it's pretty much impossible to see the errors.
 *
 * Here we call our own function which will highlight the problems
 * so that way you can immediately see where the first difference occurs.
 *
 * \note
 * This does not make use of the standard catch environment so it will
 * eventually throw or break in some other ways which may not end up
 * well.
 *
 * \param[in] a  The first string.
 * \param[in] b  The second string.
 */
#define CATCH_REQUIRE_LONG_STRING(a, b) SNAP_CATCH2_NAMESPACE::catch_compare_long_strings(a, b)



namespace Catch
{
namespace Matchers
{


class ExceptionWatcher
    : public MatcherBase<std::exception>
{
public:
    ExceptionWatcher(std::string const & expected_message, bool verbose)
        : m_expected_message(expected_message)
        , m_verbose(verbose)
    {
    }

    /** \brief Check whether we got a match.
     *
     * This function compares the expected string with the actual exception
     * what() output.
     */
    bool match(std::exception const & e) const override
    {
        if(m_verbose)
        {
            SNAP_CATCH2_NAMESPACE::catch_compare_long_strings(e.what(), m_expected_message);
        }
        return e.what() == m_expected_message;
    }

    /** \brief Describe this matcher.
     *
     * This function produces a string describing what this matcher does.
     *
     * \return The description of this matcher.
     */
    virtual std::string describe() const override
    {
        return "compare the exception what() message with \""
             + m_expected_message
             + "\".";
    }

private:
    std::string     m_expected_message = std::string();
    bool            m_verbose = false;
};


inline ExceptionWatcher ExceptionMessage(std::string const & expected_message, bool verbose = false)
{
    return ExceptionWatcher(expected_message, verbose);
}



}
// Matchers namespace
}
// Catch namespace


// vim: ts=4 sw=4 et
