import sys

def reverse_string(s):
    return s[::-1]

def count_vowels(s):
    return sum(1 for c in s.lower() if c in 'aeiou')

def is_palindrome(s):
    cleaned = ''.join(c.lower() for c in s if c.isalnum())
    return cleaned == cleaned[::-1]

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <command> <string>")
        sys.exit(1)

    cmd = sys.argv[1]
    text = sys.argv[2]

    if cmd == "reverse":
        print(reverse_string(text))
    elif cmd == "vowels":
        print(count_vowels(text))
    elif cmd == "palindrome":
        print(is_palindrome(text))
    else:
        print(f"Unknown command: {cmd}")
        sys.exit(1)