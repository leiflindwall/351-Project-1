# 351-02-Project-1
Inter-Process Communications Assignment

Group Members:

Michael Lindwall michaellindwall@csu.fullerton.edu

Alexander Lavrov alavrov@csu.fullerton.edu

Marc Liander Reyes mlcr@csu.fullerton.edu

Trung Ngo ndtrung95@csu.fullerton.edu

Instructions:

This project was written in C/C++ and is meant to be run in a UNIX-style
command line terminal that supports GCC, Makefiles, and command-line arguments.

1) Type "make" in a terminal window in the folder that contains the code to
  compile the programs.

2) First, run the receiver program by typing "./recv" after compiling.

3) Then, open another terminal in the same directory.  Now, in this second
  terminal, type "./sender <FILENAME>" which <FILENAME> is the name of the
  file to read and send.  The file used for testing was "msg.txt".

4) The two programs will synchronously transfer the data from the input file
  to the output file, which is always called "recvfile".

5) To break out of the programs in the terminal, press "CTRL-C".

6) To remove compiled object files, type "make clean" in the directory
  containing the files.  


Team Collaboration:

We collaborated on this project by using a Github repository.  Michael began the
project by creating the initial commit.  From there, whoever downloaded the
project and made progress pushed to the master branch with comments indicating
what changes and progress were made.  Michael and Marc worked out the sender
process and Alexander and Trung tackled the sender process. After the project
was fleshed out, Alexander created the design document illustrating the logical
 flow of the two processes.

Extra Credit: N/A

Special Notes: N/A
