/*
take the output from the input file and compare with the expected output
then generate the JSON file which the format could be referenced from the pyhthon implementation

{
    "tests": [
        {
            "name": "Compilation calculator.c",
            "max_score": null,
            "visibility": "visible",
            "output": "Required Files: calculator.c\nCode compilation successful\ngcc -Wall    calculator.c   -o calculator\n",
            "status": "passed",
            "score": null
        },
        {
            "name": "IO Test-addition calculator.c",
            "max_score": 20,
            "visibility": "visible",
            "output": "IO Test passed\nOutput matches the expected output.\nCode output:\nEnter num1:\nEnter num2:\n1 + 2 = 3\n",
            "status": "passed",
            "score": 20
        },
        {
            "name": "IO Test-addition calculator.c",
            "max_score": 20,
            "visibility": "visible",
            "output": "IO Test failed\nOutput does not match the expected output.\nExpected output:\nEnter num1:\nEnter num2:\n1 * 2 = 2\n\nCode output:\nEnter num1:\nEnter num2:\n1 * 2 = -1\n\nDifference found at line 3:\nExpected: 1 * 2 = 2\n   Found: 1 * 2 = -1",
            "status": "failed",
            "score": 0
        }
    ]
}

*/