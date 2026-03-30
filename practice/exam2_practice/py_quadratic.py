import cmath

def solve_quadratic(a, b, c):
    d = (b**2) - (4*a*c)
    x1 = (-b - cmath.sqrt(d)) / (2*a)
    x2 = (-b + cmath.sqrt(d)) / (2*a)
    return x1, x2
root1, root2 = solve_quadratic(1, -13, 42)

# IMPORTANT: Print the results so the C program can read them through the pipe.
# We take the .real part and cast to int to match your C code's %d format.
print(f"{int(root1.real)} {int(root2.real)}")