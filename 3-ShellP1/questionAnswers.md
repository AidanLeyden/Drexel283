1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is generally a good pick as it is a reliably safe way to read from
    >stdin. fgets naturally protects from buffer overflows, unlike similar functions like gets()

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  using malloc is necessary for several reasons, stack allocated arrays have
    >limited size that could lead to stack overflows in the case of particurly large args. the
    >buffer must persist throughout the entire program's execution, malloc allows for this while
    >also making it safer to handle large buffers


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  on the command line spaces are just another character and could interfere with
    >command recognition. for example "cd" is different from " cd". trimming extra spaces helps
    >with command recognition and providing proper arguments for commands. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: we will need input, output, and error redirection. Some challenges we might face
    > include dealing with file permissions and error handling

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  both deal with directing input/output, though redirection deals with files
    > and piping deals with directing the output of one program to the input of another

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  separating these is important as without separation redirection and piping 
    > could interfere or be interfered with by outputs and errors

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: Our shell should maintain proper separation of error and normal output streams 
    > while providing mechanisms to merge them when needed. The merging functionality should be
    > implemented through standard file descriptor operations, allowing users to combine error and 
    > output streams when desired through command-line options.