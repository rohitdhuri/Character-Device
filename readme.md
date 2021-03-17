# CS550 - Operating Systems
Spring 2020  
State University of New York at Binghamton  
Author: Rohit Mahendra Dhuri

### Description
Implementation of a character device to make a kernel module that prints all the processes running in the kernel

### COMPILATION AND EXECUTION
	-use the make file to compile the kernel module
	>sudo make

	-insert the kernel module using "insmod"
	>sudo insmod pdevice.ko

	-compile and run the user program
	>gcc user.c
	>./a.out

	-remove the kernel module using "rmmod"
	>rmmod pdevice

### Date : 03/29/2020