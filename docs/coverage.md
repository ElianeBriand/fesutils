
# How to measure coverage

There is a coverage helper script that builds and run the tests, and produces a report. 

Typically, from your own machine, invoke it from the project root like this (`-t 15` is the number of thread, `-s 0` is to not send the statistics
to the coverage server - should only be done in CI, `-d 0` is to not only run documentation coverage test):

```
./runCoverage.sh -t 15 -s 0 -d 0

```

And for subsequent build:

```
rm -rf build_cov &&  ./runCoverage.sh -t 15 -s 0 -d 0
```

The build_cov directory is in the .gitignore so it is fine to leave this directory alone in between coverage build.

An HTML report is generated in `build_cov/fesutils_coverage/index.html`

# Coverage policy

It is desired to have 100% coverage as reported by the script, at all time, so as to have good visibility when commit do
 not contain enough tests. 
 
But of course, some parts of the codebase, like pure console output, are not easily testable,
and other parts are out of scope for unit testing (eg: runfile execution function) and should be tested in system and integration test rather
than unit test.


Thus exclusion from coverage should be used liberally, so long as:
  
* The reason for exclusion is stated in the coverage-excluding comment (see below)
* If the section is excluded for technical reason (difficulty to mock, pure console output), the code can reasonably be
considered correct by inspection
  * If possible, as much of the excluded code should be extracted to function that **can** be unit tested so as to limit
  the untested section
* If the section is excluded for scope reason, the commit introducing the code should have a suitable system test (see 
directory `/system_tests`) that exercice the code block.
* If the section is excluded as the check is purely defensive (state cannot be reached in current codebase, but could possibly
occurs if the function is called in other places), it is acceptable. However consider if extracting the section to be unit testable
would be useful.


## Format of the coverage excluding comment
```c++
// LCOV_EXCL_START
// Reason for coverage exclusion: Would in essence be an integration test, not unit test

/* Code to be excluded */

// LCOV_EXCL_STOP
```

The string `Reason for coverage exclusion:` should be kept intact for ease of grep-ing.

# Documentation coverage

To run the documentation coverage only:

```
./runCoverage.sh -t 15 -s 0 -d 1
```

This outputs a summary of the doxygen-document parts of the code.

No specific policy - for information purpose only, though making it higher is always appreciated.