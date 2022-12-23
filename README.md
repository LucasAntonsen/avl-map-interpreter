# AVL-Map-Interpreter

### By Lucas J. Antonsen

---

Implementation of ```std::map``` using an AVL-tree (AVL-Map). Given the balanced nature of AVL-trees, the complexities below are observed for core functions in AVL-Map.

#### Complexity

Search: **O(log(n))**  
Insert: **O(log(n))**  
Delete: **O(log(n))**

---

#### Interpreter

Interpreter uses strings for keys and integers for values.

To run interpreter for AVL-Map:

```./interpreter```

To run with file input:

```./interpreter < input.txt```

#### Commands

```map_create```			Constructs empty map with the lowest natural number available as its tag.

```map_create 1```			Assigns a new empty map to tag $1$ if available.

```map_destroy 1```			Destroys map with tag $1$.

```map_move_assign 1 2```		Move assigns map $1$ to map $2$ (map $1$ = map $2$).

```map_move 1 2```			Moves map $1$ to map $2$ (map $2$(map $1$)).

```map_copy_assign 1 2```		Copy assigns map $1$ to map $2$ (map $1$ = map $2$).

```map_copy 1 2```			Copies map $1$ to map $2$ (map $2$(map $1$)).

```map_at ‘a’ 1```			Returns the value of key ‘a’ in map $1$.

```map_empty 1```			Tests whether map $1$ is empty.

```map_size 1```			Returns size of map $1$.

```map_max_size 1```			Returns maximum size of map $1$.

```map_clear 1```			Clears contents of map $1$.

```map_insert ‘a’ 2 3```		Inserts key ‘a’ with value $2$ into map $3$. If key ‘a’ already exists, do nothing.

```map_erase ‘a’ 2 3```		Erases key ‘a’ and its corresponding value $2$ from map $3$.

```map_swap 1 2```			Swaps contents of map $1$ with map $2$.

```map_print 1```			Prints the contents of map $1$.

```map_print_all```			Prints the contents of all maps.

```map_destroy_all```			Destroys all maps.

```map_begin 1 2```			Returns iterator $1$ pointing to first element in map $1$.

```map_end 1 2```			Returns iterator $1$ pointing to next place after last element in map $2$.

```iter_next 1```			Returns iterator $1$ pointing to next element in map based on map ordering.

```iter_prev 1```			Returns iterator $1$ pointing to previous element in map based on map ordering.

```iter_find ‘a’ 1```			Returns iterator to key ‘a’ if found, otherwise, returns not found.

```iter_print 1```			Prints key value pair pointed to by iterator $1$.

```iter_data_erase 1```		Erases element iterator $1$ refers to. The iterator now points to the next element in the map.

```iter_data_compare 1 2```		Compares the elements pointed at iterators $1$ and $2$.

```iter_destroy 1```			Deletes iterator $1$.
