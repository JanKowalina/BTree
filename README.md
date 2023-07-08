# B-tree

**Note:** This project is based on "Introduction to Algorithms" by T. H. Cormen et al. and lecture slides during the implementation of this program.

The goal of this project is to implement a B-tree and measure an enhanced structure where an amount of cache can be used.

## Instructions

The tree is as described during the lecture, i.e., a tree of order `t` has to store at least `t-1` elements in each node (except the root) and at most `2t-1`.

First, to implement the B-tree, the basic operations are:

- `I x` - Construct a tree of order `x`.
- `A x` - Add the value `x` to the tree, where `x` is a signed integer. The values added are unique, i.e., there will be no two insertions of the same value (except in the case when it was deleted in between).
- `? x` - Check if the value `x` is in the tree. If it is present, print `"x +"`, otherwise print `"x -"`.
- `P` - Print all values that are in the tree in increasing order.
- `L t` - Load a tree of order `t`. In the next line, there is a tree in a format given as follows: A pair of brackets `( )` signify a node, and a value signifies a record (key in the tree).
  - For example, `( ( ( 1 2 ) 3 ( 4 ) 5 ( 6 7 ) ) 8 ( ( 9 ) 10 ( 11 ) ) )` can be interpreted as the following tree:
    ```
                                        (         .           8       .    )
                                        (  . 3    .  5   .  )   ( . 10  .  )
                                        ( 1 2 ) ( 4 ) ( 6 7 )   ( 9 ) ( 11 )
    ```
- `S` - Save the tree in the format described above.
- `R x` - Remove the element `x` from the tree. The element may or may not be present. This operation might not change the elements in the tree but may force reorganization.
- `#` - Ignore the line.
- `X` - End the program.
- `C x q1, q2, ...` - Calculate the impact of a hypothetical cache.
  - Assume that you are given a set of queries `q1, ..., qn` (the list of queries ends with a new line character).
  - The program can store the result of the query in a cache of size `x` (measured in the number of entries/keys that can be stored).
  - If the next queried entry was already visited, the program does not have to visit the B-tree but may simply read the data from the cache.
  - Due to the fact that the size of the cache is much smaller than the size of the B-tree, only chosen entries can be stored.
  - The policy that has to be implemented is First-In-First-Out (FIFO). The first entry that was read into the cache is removed from it when some other entry is read from the B-tree, and the result has to be added to the cache.
  - Keep in mind that "Not found" is a valid result to store in the cache.
  - The measure of the impact of the cache is given by the number of accesses to the disk (measured as depth in the B-tree).


## Compilation and Execution

To compile the program, follow these steps:

1. Use a C++ compiler (such as g++) to compile the code:

   ```shell
   g++ -o BTree BTree.cpp
   ```

2. Run the program using the following command:

   ```shell
   ./BTree
   ```

   The program will start running and wait for user input.


## Example

**Input:**
```
I 2
A 2
A 4
A 5
A 1
? 2
? 3
X
```

**Output:**
```
2 +
3 -
```

**Input:**
```
I 2
A 2
A 4
A 5
A 1
P
X
```

**Output:**
```
1 2 4 5
```

**Input:**
```
I 2
A 2
A 4
A 5
A 1
S
X
```

**Output:**
```
2
( ( 1 2 ) 4 ( 5 ) )
```

**Input:**
```
L 2
( ( 1 2 ) 4 ( 5 ) )
? 1
? 10
X
```

**Output:**
```
1 +
10 -
```

**Input:**
```
I 2
A 2
A 4
A 5
? 1
R 1
? 1
A 1
? 1
X
```

**Output:**
```
1 +
1 -
1 +
```

**Input:**
```
I 2
A 2
A 4
A 5
C 2 2 10 2 2 4 2
X
```
