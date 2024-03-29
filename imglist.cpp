/**
 *  @file        imglist.cpp
 *  @description Contains partial implementation of ImgList class
 *               for CPSC 221 PA1
 *               Function bodies to be completed by student
 *
 *  THIS FILE WILL BE SUBMITTED
 */

#include "imglist.h"

#include <math.h> // provides fmax, fmin, and fabs functions

/*********************
 * CONSTRUCTORS, ETC. *
 *********************/

/**
 * Default constructor. Makes an empty list
 */
ImgList::ImgList()
{
    this->northwest = nullptr;
    this->southeast = nullptr;
}

/**
 * Creates a list from image data
 * @pre img has dimensions of at least 1x1
 */
ImgList::ImgList(PNG &img)
{

    // Initialize northwest and southeast pointers

    northwest = nullptr;
    southeast = nullptr;

    ImgNode *firstNodeUpperRow = nullptr;
    ImgNode *tempNode = nullptr;

    for (int y = 0; y < img.height(); y++)
    {
        ImgNode *prevNode = NULL; // Keep track of the previous node in the current row

        for (int x = 0; x < img.width(); x++)
        {
            ImgNode *newNode = new ImgNode();

            // Set the color of newNode based on the corresponding pixel in the input image
            newNode->colour = *img.getPixel(x, y);

            // Link newNode to the north and west nodes
            if (prevNode)
            {
                prevNode->east = newNode;
                newNode->west = prevNode;
            }

            if (y > 0 && firstNodeUpperRow)
            {
                // Find the corresponding node in the row above (north)
                ImgNode *northNode = firstNodeUpperRow;
                for (unsigned int i = 1; i <= x; i++)
                {
                    northNode = northNode->east;
                }

                newNode->north = northNode;
                northNode->south = newNode;
            }

            // Update northwest if it's the first node in the list
            if (x == 0 && y == 0)
            {
                northwest = newNode;
            }

            // Update prevNode for the next iteration
            prevNode = newNode;

            // Update southeast to point to the last node
            southeast = newNode;

            if (x == 0)
            {
                tempNode = newNode;
            }
        }

        // Set first NodeUpper Row after the row ends, so it will be of the upper row
        firstNodeUpperRow = tempNode;
    }
}

/************
 * ACCESSORS *
 ************/

/**
 * Returns the horizontal dimension of this list (counted in nodes)
 * Note that every row will contain the same number of nodes, whether or not
 *   the list has been carved.
 * We expect your solution to take linear time in the number of nodes in the
 *   x dimension.
 */
unsigned int ImgList::GetDimensionX() const
{
    // replace the following line with your implementation
    unsigned int result = 0;
    ImgNode *temp = this->northwest;

    while (temp)
    {
        result += 1;
        temp = temp->east;
    };

    return result;
}

/**
 * Returns the vertical dimension of the list (counted in nodes)
 * It is useful to know/assume that the grid will never have nodes removed
 *   from the first or last columns. The returned value will thus correspond
 *   to the height of the PNG image from which this list was constructed.
 * We expect your solution to take linear time in the number of nodes in the
 *   y dimension.
 */
unsigned int ImgList::GetDimensionY() const
{
    // replace the following line with your implementation
    unsigned int result = 0;
    ImgNode *temp = this->northwest;

    while (temp)
    {
        result += 1;
        temp = temp->south;
    };

    return result;
}

/**
 * Returns the horizontal dimension of the list (counted in original pixels, pre-carving)
 * The returned value will thus correspond to the width of the PNG image from
 *   which this list was constructed.
 * We expect your solution to take linear time in the number of nodes in the
 *   x dimension.
 */
unsigned int ImgList::GetDimensionFullX() const
{
    // replace the following line with your implementation
    unsigned int result = 0;
    ImgNode *temp = this->northwest;

    while (temp)
    {
        result += (temp->skipright + 1);
        temp = temp->east;
    };

    return result;
}

/**
 * Returns a pointer to the node which best satisfies the specified selection criteria.
 * The first and last nodes in the row cannot be returned.
 * @pre rowstart points to a row with at least 3 physical nodes
 * @pre selectionmode is an integer in the range [0,1]
 * @param rowstart - pointer to the first node in a row
 * @param selectionmode - criterion used for choosing the node to return
 *          0: minimum "brightness" across row, not including extreme left or right nodes
 *          1: node with minimum total of "colour difference" with its left neighbour and with its right neighbour.
 *        In the (likely) case of multiple candidates that best match the criterion,
 *        the left-most node satisfying the criterion (excluding the row's starting node)
 *        will be returned.
 * A note about "brightness" and "colour difference":
 * For PA1, "brightness" will be the sum over the RGB colour channels, multiplied by alpha.
 * "colour difference" between two pixels can be determined
 * using the "distanceTo" function found in RGBAPixel.h.
 */
