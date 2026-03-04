import subprocess
from tests import test_2 as test

RESULTS_DIR = "./"
SUBMISSION_DIR = "./"


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


status, result = run_script(test["script"], test["input"])
if result:
    print(result.stdout)
