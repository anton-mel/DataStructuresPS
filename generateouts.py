#! /usr/bin/env python
"""Generate output files for a series of input files, based on some reference executable.

The input files must have the following structure:

[test_directory]/
    visible/ (optional)
        v001/ (optional)
            args (optional)
            in (optional)
        v002/ (optional)
            (same as v001)
        ...
    hidden/ (optional)
        h001/ (optional)
            args (optional)
            in (optional)
        h002/ (optional)
            (same as h001)
        ...
    ...
"""

from io import TextIOWrapper
from pathlib import Path
import subprocess
from argparse import ArgumentParser, REMAINDER

from progressbar import progressbar

IN_FILE = "in"
ARGS_FILE = "args"
DOC_FILE = "doc"
OUT_FILE = "out"
ERR_FILE = "err"
CODE_FILE = "status"


def _only_dirs(dir: Path) -> list[Path]:
    """Returns a list of all and only directories at Path `dir`.
    """

    return sorted(filter(Path.is_dir, dir.glob("*")))


def _generate_out(dir: Path, exep: Path, dry_run=False):
    """`dir` is the directory of a single test case, which must contain a file called "in" or
    "args" (or both).

    The function generates files called "out", "err", and "status".
    """

    infile, args = None, []
    if dir.joinpath(IN_FILE).exists():
        infile = dir.joinpath(IN_FILE).open("r")
    if dir.joinpath(ARGS_FILE).exists():
        with dir.joinpath(ARGS_FILE).open("r") as argfile:
            args = [line.strip() for line in argfile]

    outfile, errfile, codefile = subprocess.PIPE, subprocess.PIPE, None
    if not dry_run:
        outfile = dir.joinpath(OUT_FILE).open("w")
        errfile = dir.joinpath(ERR_FILE).open("w")
        codefile = dir.joinpath(CODE_FILE).open("w")

    # Run exep with args and input from infile, printing things to outfile and errfile
    proc = subprocess.Popen(
        [str(exep)] + args, stdin=infile, stdout=outfile, stderr=errfile)

    exitcode = proc.wait()

    if dry_run:
        assert proc.stdout is not None
        out = str(proc.stdout.read(), encoding="utf-8")
        print(f"{dir.joinpath(OUT_FILE)}:\n{'-'*64}\n{out}\n{'-'*64}\n")

        assert proc.stderr is not None
        err = str(proc.stderr.read(), encoding="utf-8")
        print(f"{dir.joinpath(ERR_FILE)}:\n{'-'*64}\n{err}\n{'-'*64}\n")

        exitcode = proc.wait()
        print(f"{dir.joinpath(CODE_FILE)}:\n{'-'*64}\n{exitcode}\n{'-'*64}\n")
    else:
        assert isinstance(codefile, TextIOWrapper)
        codefile.write(f"{exitcode}\n")

        assert isinstance(outfile, TextIOWrapper)
        assert isinstance(errfile, TextIOWrapper)

        outfile.close()
        errfile.close()
        codefile.close()

    if infile:
        infile.close()


def _run_cases_in_dir(args, d):
    """Generate output for all tests cases with folders in `d`.
    """

    case_dirs = _only_dirs(d)
    if len(case_dirs) > 0:
        print(f"Running tests in {d}...")
        case_dirs = case_dirs if args.dry_run else progressbar(
            case_dirs)
        for d in case_dirs:
            _generate_out(d, args.ref_exe, args.dry_run)
    else:
        print(f"No tests found in {d}.")


def main():
    """Generate output files based on the input files in a directory, using `ref_exe` as the
    canonical version.
    """

    parser = ArgumentParser()
    parser.description = __doc__
    parser.add_argument("-d", "--dry-run", action="store_true")
    parser.add_argument(
        "ref_exe", help="Path to the reference executable.", type=Path)
    parser.add_argument(
        "dirs", help="(...) Directory(s) in which to find test cases.", type=Path, nargs=REMAINDER)
    args = parser.parse_args()

    if args.dry_run:
        print(f"{'':*^64}\n*{'DRY RUN':^62}*\n{'':*^64}")

    for dir in args.dirs:
        vis_dirs = _only_dirs(dir)
        if len(vis_dirs) > 0:
            print(f"Searching for tests in {dir}...")
            for d in vis_dirs:
                _run_cases_in_dir(args, d)
        else:
            print(f"No tests found in {dir}.")


if __name__ == "__main__":
    main()