ImgNode *ImgList::SelectNode(ImgNode *rowstart, int selectionmode)
{
    ImgNode *bestNode = rowstart->east;
    double bestValue = 9999999; // Initialize with maximum possible value

    // Start from the node next to the first node in the row and stop at the node before the last node
    ImgNode *currentNode = rowstart->east;
    while (currentNode->east)
    {
        // Compute the value based on the selection mode
        double value = 0.0;
        if (selectionmode == 0)
        {
            // Compute brightness: sum over the RGB color channels, multiplied by alpha
            value = (currentNode->colour.r + currentNode->colour.g + currentNode->colour.b) * currentNode->colour.a;
        }
        else if (selectionmode == 1)
        {
            // Compute color difference with left neighbor and right neighbor
            double leftDifference = currentNode->colour.distanceTo(currentNode->west->colour);
            double rightDifference = currentNode->colour.distanceTo(currentNode->east->colour);
            // Compute total color difference
            value = leftDifference + rightDifference;
        }

        // Update the best node if the current node has a lower value
        if (value < bestValue)
        {
            bestNode = currentNode;
            bestValue = value;
        }

        // Move to the next node
        currentNode = currentNode->east;
    }

    // Return the best node found
    return bestNode;
}

/**
 * Renders this list's pixel data to a PNG, with or without filling gaps caused by carving.
 * @pre fillmode is an integer in the range of [0,2]
 * @param fillgaps - whether or not to fill gaps caused by carving
 *          false: render one pixel per node, ignores fillmode
 *          true: render the full width of the original image,
 *                filling in missing nodes using fillmode
 * @param fillmode - specifies how to fill gaps
 *          0: solid, uses the same colour as the node at the left of the gap
 *          1: solid, using the averaged values (all channels) of the nodes at the left and right of the gap
 *          2: linear gradient between the colour (all channels) of the nodes at the left and right of the gap
 *             e.g. a gap of width 1 will be coloured with 1/2 of the difference between the left and right nodes
 *             a gap of width 2 will be coloured with 1/3 and 2/3 of the difference
 *             a gap of width 3 will be coloured with 1/4, 2/4, 3/4 of the difference, etc.
 *             Like fillmode 1, use the smaller difference interval for hue,
 *             and the smaller-valued average for diametric hues
 */
PNG ImgList::Render(bool fillgaps, int fillmode) const
{
    // Add/complete your implementation below

    PNG outpng; // this will be returned later. Might be a good idea to resize it at some point.

    return outpng;
}

/************
 * MODIFIERS *
 ************/

/**
 * Removes exactly one node from each row in this list, according to specified criteria.
 * The first and last nodes in any row cannot be carved.
 * @pre this list has at least 3 nodes in each row
 * @pre selectionmode is an integer in the range [0,1]
 * @param selectionmode - see the documentation for the SelectNode function.
 * @param this list has had one node removed from each row. Neighbours of the created
 *       gaps are linked appropriately, and their skip values are updated to reflect
 *       the size of the gap.
 */
void ImgList::Carve(int selectionmode)
{
    // add your implementation here
}

// note that a node on the boundary will never be selected for removal
/**
 * Removes "rounds" number of nodes (up to a maximum of node width - 2) from each row,
 * based on specific selection criteria.
 * Note that this should remove one node from every row, repeated "rounds" times,
 * and NOT remove "rounds" nodes from one row before processing the next row.
 * @pre selectionmode is an integer in the range [0,1]
 * @param rounds - number of nodes to remove from each row
 *        If rounds exceeds node width - 2, then remove only node width - 2 nodes from each row.
 *        i.e. Ensure that the final list has at least two nodes in each row.
 * @post this list has had "rounds" nodes removed from each row. Neighbours of the created
 *       gaps are linked appropriately, and their skip values are updated to reflect
 *       the size of the gap.
 */
void ImgList::Carve(unsigned int rounds, int selectionmode)
{
    // add your implementation here
}

/*
 * Helper function deallocates all heap memory associated with this list,
 * puts this list into an "empty" state. Don't forget to set your member attributes!
 * @post this list has no currently allocated nor leaking heap memory,
 *       member attributes have values consistent with an empty list.
 */
void ImgList::Clear()
{
    // add your implementation here
}

/**
 * Helper function copies the contents of otherlist and sets this list's attributes appropriately
 * @pre this list is empty
 * @param otherlist - list whose contents will be copied
 * @post this list has contents copied from by physically separate from otherlist
 */
void ImgList::Copy(const ImgList &otherlist)
{
    // add your implementation here
}

/*************************************************************************************************
 * IF YOU DEFINED YOUR OWN PRIVATE FUNCTIONS IN imglist-private.h, YOU MAY ADD YOUR IMPLEMENTATIONS BELOW *
 *************************************************************************************************/
