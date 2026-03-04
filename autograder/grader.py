import os
import subprocess
import json
import pprint
from tests import tests

# all the student submissions are stored in the SUBMISSION_DIR
# and the results.json file will be saved in RESULTS_DIR
SUBMISSION_DIR = "/autograder/submission"
RESULTS_DIR = "/autograder/results"

# uncomment these below lines and modify the path to run this code locally
# and then simply run python3 ./grader.py

RESULTS_DIR = "/mnt/c/Users/LENOVO/OneDrive - University of Connecticut/BAO_FILES/Classes/SPRING_2026/CSE_3100/autograder"
SUBMISSION_DIR = "/mnt/c/Users/LENOVO/OneDrive - University of Connecticut/BAO_FILES/Classes/SPRING_2026/CSE_3100/autograder"

RESULTS_FILE = os.path.join(RESULTS_DIR, "results.json")


def dumpResult(result):
    with open(RESULTS_FILE, "w", encoding="utf-8") as file:
        json.dump(result, file, indent=4)


def run_script(script, input_data=None, timeout=None):
    if timeout is None:
        timeout = 5
    try:
        result = subprocess.run(
            script,
            input=input_data,
            cwd=SUBMISSION_DIR,
            capture_output=True,
            text=True,
            shell=True,
            timeout=timeout,
        )
        return "success", result
    except subprocess.TimeoutExpired:
        return "failed", "Script execution timed out."
    except subprocess.CalledProcessError as e:
        return (
            "failed",
            f"Script execution failed with error code {e.returncode}: {e.stderr}",
        )


# compares output from tests and stdout strictly including spaces and letter case
def checkIOStrict(test, stdout):
    expected_strip = test["expected"].strip()
    stdout_strip = stdout.strip()
    output = ""
    if expected_strip == stdout_strip:
        if test["displayDiff"]:
            output = f"""Output matches the expected output.\nCode output:\n{stdout}"""
        return output, True
    else:
        if test["displayDiff"]:
            output = f"""Output does not match the expected output.\nExpected output:\n{test['expected']}\n"""
        if test["displayDiff"]:
            output += f"""Code output:\n{stdout}\n"""
        return output, False


# compares output from tests and stdout without worrying about spaces and letter case
def checkIOFlexible(test, stdout):
    exp_lines = test["expected"].strip().split("\n")
    out_lines = stdout.strip().split("\n")
    feedback = ""
    exp = [
        line.strip().lower().replace(" ", "") for line in test["expected"].split("\n")
    ]
    out = [line.strip().lower().replace(" ", "") for line in stdout.split("\n")]
    i = 0
    j = 0
    if len(out) > len(exp):
        if test["displayDiff"]:
            feedback = f"""Output contains additional text .\nExpected output:\n{test['expected']}\nCode output:\n{stdout}\n"""
        return feedback, False
    if len(out) < len(exp):
        if test["displayDiff"]:
            feedback = f"""Output incomplete .\nExpected output:\n{test['expected']}\nCode output:\n{stdout}\n"""
        return feedback, False

    while i < len(out) and j < len(exp):
        if out[i] == exp[i]:
            i += 1
            j += 1
        else:
            if test["displayDiff"]:
                feedback = f"""Output does not match the expected output.\nExpected output:\n{test['expected']}\nCode output:\n{stdout}\n"""
            if test["displayDiff"]:
                feedback += f"""Difference found at line {i+1}:\nExpected: {exp_lines[i]}\n   Found: {out_lines[i]}"""
            return feedback, False
    if test["displayDiff"]:
        feedback = f"""Output matches the expected output.\nCode output:\n{stdout}"""
    return feedback, True


# processes tests that requires IO testing
def processIOTestOutput(test, status, output):
    result = test["meta"]
    result["output"] = ""
    if status == "success":
        if "context" in test:
            result["output"] = test["context"]
        if output.returncode == 0:
            if test["ioCheck"] == "strict":
                feedback, match = checkIOStrict(test, output.stdout)
            else:
                feedback, match = checkIOFlexible(test, output.stdout)

            if match:
                result["output"] += test["successText"] + feedback
                result["status"] = "passed"
                result["score"] = result["max_score"]
            else:
                result["output"] += test["failureText"] + feedback
                result["status"] = "failed"
                result["score"] = 0
        else:
            result["output"] = test["failureText"] + output.stdout + output.stderr
            result["status"] = "failed"
            result["score"] = 0
    else:
        result["output"] = (
            "Code failed to run properly. (Might contain an infinite loop)"
        )
        result["status"] = "failed"
        result["score"] = 0
    return result


# scripts like unit test, and bash scripts or even compilation are done by this function
# with script test you cannot compare input and output, it will simply output stdout and stderr .
# and will throw errors based on exit code


def processScriptTestOutput(test, status, output):
    result = test["meta"]
    if status == "success":
        result["output"] = ""
        if "context" in test:
            result["output"] = test["context"]
        if output.returncode == 0:
            result["output"] += test["successText"] + output.stdout
            result["status"] = "passed"
            result["score"] = result["max_score"]
        else:
            result["output"] += test["failureText"] + output.stdout + output.stderr
            result["status"] = "failed"
            result["score"] = 0
        return result
    else:
        result["output"] = (
            "Code failed to run properly. (Might contain an infinite loop)"
        )
        result["status"] = "failed"
        result["score"] = 0
        return result


result = {"tests": []}

for test in tests:
    if test["type"] == "script" and "input" not in test:
        script_status, script_output = run_script(test["script"])
        result["tests"].append(
            processScriptTestOutput(test, script_status, script_output)
        )
    if test["type"] == "script" and "input" in test:
        script_status, script_output = run_script(test["script"], test["input"])
        result["tests"].append(
            processScriptTestOutput(test, script_status, script_output)
        )
    if test["type"] == "IO":
        script_status, script_output = run_script(test["script"], test["input"])
        result["tests"].append(processIOTestOutput(test, script_status, script_output))


dumpResult(result)
# pprint.pprint(result)
