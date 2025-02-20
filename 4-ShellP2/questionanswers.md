1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Fork is required because execvp replaces the current process with the new
    > program. By forking first, we create a child process to run the new program while the parent > process continues running. without a fork the process would end after a single command.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  when fork() fails it returns -1. I dont think there was a specific error in
    > dshlib.h to print/return in the case of this happening so I simply continue;

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches for commands in the directories listed in the PATH 
    > environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  wait() allos us to know the exit status of the child process. without it the parent process would continue without us knowing of any possible issues that occured within the child process.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() gets the exit code that wait() returned. this tells us if the
    > child status finished with or without any issues. we can use this information in the parent 
    > class to adjust based on any issues, for exaple i could throw an error and terminate the
    > parent class if a child class failed

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  my implementation of build_cmd_buff triggers a bool when a quote is seen.
    > while true spaces are no longer ignored while parsing through the command. a second
    > quote will negate the bool and all of the text will be taken in as a single arg (minus
    > the actual quotes). this is important as several functions like 'echo' 
    > require being able to take in a quoted sentance as a single arg

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I monstly redid my parsing logic compared to the previous assignment. instead 
    > of splitting things into tokens I ended up going character by character. It was easier to 
    > handle quoted arguments the new way. most of my problems stemmed from just trying to raw 
    > port all of my previous work over to this assignment. it became much easier when I just 
    > started over and used my previous experience as a mental starting point instead of a 
    > physical starting point

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  signals are used to notify a process of something that is currently happening. 
    > unlike other IPCs signals are asychronus.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGTERM is the standard type of termination signal, it terminates a program 
    >while the program to interact with it. SIGKILL on the other hand is the immediate termination 
    > of the process, the program cannot interact with this in any way. SIGINT is similar to 
    > SIGTERM in that it causes a kinder shutdown of the program, it can be called with crtl+c.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP is similar to SIGKILL in that it immediately terminates the program. 
    > this cannot be caught or ignored by the program and will only be undone by SIGCONT