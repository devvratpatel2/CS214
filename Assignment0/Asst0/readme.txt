Devvrat Patel 
CS 214 SPRING 2019 POINTERSORTER

This program takes in an argument from the command line, parses it in different sequence of alphabetic characters, and then sorts then in a descending order. The strings in the argument will be separated by non-alphabetic characters. For example,

"Cat cat zebra Apple":
Output-Cat
       Apple
       zebra
       cat

Here the argument is separated by spaces but they also can be sorted by digits and other separators including ,./` and many others. 

I have included more examples of the program in the testcases.txt.

Some problems that i encountered included how to sort the string in descending order. I was using strcmp at first and reversed the print order for the descending print but then that would print the uppercases differently as well. Therefore, in the end I just decided to create my own compareTo function which worked like strcmp but according to this assignment. There were also many problems that I faced with segmentation fault but i was able to fix it using an online gdb and debugging each line. 

Malloc was tricky at first and it took me some time understanding that and the pointers. 

I used Tree for my data structure to store the strings after separating them and that gave me an insertion complexity of log(n).

After the rotations were finished, the tree was done balancing and i just printed it out with simple in-order traversal method. This traversal also was used to free the noes and its word so that there is no memory leaks. This was tricky at first and took some trials and errors to work with it.
In the end the assignment was fair and was a good introductory assignment for C. I enjoyed working on this assignment. Some things I would do differently from before would be to start earlier than I did. I was busy with my other classes and wasn’t able to start until the last day. 
