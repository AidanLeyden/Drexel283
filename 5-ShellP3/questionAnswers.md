1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_I have a loop that calls waitpid() for each forked process, not doing this could cause several issues such as outputs getting mixed up and child processes continuing to use system resources_

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_Open pipe ends prevent cause several problems such as preventing programs from receiving EOF and leaving data buffered indefinitely_

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_challenges that we would have to face if cd were implemented as an external porcess include the fact that child processes can't modify their parent's environment and the fact that the shell needs to track the current directory for future commands_

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_I could edit commandlist to have a dynamic array instead of an array of fixed size, this would have the tradeoff of making memory management much harder and a great risk of errors and resource issues popping up when the command chain gets very long_