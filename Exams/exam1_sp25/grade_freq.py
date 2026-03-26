import subprocess
import os
import sys

# Detect if you are on Windows to handle the .exe extension
EXE_NAME = "freq.exe" if os.name == "nt" else "./freq"

def compile_freq():
    if not os.path.exists("freq.c"):
        print("Error: freq.c not found.")
        sys.exit(1)

    result = subprocess.run(
        ["gcc", "freq.c", "-o", "freq"],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Compilation FAILED:")
        print(result.stderr)
        sys.exit(1)

    print("Compilation successful.\n")

def run_tests():
    compile_freq()

    if not os.path.exists(EXE_NAME.replace("./", "")):
        print(f"Error: {EXE_NAME} executable not found after compilation.")
        sys.exit(1)

    # Test cases represented as tuples of strings
    test_cases = [
        ("10", "15", "200", "2", "34", "3"),
        ("99", "98", "9", "9234"),
        ("1", "22", "333", "4444", "55555"),
        ("0", "10", "100", "1000", "10000"),
        ("8", "88", "888", "7", "77", "777"),
        ("123", "456", "789", "101112"),
        ("5",)
    ]

    passed_all = True
    passed_count = 0

    print("Running Tests...\n" + "-"*30)

    for i, args in enumerate(test_cases, 1):
        # Calculate the expected result in Python
        expected_counts = {str(digit): 0 for digit in range(10)}
        for num_str in args:
            first_char = num_str[0]
            expected_counts[first_char] += 1

        expected_output_lines = [f"{digit}\t{expected_counts[str(digit)]}" for digit in range(10)]
        args_str = " ".join(args)

        try:
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

            if len(output_lines) == 10:
                match = True
                for j in range(10):
                    if output_lines[j] != expected_output_lines[j]:
                        match = False
                        break

                if match:
                    print(f"Test {i:02d} PASS: {args_str}")
                    passed_count += 1
                else:
                    print(f"Test {i:02d} FAIL: {args_str}")
                    print(f"  Expected:\n" + "\n".join(expected_output_lines))
                    print(f"  Got:\n" + "\n".join(output_lines))
                    passed_all = False
            else:
                print(f"Test {i:02d} FAIL: {args_str}")
                print(f"  Expected 10 lines of output, got {len(output_lines)}")
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
