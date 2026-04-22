/* executes the files using execvp
get the parameters needed for execvp to run

fork() a child process
Set up pipes for stdin/stdout capture
Use execvp() to run the command
Need to handle infinite loops + seg faults + memory overflow + etc.

*/