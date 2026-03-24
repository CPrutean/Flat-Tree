# Project Overview

This was a small exercise in making data structures in C for me as well as trying to write tests for C code using python which went well.

## Flat Tree Overview

This is an implementation of a flat binary search tree, which takes what a regular tree would look like, and turn it into one long continous block of memory to avoid heap fragmentation.

### The memory layout

A regular binary tree looks something like this

```
  (2)
  /\
 /  \
(1) (3)
/\  /\
x x x x
```

Each of the nodes contains 2 pointers, and a value. However when writing code for a much larger tree something like this can cause memory to fragment.

Instead we can arrange our memory Linearly we can avoid this fragmentation.

```
[2, 1, 3, NULL, NULL, NULL, NULL]
```

Notice how our root node is the first node if we take the array index of our root multiply by 2 (this becomes relevant later) and add 1, we arrive at our left node. If we apply the same formula and add 2 instead, we arrive at our right node. Lets say we want to include the 0 and 4. Our new tree would look like this.

```
   (2)
   /\
  /  \
 (1) (3)
 /\   /\
(0)x x (4)

```

And our memory like this.

```
[2, 1, 3, 0, NULL, NULL, 4]
```

We traverse down the tree, when we arrive at the node 1, we multiply by 2 and add by 1, giving us array index of 3. When we arrive at node 3 with an index of 2, we multiply by 2 and add 2, giving us array index of 6. Cleverly arranging our memory this way gives us nice continous block of memory to work with.

### Conclusion

Is this much better for memory, it depends. If you have enough memory and need the continous block it is worth it, however it is still an expensive data structure in cases where you arent going to populate each node. Values will just be empty and unsused and our memory will grow exponentially. However it allows us to control on large block for the data structure improving the general speed of our tree considering we dont have to search through all heap memory for different nodes.
