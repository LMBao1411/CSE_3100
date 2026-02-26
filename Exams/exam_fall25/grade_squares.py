import subprocess
import os
import sys

# Detect if you are on Windows to handle the .exe extension
EXE_NAME = "squares.exe" if os.name == "nt" else "./squares"

def compile_squares():
    if not os.path.exists("squares.c"):
        print("Error: squares.c not found.")
        sys.exit(1)

    # Compiling just squares.c as there are no helper files for this assignment
    result = subprocess.run(
        ["gcc", "squares.c", "-o", "squares"],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Compilation FAILED:")
        print(result.stderr)
        sys.exit(1)

    print("Compilation successful.\n")


def run_tests():
    compile_squares()

    if not os.path.exists(EXE_NAME.replace("./", "")):
        print(f"Error: {EXE_NAME} executable not found after compilation.")
        sys.exit(1)

    # Test cases represented as lists of strings mapping to command line arguments
    test_cases = [
        ("-4", "-1", "0", "3", "10"),
        ("-7", "-3", "-1"),
        ("1", "2", "3", "4", "5"),
        ("0", "0", "0", "0"),
        ("-100", "-50", "0", "50", "100"),
        ("-5", "-4", "-3", "-2", "-1", "0", "1", "2", "3", "4", "5"),
        ("-99", "1", "2", "3"),
        ("-10", "10"),
        ("-20", "-15", "-10", "5", "6", "7", "8"),
        ("42",)
    ]

    passed_all = True
    passed_count = 0

    print("Running Tests...\n" + "-"*30)

    for i, args in enumerate(test_cases, 1):
        # Calculate the expected result in Python
        nums = [int(x) for x in args]
        expected_squares = sorted([x**2 for x in nums])
        expected_output = " ".join(map(str, expected_squares))
        args_str = " ".join(args)

        try:
            # Pass unpacked args directly to the subprocess
            result = subprocess.run(
                [EXE_NAME, *args],
                capture_output=True,
                text=True,
                timeout=2
            )

            # Clean up stdout to compare lines reliably
            output_lines = [
                line.strip()
                for line in result.stdout.strip().split('\n')
                if line.strip()
            ]

            if len(output_lines) >= 1:
                actual_output = output_lines[0]

                if actual_output == expected_output:
                    print(f"Test {i:02d} PASS: {args_str}")
                    passed_count += 1
                else:
                    print(f"Test {i:02d} FAIL: {args_str}")
                    print(f"  Expected: {expected_output}")
                    print(f"  Got:      {actual_output}")
                    passed_all = False
            else:
                print(f"Test {i:02d} FAIL: {args_str}")
                print(f"  Expected 1 line of output, got {len(output_lines)}")
                print(f"  Raw output: {result.stdout}")
                passed_all = False

        except subprocess.TimeoutExpired:
            print(f"Test {i:02d} FAIL: {args_str} (Timed out, infinite loop?)")
            passed_all = False

    print("-" * 30)
    print(f"Results: {passed_count}/{len(test_cases)} passed.\n")

    if passed_all:
        print("yeah!")
    else:
        print("NO!!!!!!!!!!!!!!!!!!!")

if __name__ == "__main__":
    run_tests()
