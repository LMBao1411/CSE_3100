import subprocess
import os
import sys

# Detect if you are on Windows to handle the .exe extension
EXE_NAME = "calculator.exe" if os.name == "nt" else "./calculator"

def compile_calculator():
    if not os.path.exists("calculator.c"):
        print("Error: calculator.c not found.")
        sys.exit(1)

    if not os.path.exists("linkedlist.c"):
        print("Error: linkedlist.c not found. I need this to compile the helper functions!")
        sys.exit(1)

    result = subprocess.run(
        ["gcc", "calculator.c", "linkedlist.c", "-o", "calculator"],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Compilation FAILED:")
        print(result.stderr)
        sys.exit(1)

    print("Compilation successful.\n")


def run_tests():
    compile_calculator()

    if not os.path.exists(EXE_NAME.replace("./", "")):
        print(f"Error: {EXE_NAME} executable not found after compilation.")
        sys.exit(1)

    test_cases = [
        ("456", "123"),
        ("1000", "5"),
        ("105", "102"),
        ("999", "1"),
        ("10000", "9999"),
        ("1", "1"),
        ("500", "500"),
        ("10", "9"),
        ("85", "16"),
        ("30005", "6"),
        ("111111", "111"),
        ("20", "19"),
        ("732894723", "732894723"),
        ("123456789", "98765432"),
        ("9999999999999999999", "1")
    ]

    passed_all = True
    passed_count = 0

    print("Running Tests...\n" + "-"*30)

    for i, (num1, num2) in enumerate(test_cases, 1):
        expected_sum = str(int(num1) + int(num2))
        expected_diff = str(int(num1) - int(num2))

        try:
            result = subprocess.run(
                [EXE_NAME, num1, num2],
                capture_output=True,
                text=True,
                timeout=2
            )

            output_lines = [
                line.strip()
                for line in result.stdout.strip().split('\n')
                if line.strip()
            ]

            if len(output_lines) >= 2:
                actual_sum = output_lines[0]
                actual_diff = output_lines[1]

                if actual_sum == expected_sum and actual_diff == expected_diff:
                    print(f"Test {i:02d} PASS: {num1} and {num2}")
                    passed_count += 1
                else:
                    print(f"Test {i:02d} FAIL: {num1} and {num2}")
                    print(f"  Expected: Sum={expected_sum}, Diff={expected_diff}")
                    print(f"  Got:      Sum={actual_sum}, Diff={actual_diff}")
                    passed_all = False
            else:
                print(f"Test {i:02d} FAIL: {num1} and {num2}")
                print(f"  Expected 2 lines of output, got {len(output_lines)}")
                print(f"  Raw output: {result.stdout}")
                passed_all = False

        except subprocess.TimeoutExpired:
            print(f"Test {i:02d} FAIL: {num1} and {num2} (Timed out, infinite loop?)")
            passed_all = False

    print("-" * 30)
    print(f"Results: {passed_count}/{len(test_cases)} passed.\n")

    if passed_all:
        print("yeah!")
    else:
        print("NO!!!!!!!!!!!!!!!!!!!")

if __name__ == "__main__":
    run_tests()
