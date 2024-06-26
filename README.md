# Sequential-Steiner-Tree-heuristic

This is a course project for CENG4120 Computer-aided Design for Very Large Scale Integrated Circuits. In this project, we are required to make a program that routes all end nodes (called sink) to any one source node available, with the goal of reducing the wire length and runtime. 

I made use of a heuristic that is introduced in the lecture slides to build a Steiner tree sequentially. This method starts by finding a minimum bounding box(mbb) on a pair of points that have the shortest rectilinear distance. Then it continuously find the closest pin and create a mbb another mbb, while deleting the unwanted L-shaped route from the previous mbb. The loop stops when the set of unrouted pins is empty. Below is a pseudocode for the method:
1.	Find the closest pin pair, construct their minimum bounding box (mbb).
2.	Find the closest point pair (pmbb,pc) between any point pmbb on the mbb and pc from the set of pins to consider.
3.	Construct the mbb of pmbb and pc.
4.	Add the L-shape that pmbb lies on to the Tree (deleting the other L-shape). If pmbb is a pin, then add any L-shape of the mbb to the Tree.
5.	Loop to step 2 until the set of pins to consider is empty.
6.	For the last pin, add (delete) any one of the two L-shape.

This project is implemented in C.


Example visual for the process:

![ezgif-2-edb8d11cb8](https://github.com/Ian-HJQ/Sequential-Steiner-Tree-heuristic/assets/71361819/d057c493-6723-4c08-80e4-01b797486659)

Example output plotted with provided matplotlib script:

![Test case 5](https://github.com/Ian-HJQ/Sequential-Steiner-Tree-heuristics/assets/71361819/3127d341-55a2-48f4-864b-b768923757e4)
