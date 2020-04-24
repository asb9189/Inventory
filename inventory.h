
//@Contributing Author Aleksei Bingham (asb9189)

//File Description: This header file
//is imported into the inventory.c file and provides
//nemerous functions and structs for inventory.c
//this header file also provides trim.c's implementation
//of functions to be used for trimming and obtaining lines
//read by external files passed to inventory.c

#ifndef INVENTORY_H
#define INVENTORY_H

extern char * trim(char *);
extern int getline(char **, size_t *, FILE *);

#define ID_MAX 11

// part
struct part {
    char id[ID_MAX+1];        // ID_MAX plus NUL
    struct part * next; // the next part in the list of parts
};

// assembly
struct assembly {
    char id[ID_MAX+1];
    int capacity;
    int on_hand;
    struct items_needed * items; // parts/sub-assemblies needed for this ID
    struct assembly * next;      // the next assembly in the inventory list
};

// inventory item (a part or an assembly)
struct item {
    char id[ID_MAX+1];           // ID_MAX plus NUL
    int quantity;
    struct item * next; // next item in the part/assembly list
};

// the inventory (parts and assemblies)
struct inventory {
    struct part * part_list;         // list of parts by ID
    int part_count;                  // number of distinct parts
    struct assembly * assembly_list; // list of assemblies by ID
    int assembly_count;              // number of distinct assemblies
};

// parts/sub-assemblies needed to make required assemblies
struct items_needed {
    struct item * item_list;
    int item_count;
};

// req struct (not used)
struct req {
    char * req_string;
    void (*req_fn)(struct inventory *);
};

//typedef's for all structs
typedef struct inventory inventory_t;
typedef struct items_needed items_needed_t;
typedef struct item item_t;
typedef struct part part_t;
typedef struct assembly assembly_t;

//returns pointer to part if part (id) is in invetory
part_t * lookup_part(part_t * pp, char * id);

//returns pointer to assembly struct (id) if in inventory
assembly_t * lookup_assembly(assembly_t * ap, char * id);

//returns pointer to item struct if item (id) is in inventory
item_t * lookup_item(item_t * ip, char * id);

//adds part to inventory given pointer to inventory and part id
void add_part(inventory_t * invp, char * id);

//adds an assembly into the inventory
//given invp pointer, assembly id, assembly capacity,
//a pointer to items struct
void add_assembly(inventory_t * invp,
                  char * id,
                  int capacity,
                  items_needed_t * items);

//adds an item into the given items needed struct (items)  given the items
//specifications such as the item id and quantity
void add_item(items_needed_t * items, char * id, int quantity);

// these are used for sorting purposes
part_t ** to_part_array(int count, part_t * part_list);

//converts the assembly list into an array 
assembly_t ** to_assembly_array(int count, assembly_t * assembly_list);

//converts item linked list into array of items
item_t ** to_item_array(int count, item_t * item_list);

//compare function for parts to be used in q-sort
int part_compare(const void *, const void *);

//compare function for assembly to be used in q-sort
int assembly_compare(const void *, const void *);

//compare function for items to be used in q-sort
int item_compare(const void *, const void *);

//the make function manufactures assembles recursivley
void make(inventory_t * invp, char * id, int n, items_needed_t * parts);

//the get function gets the needed parts required for make to do its job
void get(inventory_t * invp, char * id, int n, items_needed_t * parts);

//prints the inventory
void print_inventory(inventory_t * invp);

//prints the parts linked list in alphabetical order
void print_parts(inventory_t * invp);

//prints the items needed in alphabetical order
void print_items_needed(items_needed_t * items);

//free's the inventory of dynamically allocated space
void free_inventory(inventory_t * invp);

#endif // INVENTORY_H
