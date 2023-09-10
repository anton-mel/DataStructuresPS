#! /usr/bin/env python

from unittest import defaultTestLoader, TestSuite, TextTestRunner
from argparse import ArgumentParser
from pathlib import Path

from create_tests import TestCaseGenerator


def load_tests_from_dir(dir: Path, uut: Path, suite: TestSuite):
    """Load tests from a directory containing test cases. It must be organized as follows:

        dir/
            visible/ (optional)
                v001/ (optional)
                    in (optional)
                    args (optional)
                    out
                    err
                    status
                v002/ (optional)
                    (same as t001)
                ...
            hidden/ (optional)
                h001/ (optional)
                    in (optional)
                    args (optional)
                    out
                    err
                    status
                h002/ (optional)
                    (same as p001)
                ...
            ...

    The name of each subdirectory of dir must match one of the visibility levels supported by Gradescope ("visible", "hidden" are the most useful). The directory names in those visibility level directories are unimportant, but are used to label the generated test case methods.
    """

    subdirs = filter(Path.is_dir, dir.glob("*"))
    for vis in subdirs:
        test_case = TestCaseGenerator(vis, uut, visibility=vis.name).testCase
        suite.addTests(defaultTestLoader.loadTestsFromTestCase(test_case))


def main():
    """Executes a series of input/output test cases from tests_dir, exercising the executable uut.
    """

    parser = ArgumentParser()
    parser.add_argument("uut", help="Executable under test.", type=Path)
    parser.add_argument("tests_dir",
                        help="Directory from which to load tests.", type=Path, action="append")
    args = parser.parse_args()

    suite = TestSuite()
    for dir in args.tests_dir:
        load_tests_from_dir(dir, args.uut, suite)

    TextTestRunner().run(suite)


if __name__ == "__main__":
    main()
