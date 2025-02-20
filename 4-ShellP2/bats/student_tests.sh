#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Handle non existent command" {
    run "./dsh" <<EOF
fakecommand
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd to non existent directory" {
    run "./dsh" <<EOF
cd /nonexistent
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd with spaces in path" {
    current=$(pwd)
    cd /tmp
    mkdir -p "with spaces"
    run "${current}/dsh" <<EOF
cd /tmp/with spaces
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmpdsh2>dsh2>dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "exit with args" {
    run "./dsh" <<EOF
exit these are args
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "command with invalid argument" {
    run "./dsh" <<EOF
ls --notarealarg
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="ls:unrecognizedoption'--notarealarg'Try'ls--help'formoreinformation.dsh2>dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "quotes that are not closed" {
    run "./dsh" <<EOF
echo "this is a test  
EOF
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    expected_output="this is a test  dsh2> dsh2> cmd loop returned 0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "crtl-d" {
    run "./dsh" <<^D
^D
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "ctrl-c" {
    run "./dsh" <<^C
^C
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


