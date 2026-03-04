# Two different types of tests
# script - to run bash scripts or compile files and running anyother code that doesn't need to check outputs, will give results based on exit code.
# IO - to run IO tests.

# IO- io tests take an input and output parameters. you can pass all the inputs with input and required output with output var.
# script - can also take input parameter if required.

# test_1 is a script test that compiles calculator.c file using a makefile form files/ folder
test_1 = {
    "type": "script",
    "meta": {
        "name": "Compilation calculator.c",
        "max_score": None,
        "visibility": "visible",
    },
    "script": "make calculator",
    "context": "Required Files: calculator.c\n",
    "successText": "Code compilation successful\n",
    "failureText": "Code failed to compile\n",
}

# test_2 does an IO test on calculator by running ./calculator addition
# addition is the parameter telling the code to perform addition
# we give input 1 and 2 with input parameter and required output with output paramter
# as 1+2=3, output is 3 .

test_2 = {
    "type": "IO",
    "meta": {
        "name": "IO Test-addition calculator.c",
        "max_score": 20,
        "visibility": "visible",
    },
    "script": "./calculator add",
    "successText": "IO Test passed\n",
    "failureText": "IO Test failed\n",
    "displayDiff": True,
    "ioCheck": "flex",
    "input": "1\n2",
    "expected": """Enter num1:
Enter num2:
1 + 2 = 3
""",
}
test_3 = {
    "type": "IO",
    "meta": {
        "name": "IO Test-addition calculator.c",
        "max_score": 20,
        "visibility": "visible",
    },
    "script": "./calculator mul",
    "successText": "IO Test passed\n",
    "failureText": "IO Test failed\n",
    "ioCheck": "strict",
    "displayDiff": True,
    "input": "1\n2",
    "expected": """Enter num1:
Enter num2:
1 * 2 = 2
""",
}


# pass all the tests to tests list , you these tests run in the order of the list
tests = [test_1, test_2, test_3]
