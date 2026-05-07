import sys

# missing base case for n==1, causes infinite recursion on odd numbers
def factorial(n):
    if n == 0:
        return 1
    return n * factorial(n - 1)   # works fine actually, but...

# wrong base case — fib(1) returns 0 instead of 1
def fibonacci(n):
    if n == 0:
        return 0
    if n == 1:
        return 0    # <-- should be 1
    return fibonacci(n - 1) + fibonacci(n - 2)

# no base case at all — infinite recursion and crash
def sum_digits(n):
    return (n % 10) + sum_digits(n // 10)

# decrements base instead of exponent
def power(base, exp):
    if exp == 0:
        return 1
    return base * power(base - 1, exp)   # should be (base, exp-1)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <command> <args>")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "factorial":
        print(factorial(int(sys.argv[2])))
    elif cmd == "fibonacci":
        print(fibonacci(int(sys.argv[2])))
    elif cmd == "sumdigits":
        print(sum_digits(int(sys.argv[2])))
    elif cmd == "power":
        if len(sys.argv) < 4:
            print("power requires base and exponent")
            sys.exit(1)
        print(power(int(sys.argv[2]), int(sys.argv[3])))
    else:
        print(f"Unknown command: {cmd}")
        sys.exit(1)