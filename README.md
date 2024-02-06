# CPSC221_PA1
Goals and Overview
In this PA (Programming Assignment) you will:

learn about the course programming environment
learn about vectors
learn about pointers
learn about linked lists
learn about management of dynamic memory
The Assignment
Problem Specification
A Linked List is a dynamic linear structure designed to hold any type of data. In this exercise, we develop and use a 2-dimensional doubly-linked list to manipulate pixels from an image. (Technically, a 2-D linked list is not a linked list at all. This is more like a linked representation of a 2-D array, grid, or matrix. Structures like this are commonly used, though in use they typically store arrays of pointers to the start of each row/column. The gaps or holes we allow in our grid are especially useful for representing enormous matrices where much of the matrix may be simply zeroes.)

 

We define a class called ImgList, which is a 2-dimensional doubly-linked list constructed out of ImgNode objects.

An ImgNode consists of member attributes that describe:

its colour
four ImgNode pointers linking to its neighbours to the north/east/south/west, if available
four integers describing how many pixels spaces to fill during reconstruction of the overall image data, if any neighbours have been deleted from the list
The ImgList provides entry into the linked structure at two locations (northwest and southeast) and an example 8x8 structure with its (enlarged) image blueprint is shown below:



Any node that has fewer than four outgoing pointers will have a value of NULL for each pointer not illustrated.

 

In this assignment, you will construct an ImgList object using RGBAPixel data from an input PNG image, implement a function which removes a node from each row of nodes, and reconstruct and render a PNG image from (possibly reduced) ImgList content.

We have provided a starting point for achieving this functionality. It is your task to complete and expand on our implementation.

The most notable feature of this structure is the ability to "carve" away one or more nodes from each row in the ImgList, thereby reducing the amount of memory needed to store the ImgList object. When removing nodes, the "skip" attributes of neighbouring nodes must be updated to reflect the number of nodes that previously existed in the gap which gets created/enlarged. An example list with one node removed from each row is illustrated below:


A subsequent round of removal (for a total of 2 pixels removed from each row) is illustrated below. Pay special attention to the "skip" values of each node that is adjacent to a gap:


Note that the specific nodes selected for removal in the figures above may not accurately match the selection criteria which you are asked to implement; they are shown only to illustrate how the pointer links and skip values should be updated.

Specifications for each function you write are contained in the given code. The list of functions here should serve as a checklist for completing the exercise.

In imglist.cpp
ImgList(): default constructor creates an empty list.
ImgList(PNG& img): constructs a new list using pixel data from img.
void Clear(): Helper function for destructor and assignment operator.
void Copy(const ImgList & otherlist): (CHALLENGING! DO THIS LAST) Helper function for copy constructor and assignment operator. BEWARE: Copying the north/south links correctly is challenging, but you can do it in 
 time (using 
 additional space) or 
 time.
unsigned int GetDimensionX() const: retrieves the number of nodes in each row of the list; the horizontal dimension of the list.
unsigned int GetDimensionY() const: retrieves the number of nodes in the tallest column of the list; the vertical dimension of the list. This will always correspond to the original height of the PNG image used to construct the list.
unsigned int GetDimensionFullX() const: retrieves the original pixel width of the PNG image used to construct the list. Computed using skip values.
ImgNode* SelectNode(ImgNode* rowstart, int selectionmode): From the row beginning with rowstart, returns a pointer to the node which best satisfies the selection criteria determined by selectionmode. The selected node MUST have non-NULL left and right neighbours.
void Carve(int selectionmode): Removes a node from every row in the list. Hint: use the SelectNode function.
void Carve(unsigned int rounds, int selectionmode): Removes rounds number of nodes from each row of the list.
PNG Render(bool fillgaps, int fillmode) const: Draws the entire ImgList's pixel data into an appropriately-sized PNG and returns it. fillgaps and fillmode are used to determine whether a (possibly carved) list is rendered to its current dimensions, or to its pre-carving dimensions, and how any gaps due to carving are filled. (fillmode == 2 uses a linear interpolation to fill the gaps. See the documentation in imglist.cpp for details.
Implementation Constraints and Advice
We will be grading your work on functionality, efficiency, and memory use. All ImgList functionality, aside from the constructor(s) and the copy functions, can be achieved by manipulating node pointers, rather than by allocating new ones and/or making copies. If you are tempted to use the new function when you are manipulating the ImgList, ask yourself if you can achieve your goal by reassigning pointers, instead.

Getting the Given Code
Download the source files here pa1-20240131-1959.zip and follow the procedures you learned in lab to move them to your home directory on the remote linux machines or to set up a project in a local development environment.

Handing in your code
Pair work is encouraged. Have one member of your partnership create a group here on PrairieLearn, and add the other member's CWL to the group. Once both members have been added, you can jointly submit and also view previous submissions.

Submit your files for PA1 grading here! These are the only files used for grading.
