import sys

if len(sys.argv) != 3:
    print("Usage: python3 py_calc.py <num1> <num2>")
    sys.exit(1)

a = int(sys.argv[1])
b = int(sys.argv[2])

print(a + b)