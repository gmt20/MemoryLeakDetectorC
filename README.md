# MemoryLeakDetectorC

This is a small Memory Leak Detector library written in C. 
Following files are included in the code:
MLD.h - header file for the library
MLD.c - file consisting of all the definitions of MLD functions
app.c - file consisting of test cases for MLD library
How it works? 
First, any call to malloc, calloc or free (library functions used by C program from allocation and deallocation of memory) is replaced by xmallox, xcalloc and xfree.
Second, the xmalloc and xcalloc internally calls the respective library functions to allocate memory and then calls the add_node function dfeined in MLD library.
This add node function creates a struct node consisting of information about the memory allocated including the address of memory allocated, its size, name and line number. We internally maintains a linked list of all the struct memory nodes created. 

Similarly, xfree function not only calls the free function internally but also remove the node  related to that address from our linked list.
Finally, the report leak function can be run at the end of test program to print out the list of memory nodes still not freed.

Commands to run the MLD library with any C program(app.c in my case).

![alt text](https://github.com/gmt20/MemoryLeakDetectorC/blob/main/1.PNG)

The leaked memory locations are logged in leak-info.txt file under /home directory.

![alt text](https://github.com/gmt20/MemoryLeakDetectorC/blob/main/2.PNG)
