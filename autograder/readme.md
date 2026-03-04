### Autograder for Gradescope

This code is an example for autograder that grades calculator.c.

Follows https://gradescope-autograders.readthedocs.io/en/latest/specs/

The idea is to have a simple python script that runs multiple tests, it needs to be plug and play so new features can be added when required.

Initial version had objects for different tests and different ways to run the code but it was very complex to add tests. So this version was created where we can add tests in no time and run them in any order we like.

To run this example autograder code locally, modify SUBMISSION_DIR and RESULTS_DIR to your desired local folder and

make sure to copy all the dependency files like make files and custom header files to SUBMISSION_DIR and run

`python3 ./grader.py`

Results are stored in results.json file

**To use this for gradescope to run grade your own assignments, follow below steps:**

1. Stores all files like Makefile, headers and other files your submission requires in files/ directory.
2. Add tests in tests.py (more guidelines are given in comments)
3. make sure SUBMISSION_DIR = "/autograder/submission" and RESULTS_DIR = "/autograder/results"
4. zip files/ tests.py grader.py run_autograder setup.sh
5. Upload the zip file to autograder (grader.zip should be uploaded to gradescope)

##### What do these files do?

1. tests.py - contains all tests
2. grader.py code that runs these tests
3. run_autograder- autograder runs this script to execute all the tests
4. setup.sh - you can install required packages like valgrind with this file
5. helper.py- a helper file to copy the output for a test. It is sometimes hard to infer what the stdout will be for a test so you can simply run `python3 ./helper.py >> out.txt` and copy out.txt and give it as output parameter in tests.py
6. results.json - grader.py will generate this file and gradescope will use this file to display points and feedback for each test

##### Some disadvantages using this

tests.py could get more complex if the code has a large stdout, in such cases I suggest redirecting stdout to a different file and using diff to compare outputs. `./cprogram >> stdout.txt && diff stdout.txt requried.txt`

This code doesn't inherently run multiple tests in parallel as we might require it do for server-client programs. In such case modify code like this.

```python
from tests import test_1_0, test_2_0, test_1_1, test_2_1
#test_1_0 compiles server
#test_2_0 compiles client
#test_1_1 runs server
#test_2_1 runs client

for test in test_1_0, test_2_0:
    if test["type"] == "script" and "input" not in test:
        script_status, script_output = run_script(test["script"])
        result["tests"].append(
            processScriptTestOutput(test, script_status, script_output)
        )

thread_server = threading.Thread(
    target=lambda: processScriptTestOutput(test_1_1, *run_script(test_1_1["script"]))
)
thread_server.start()

thread_client = threading.Thread(
    target=lambda: result["tests"].append(
        processIOTestOutput(test_2_1, *run_script(test_2_1["script"]))
    )
)
thread_client.start()

thread_client.join()
thread_server.join()
```

##### test structure

```python
#script test

test_name = {
    "type": "script",
    "meta": {
        "name": "Name of the test to be displayed on gradescope",
        "max_score": None,
        "visibility": "visible", #(visible/hidden)
    },
    "script": "script file /command to execute",
    "context": "Any information that needs to be displayed under a test", #(optional)
    "successText": "",#(optional)
    "failureText": "",#(optional)
    "input": "",#(optional)
}
#IO test

test_name = {
    "type": "IO",
    "meta": {
        "name": "Name of the test to be displayed on gradescope",
        "max_score": ,#(score for this test)
        "visibility": "visible",
    },
    "script": "script file /command to execute",
    "successText": "IO Test passed\n",
    "context": "Any information that needs to be displayed under a test", #(optional)
    "successText": "",#(optional)
    "failureText": "",#(optional)
    "displayDiff": True, #(True/False)
    "ioCheck": "flex", #(flex/strict)
    "input": "",
    "expected": "",
}


```

​
