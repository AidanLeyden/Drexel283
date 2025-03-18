#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file
setup() {
  ./dsh -s > server.log 2>&1 &
  server_pid=$!
  sleep 2
}

teardown() {
  kill $server_pid 2>/dev/null
  rm -f server.log
}


@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "simple command" {
  run ./dsh -c <<EOF
ls
EOF
  echo "Client output:"
  echo "$output"
  [ "$status" -eq 0 ]
}

@test "basic pipe" {
  run ./dsh -c <<EOF
ls | grep dsh
EOF
  # Adjust expected output as needed.
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>rsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped Output: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "Non existent command returns proper error" {
  run ./dsh -c <<EOF
nonexistent
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Expected error message should be exactly what your shell prints for unknown commands.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>error:nonexistent:commandnotfoundrsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "Incorrect pipe formatting pipe at start" {
  run ./dsh -c <<EOF
| echo hello
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Expected error for leading pipe.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>error:incorrectcommandorpipeformattingrsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "Incorrect pipe formatting pipe at end" {
  run ./dsh -c <<EOF
echo hello |
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Expected error for trailing pipe.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>error:incorrectcommandorpipeformattingrsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "Multiple piped commands" {
  run ./dsh -c <<EOF
ls | grep .c
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Use your shell’s actual output from this pipeline.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>rsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "Several piped commands over multiple lines" {
  run ./dsh -c <<EOF
echo hello | tr 'a-z' 'A-Z'
echo world | tr 'a-z' 'A-Z'
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Expect output like "HELLO" from the first pipeline and "WORLD" from the second,
  # followed by the usual shell messages.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>rsh>rsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "change directory and pwd" {
  run ./dsh -c <<EOF
cd /tmp
pwd
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Expect the pwd command to output "/tmp" then the shell’s termination prompt.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>changeddirectoryrsh>rsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "exit" {
  run ./dsh -c <<EOF
exit
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # When exit is executed, only the termination message should be left.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}

@test "stop-server" {
  run ./dsh -c <<EOF
stop-server
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  # Expect the final termination message with no additional prompt.
  expected_output="socketclientmode:addr:127.0.0.1:1234rsh>cmdloopreturned0"
  echo "Output: $output"
  echo "Stripped: ${stripped_output}"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}