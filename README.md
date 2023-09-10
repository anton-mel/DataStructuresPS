# Automated Testing Script

### Author: Alan Weide

## Requirements
* Python version 3.10 or greater
* The python `gradscope_utils` package, version 0.5.0 or greater

From a virtual environment configured with Python version 3.10 or greater, run the following command from this directory to install the dependencies for this project:
```bash
(my-3.10-venv) $ pip install -r requirements.txt
```

## Introduction
This directory contains a Python script, `run_tests.py`, that generates and runs a series of `unittest` test cases based on input/output pair files in a specified directory.
It also contains another script, `generateouts.py`, which may be used to generate correct output files based on a series of test input files, according to the output from a reference executable.
The other Python scripts in this directory are helper scripts for the `run_tests` program; take a look at them if you're curious about how this all works.

## Usage
Test cases are stored in a directory with the following structure:
```
[test_directory]/
    visible/ (optional)
        v001/ (optional)
            args (optional)
            in (optional)
            doc (optional)
            out
            err
            status
        v002/ (optional)
            (same as v001)
        ...
    hidden/ (optional)
        h001/ (optional)
            args (optional)
            in (optional)
            doc (optional)
            out
            err
            status
        h002/ (optional)
            (same as h001)
        ...
    ...
```

Each directory name at the first level (`visible`, `hidden`, _etc._) must have a name matching one of the visibility levels supported by Gradescope (of which "visible" and "hidden" are most common).
Each directory in one of the visibility-level directories contains the files needed for a single test case:
* `args` (optional), containing the command-line arguments for this test case, with one argument per line
* `in` (optional), containing the content to redirect to `stdin` when running this test case
* `doc` (optional), containing a descriptive name for the test case, when the exact command that is run is insufficient for whatever reason
* `out`, containing the correct contents of `stdout` for this test case
* `err`, containing the correct contents of `stderr` for this test case
* `status`, containing the correct exit code for this test case

To run the test script, execute the following command from this directory using a Python virtual environment configured with at least Python version 3.10:
```bash
(my-3.10-venv) $ python run_tests.py /path/to/your/executable test-directory
```

For example, if you would like to run the test script for the "Ahoy, Matey!" assignment and your executable is in the folder `~/cs223/psets/pset3`, you would execute the following command from this directory:
```bash
(my-3.10-venv) $ python run_tests.py ~/cs223/psets/pset3/HookBook hookbook_tests
```

## Understanding the Output
The output of `run_tests.py` is in the default format for the Python standard library `unittest` module's `TextTestRunner`.
If all tests pass, the output will look something like this:
```text
.........
----------------------------------------------------------------------
Ran 9 tests in 0.936s

OK
```

If your executable fails a test, you will see additional output that may look like the following:
```
======================================================================
FAIL: test_visible_v000 (create_tests.TestCaseGenerator.__init__.<locals>.Generated)
.../HookBook .../names.txt < hookbook_tests/visible/v000/in
----------------------------------------------------------------------
Traceback (most recent call last):
  File ".../create_tests.py", line 100, in method
    self.assertMultiLineEqual(uut_output, ref_output)
AssertionError: 'jack sparrow\william turner\elizabeth swan\n' != 'elizabeth swan\njack sparrow\nwilliam turner\n'
+ elizabeth swan
  jack sparrow
  william turner
- elizabeth swan
```

The important part starts on line 7 ("`self.assertMultiLineEqual(uut_output, ref_output)`"), which explains that the output produced by your program ("`uut_output`", on the lable of the inequality) differs from the correct output ("`ref_output`", on the value of the inequality).
The following lines contain the individual diff markers, with lines beginning with a `+` being lines that appear in the correct output but not yours, and lines beginning with a `-` appearing in your output but not the correct output.
Lines that have neither a `+` nor `-` appear in both files, but possibly at different line numbers.

Other errors may occur and your test case may fail for multiple reasons.
Read the output from the failing test carefully&mdash;the answer to your question is in there somewhere.
It might also behoove you to look at the source code for this test script if you're really convinced there's a bug there and not in your solution.

## Viewing and Adding Tests
The test cases we distribute for any given assignment are public and you can see them in the `[pset-name]_tests` directory.
That directory is structured as defined above, with visibility-level directories containing individual test cases.

Within each test case directory (_e.g._, `visible/v000`), there are up to 6 relevant files (and maybe some additional ones):
* `args`, if present, contains the command-line arguments for this test case, with one argument per line
* `in`, if present, contains the contents of the `stdin` stream used in this test case
* `out` contains the contents of the `stdout` stream used as the "correct" output for the test case
* `err` contains the contents of the `stderr` stream used as the "correct" error output for the test case
* `status` contains the correct exit code for this test case
* `doc`, if present, contains a description of what this test case does, if the command used to run the test case is for some reason insufficient

### Adding Test Cases
You are free to increase the breadth of the test suite by adding new cases.
You may do so by creating new directories within the `visible` directory in the appropriate `[pset-name]_tests` directory.
These new directories can be named anything.

Within each new test case directory you create, you must create at least an `args` or `in` file (or both) representing the input for the test case.
Thereafter, if you have access to a reference executable, you may use the `generateouts.py` script to generate the `out`, `err`, and `status` files automatically for your new test cases (it will actually generate those files for _all_ of the test cases, overwriting previous versions, so be careful!).
If you do not have access to a reference executable, you must create **all** of those files (`out`, `err`, and `status`) manually, using the pset specification as a guide.

<hr/>
&copy;2023 Alan Weide
