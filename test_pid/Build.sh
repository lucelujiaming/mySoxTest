#!/bin/sh
# Clean
rm test_anti_windup_pid          
rm test_increment_pid            
rm test_integral_separate_pid    
rm test_pid                      
rm test_trapezoidal_integral_pid 
rm test_variable_integral_pid    
rm test_anti_windup_pid_output.txt
rm test_increment_pid_output.txt
rm test_integral_separate_pid_output.txt
rm test_pid_output.txt
rm test_trapezoidal_integral_pid_output.txt
rm test_variable_integral_pid_output.txt

# Compile
g++ -o test_anti_windup_pid             test_anti_windup_pid.c
g++ -o test_increment_pid               test_increment_pid.c
g++ -o test_integral_separate_pid       test_integral_separate_pid.c
g++ -o test_pid                         test_pid.c
g++ -o test_trapezoidal_integral_pid    test_trapezoidal_integral_pid.c
g++ -o test_variable_integral_pid       test_variable_integral_pid.c
# Execute
./test_anti_windup_pid           > test_anti_windup_pid_output.txt
./test_increment_pid             > test_increment_pid_output.txt
./test_integral_separate_pid     > test_integral_separate_pid_output.txt
./test_pid                       > test_pid_output.txt
./test_trapezoidal_integral_pid  > test_trapezoidal_integral_pid_output.txt
./test_variable_integral_pid     > test_variable_integral_pid_output.txt
# Check result
tail -3 test_anti_windup_pid_output.txt
tail -3 test_increment_pid_output.txt
tail -3 test_integral_separate_pid_output.txt
tail -3 test_pid_output.txt
tail -3 test_trapezoidal_integral_pid_output.txt
tail -3 test_variable_integral_pid_output.txt

