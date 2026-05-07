import sys

def factorial(n):
    if n < 0:
        return -1
    if n == 0 or n == 1:
        return 1
    return n * factorial(n - 1)

def fibonacci(n):
    if n < 0:
        return -1
    if n == 0:
        return 0
    if n == 1:
        return 1
    return fibonacci(n - 1) + fibonacci(n - 2)

def sum_digits(n):
    n = abs(n)
    if n < 10:
        return n
    return (n % 10) + sum_digits(n // 10)

def power(base, exp):
    if exp == 0:
        return 1
    if exp < 0:
        return -1
    return base * power(base, exp - 1)

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