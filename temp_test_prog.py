#!/usr/bin/env python3

import re
import sys
from pathlib import Path


# Matches hexadecimal byte values such as:
#   0x00
#   0xAF
#   0xff
OPCODE_RE = re.compile(r"\b0x([0-9a-fA-F]{1,2})\b")


def parse_tested_instructions(source_path: Path) -> set[int]:
    """
    Parse the '// INSTRUCTIONS TESTED:' comment block from a C source file
    and return a set containing all tested opcodes.
    """
    source = source_path.read_text(encoding="utf-8")

    tested = set()
    in_instruction_block = False

    for line in source.splitlines():
        stripped = line.strip()

        # Start of the instruction coverage block.
        if stripped == "// INSTRUCTIONS TESTED:":
            in_instruction_block = True
            continue

        if not in_instruction_block:
            continue

        # Only continue through // comment lines.
        #
        # A blank line or non-comment line terminates the block.
        if not stripped.startswith("//"):
            break

        # Extract every 0xNN value from this comment line.
        for match in OPCODE_RE.finditer(stripped):
            opcode = int(match.group(1), 16)

            # A byte opcode must be 0x00-0xFF.
            if 0 <= opcode <= 0xFF:
                tested.add(opcode)

    return tested


def print_analysis(tested: set[int]) -> None:
    """Print a human-readable coverage report."""
    all_opcodes = set(range(0x100))
    untested = all_opcodes - tested

    print("=" * 70)
    print("CPU INSTRUCTION COVERAGE")
    print("=" * 70)

    print(f"Tested:   {len(tested):3d} / 256 ({len(tested) / 256:.1%})")
    print(f"Untested: {len(untested):3d} / 256 ({len(untested) / 256:.1%})")

    print()
    print("-" * 70)
    print("TESTED INSTRUCTIONS")
    print("-" * 70)

    for opcode in sorted(tested):
        print(f"0x{opcode:02X}", end=" ")

        # Keep output reasonably readable.
        if (sorted(tested).index(opcode) + 1) % 12 == 0:
            print()

    print()

    print("-" * 70)
    print("UNTESTED INSTRUCTIONS")
    print("-" * 70)

    for i, opcode in enumerate(sorted(untested), 1):
        print(f"0x{opcode:02X}", end=" ")

        if i % 12 == 0:
            print()

    print()

    print("-" * 70)
    print("UNTESTED INSTRUCTION RANGES")
    print("-" * 70)

    for start, end in make_ranges(sorted(untested)):
        if start == end:
            print(f"0x{start:02X}")
        else:
            print(f"0x{start:02X}-0x{end:02X}")

    print()
    print("=" * 70)


def make_ranges(values: list[int]) -> list[tuple[int, int]]:
    """
    Convert a sorted list of integers into contiguous ranges.

    Example:
        [1, 2, 3, 7, 8, 10]
    becomes:
        [(1, 3), (7, 8), (10, 10)]
    """
    if not values:
        return []

    ranges = []
    start = previous = values[0]

    for value in values[1:]:
        if value == previous + 1:
            previous = value
            continue

        ranges.append((start, previous))
        start = previous = value

    ranges.append((start, previous))

    return ranges


def main() -> int:
    # Default to the path from the question, but allow an optional
    # command-line argument.
    if len(sys.argv) > 1:
        source_path = Path(sys.argv[1])
    else:
        source_path = Path("tests/cpu/test_cpu.c")

    if not source_path.exists():
        print(f"Error: file not found: {source_path}", file=sys.stderr)
        return 1

    try:
        tested = parse_tested_instructions(source_path)
    except OSError as exc:
        print(f"Error reading {source_path}: {exc}", file=sys.stderr)
        return 1

    print_analysis(tested)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

