import subprocess
import os
import sys

EXE_NAME = "linked_list.exe" if os.name == "nt" else "./linked_list"

def compile_linked_list():
    if not os.path.exists("linked_list.c"):
        print("Error: linked_list.c not found.")
        sys.exit(1)

    print("Compiling Linked List...")
    result = subprocess.run(
        ["gcc", "-Wall", "-g", "-std=c99", "-o", "linked_list", "linked_list.c"],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Compilation FAILED:\n", result.stderr)
        sys.exit(1)
    print("Compilation successful.\n")

def check_null_ptr(val):
    val = val.strip().lower()
    if val in ["(nil)", "(null)"]:
        return True
    try:
        return int(val, 16) == 0
    except ValueError:
        return False

def run_tests():
    compile_linked_list()

    if not os.path.exists(EXE_NAME.replace("./", "")):
        print(f"Error: Executable not found.")
        sys.exit(1)

    # 15 Test Cases
    test_cases = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 50, 100]
    passed_count = 0

    print("Running 15 Linked List Tests...\n" + "="*40)

    for i, n in enumerate(test_cases, 1):
        # Calculate exactly what the C program should output
        list1 = [x for x in range(n)]
        list2 = [2 * x for x in range(n)]
        merged = sorted(list1 + list2)

        expected_l1 = " ".join(map(str, list1))
        expected_l2 = " ".join(map(str, list2))
        expected_merged = " ".join(map(str, merged))

        try:
            result = subprocess.run([EXE_NAME, str(n)], capture_output=True, text=True, timeout=2)
            output_lines = [line.strip() for line in result.stdout.strip().split('\n') if line.strip()]

            if len(output_lines) >= 5:
                actual_l1 = output_lines[0]
                actual_l2 = output_lines[1]
                actual_merged = output_lines[2]
                ptr1 = output_lines[3]
                ptr2 = output_lines[4]

                if (actual_l1 == expected_l1 and actual_l2 == expected_l2 and
                    actual_merged == expected_merged and check_null_ptr(ptr1) and check_null_ptr(ptr2)):
                    print(f"Test {i:02d} [PASS] (n = {n})")
                    passed_count += 1
                else:
                    print(f"\nTest {i:02d} [FAIL] (n = {n})")
                    print("-" * 30)
                    if actual_l1 != expected_l1:
                        print(f"  [EXPECTED L1]: {expected_l1}")
                        print(f"  [ACTUAL L1]:   {actual_l1}\n")
                    if actual_l2 != expected_l2:
                        print(f"  [EXPECTED L2]: {expected_l2}")
                        print(f"  [ACTUAL L2]:   {actual_l2}\n")
                    if actual_merged != expected_merged:
                        print(f"  [EXPECTED MERGED]: {expected_merged}")
                        print(f"  [ACTUAL MERGED]:   {actual_merged}\n")
                    if not check_null_ptr(ptr1) or not check_null_ptr(ptr2):
                        print(f"  [POINTER ERROR]: Ensure *p1 and *p2 are set to NULL.")
                        print(f"  [ACTUAL PTRS]:   {ptr1}, {ptr2}\n")
                    print("=" * 40)
            else:
                print(f"\nTest {i:02d} [FAIL] (n = {n}) - Missing output lines.")
                print(f"Raw output:\n{result.stdout}\n" + "=" * 40)

        except subprocess.TimeoutExpired:
            print(f"Test {i:02d} [FAIL] (n = {n}) - TIMED OUT (Infinite loop?)\n" + "=" * 40)

    print(f"\nResults: {passed_count}/{len(test_cases)} passed.")
    if passed_count == len(test_cases): print("yeah! All tests passed!")
    else: print("NO!!!!!!!!!!!!!!!!!!!")

if __name__ == "__main__":
    run_tests()
