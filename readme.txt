compile the code with 

g++ -o maze maze.cpp

and then run it like this:

./maze < input.txt > output.txt

in order to get a result saved in output.txt, which you can then compare with expected_result.txt like this:

vimdiff output.txt expected.txt


notes: 
-expected_result might have changed, copy the new one from moodle and paste it into expected_result.txt
-i used a linux terminal for this, windows might differ a bit, i recommend installing wsl (Windows Subsystem for Linux)