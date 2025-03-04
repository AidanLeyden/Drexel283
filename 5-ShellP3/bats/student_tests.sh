#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Basic pipe: ls | grep .c" {
    run "./dsh" <<EOF
ls | grep .c
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh_cli.cdshlib.cdsh3>dsh3>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Multiple pipes: ls | grep .c | wc -l" {
    run "./dsh" <<EOF
ls | grep .c | wc -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="2dsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Pipe with non-existent first command" {
    run "./dsh" <<EOF
nonexistent | grep test
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="error:nonexistent:commandnotfounddsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Pipe with non-existent second command" {
    run "./dsh" <<EOF
ls | nonexistent
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="error:nonexistent:commandnotfounddsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Maximum number of pipes (CMD_MAX-1)" {
    run "./dsh" <<EOF
echo uh | echo oh | echo uh | echo oh | echo uh | echo oh | echo uh | echo oh | echo uh | echo oh | echo uh | echo oh
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Incorrect pipe formating (start)" {
    run "./dsh" <<EOF
| echo hello
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="error:incorrectpipeformattingdsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Incorrect pipe formating (end)" {
    run "./dsh" <<EOF
echo hello |
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="error:incorrectpipeformattingdsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Pipe with quoted arguments" {
    run "./dsh" <<EOF
echo "hello | world" | grep world
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hello|worlddsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Several piped commands" {
    run "./dsh" <<EOF
ls | grep .c
ls | grep .c
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh_cli.cdshlib.cdsh_cli.cdshlib.cdsh3>dsh3>dsh3>cmdloopreturned0"
    echo "Output: $output"
    echo "Stripped Output: ${stripped_output}"
    echo "Expected Output: ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}