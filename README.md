# TetrisDuel

# Introduction
Two players are able to follow the classic Tetris rules and take turns to compete.
This is a non-runtime game with a text display on command line and a graphics display on X11.

# Overview
The program starts from the main function where all the commands are read in. Then, it calls the corresponding methods from the Controller class.  

1. Main - input and output
2. Controller - manages essential modules
3. Grid - controls the “boards” of gameplay
4. Block - directs the blocks’ movement
5. NextBlock & Level -  generates new blocks based on specification
6. TextDisplay & GraphicsDisplay - gives visual representations of the gameplay 
7. Special Actions - put a Decorator wrapper around the blocks


# Design
Instead of having different modules taking charge of inputting and outputting, we designed a main function that implements command line arguments, takes/interprets all input commands, reads sequence/command files, and directs standard output and error. It creates a Controller class that initializes and stores two pointers to players’ Grids, a pointer to TextDisplay as well as another pointer to GraphicsDisplay. Thus, the main function calls commands on the controller, which then dissects the commands if necessary and calls commands on their corresponding Grid. The Grids therefore manage current blocks and generate next blocks. Specific implementations on essential modules are as follows:

## *Displays*
We utilized the Observer design pattern and created an abstract Observer class which manages all the displays. Two children classes, TextDisplay and GraphicsDisplay, inherit from Observer and override functions in Observer. Grid stores Observer pointers and notify its Observers whenever it changes. 
Thus, if we were to create a new display, we could write a new class that inherits from Observer and store the pointer to that new display class in Grid. By doing this, besides implementing the new display, attaching another pointer to Grid would be the only change needed to create a new display; Grid could still call the virtual notify in Observer to make any changes.


## *Blocks*
Firstly, we changed the return value for the virtual makeBlock(Grid &, CellType) function from CellType to std::shared_ptr<Block>. Thus, every time we try to call this function in the Gird class, we could simply return a pointer of a Block instead of using a switch statement and make a block based on the given CellType.
We also added a reference to the Grid as a private field, so the concrete Baseblock class takes responsibility for checking if it is valid to move or rotate the current block. Since the blocks are vectors of Point structures, we noticed that all types of blocks except for O and I are rotated in order as a 2 X 3 grid where a set of specific Points have their state being true based on the current block’s CellType. Hence, it is necessary to have a getter function of the current block’s point, so the grid could have access to the field and check if it should notify the Cell to turn on or not.
In addition, we added some public functions for the Heavy feature since it is applied to individual blocks. To satisfy the scoring scheme, we also have an addCount(): Void function. When one of the cells of a block is cleared, we call the addCount() to increment the number of cleared cells in a block. When it reaches 4, we know the entire block has been cleared so we could add specific scores based on the level we were in when the block was generated. 

## *NextBlock & Levels*
Similar to the BaseBlock class, Levels are concrete subclasses that inherit from the NextBlock class. Each level is defined in its own subclasses with override method makeNextBlock(Grid & grid): std::shared_ptr<Block>. Once the user changes the level, we create a new class for Nextblock in Grid and call the makeNextBlock(Grid & grid) function on the class, we could then update the next block after the shown next block (which is not generated yet) accordingly.

## *Special Actions*
Special Actions used the Decorator design pattern and it acts like a wrapper on the BaseBlock. It enables behaviours to be applied to the BaseBlock without adding new functions or fields to the BaseBlock; instead, when certain commands are called on Block, it first applies some effects on the BaseBlock before calling the commands on BaseBlock.
We also designed each special action as concrete classes inherited from the abstract Decorator class. Thus, if we want to create a new special action, no changes are needed to make to other special actions classes.

# Resilience to Change
## *High Cohesion and Low Coupling*
Our program followed the high cohesion and low coupling designing principle. We tried to structure a module so that each of the elements in that module has functionalities that belong together and serve the same purpose (High Cohesion). We also ensured that we keep dependencies between modules as few as possible (Low Coupling). We achieved this by doing the followings:

*Blocks*
We used a Block abstract class with a concrete subclass Baseblock. The whole Block class is responsible for one single purpose, which is the block movement. This has shown high cohesion in our code. In addition, if we wish to add more blocks to the grid, we could simply modify the Baseblocks class without the need to modify any other classes. This has shown low coupling in our code. 

*Observer*
Our initial design actually had heavy dependencies in between each display class; each time the grid changes, it notifies the TextDisplay, and the TextDisplay then notifies the GraphicsDisplay. We thought doing this would make our program more efficient. However, this design was not maintainable as we couldn’t easily modify the implementations in both of the displays when we wanted to. 
We then used the Observer design pattern where we linked both of the displays to an abstract Observer class. We stored pointers to the two observers in the Grid and the Grid calls notify on displays if needed. By doing this, if we wanted to change something in one display, we could simply do so without worrying that it might affect the other display. Even if we wanted to add another display, we could just write another concrete class under Observer and store a pointer to it in the Grid.

*Levels*
Similar to the Blocks class, Level 0 to Level 5 are concrete subclasses, which are only responsible for creating the nextblock based on the grid’s current level. This design has made our code more maintainable when we wish to add more levels.
