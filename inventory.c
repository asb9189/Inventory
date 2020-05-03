
//@Author Aleksei Bingham

//Project Description: Inventory is a massive c driven project
//that simulates a generic company and allows for the manufacturing
//of assemblies. These assemblies contain parts and sub assemblies
//that must all be stored in structs defined in inventory.h and must
//be displayed properly and in order with qsort function and 
//comparator methods also provided to us by inventory.h but implemented
//here. The inventory project allows users to manually type in
//requests from stdin or pass a file with both comments and requests
//to be read and executed. This project is broken down into neumerous steps
//such as reading from stdin or file, parsing read information, passing
//information onto appropriate function and displaying in alphabetical order.


//#define _GNU_SOURCE
//#define _DEFAULT_SOURCE
#define MAX_LINE 80

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "inventory.h"
#include "trimit.h"

//global variable
inventory_t * invp;

//checks if a part id exists in the inventory
//@param id: id of part to be checked
//@return int: 0 false, 1 true
int part_in_inventory(char * id) {
    
    part_t * temp = invp->part_list;

    if (temp == NULL) {
        return 0;
    }

    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0) {
	    return 1;
	}
	temp = temp->next;
    } 
    return 0;
}

//adds a part to the invp part_list
//@parm invp: inventory struct
//@param id: id of part
void add_part(inventory_t * invp, char * id) {

    part_t * part = malloc(sizeof(part_t));
    strcpy(part->id, id);
    part->next = NULL;

    if (invp->part_list == NULL) {
        invp->part_list = part;
	invp->part_count += 1; //recently added
    } else if (part_in_inventory(id) == 1) {
        fprintf(stderr, "!!! %s: duplicate part ID\n", id);
	free(part);
	return;
    } else {
        part_t * temp  = invp->part_list;
        while (temp->next != NULL) {
	    temp = temp->next;
	}
	
	temp = invp->part_list;
	part->next = temp;
	invp->part_list = part;
	invp->part_count += 1; //recently added
    }
}

//adds an assembly struct to the list of assemblies
//@param invp: inventory
//@param id: id of new assembly
//@param capacity: capacity of new assem
//@param items: a linked list of items needed to manufacture assembly
void add_assembly(inventory_t * invp, char * id, int capacity, 
		items_needed_t * items) {

    assembly_t * assembly = malloc(sizeof(assembly_t));
    strcpy(assembly->id, id);
    assembly->capacity = capacity;
    assembly->on_hand = 0;
    assembly->items = items;
    assembly->next = NULL;

    if (invp->assembly_list == NULL) {
        invp->assembly_list = assembly;
	invp->assembly_count += 1;
    } else {
        assembly_t * temp = invp->assembly_list;
	while (temp->next != NULL) {

	    //check for duplicates
	    if (strcmp(temp->id, id) == 0) {
	        fprintf(stderr, "!!! %s: duplicate assembly ID\n", id);
		free(assembly);
		free(items);
		return;
	    }
	    temp = temp->next;
	}

	if (strcmp(temp->id, id) == 0) {
	    fprintf(stderr, "!!! %s: duplicate assembly ID\n", id);
	    free(assembly);
	    free(items);
	    return;
        }

	temp = invp->assembly_list;
        assembly->next = temp;
	invp->assembly_list = assembly;
	invp->assembly_count += 1;
    }
}

//adds an item to the items list
//@param items: list to be added too
//@param id: id of new item
//@param quantity: quantity of new item
void add_item(items_needed_t * items, char * id, int quantity) {

    item_t * temp = items->item_list;

    if (temp == NULL) {
        item_t * item = malloc(sizeof(item_t));
	strcpy(item->id, id);
	item->quantity = quantity;
	item->next = NULL;
	items->item_count += 1;
	items->item_list = item;
	return;
    }

    while (temp->next != NULL) {
        if (strcmp(id, temp->id) == 0) {
	    temp->quantity = temp->quantity + quantity;
	    return;
	}
	temp = temp->next;
    }

    if (temp != NULL) {
        if(strcmp(id, temp->id) == 0) {
	    temp->quantity = temp->quantity + quantity;
	    return;
	}
    }

    //The ID was not in the list
    item_t * item = malloc(sizeof(item_t));
    strcpy(item->id, id);
    item->quantity = quantity;
    
    
    temp = items->item_list; //get the head of the linekd list
    item->next = temp;
    items->item_list = item;
    
    items->item_count += 1; //recently added this line to fix parts needed

}

//prints the parts inside the inventory in alphabetical order
//using the c libary qsort function
void parts() {

    printf("Part inventory:\n");
    printf("---------------\n");
    
    if (invp->part_list == NULL) {
        printf("NO PARTS\n");
	return;
    }

    printf("Part ID\n");
    printf("===========\n");

    part_t ** part_array = to_part_array(invp->part_count, invp->part_list);
    qsort(part_array, invp->part_count, sizeof(part_array[0]), part_compare);

    for (int i = 0; i < invp->part_count; i++) {
        printf("%s\n", part_array[i]->id);
    }

    free(part_array);
    
}

//retrieves an assembly object from the inventory
//@param str: the id of the assembly to retreieve
//@return assembly_t *: a pointer to an assembly struct 
assembly_t * get_assembly(char * str) {

    assembly_t * temp = invp->assembly_list;

    if (temp == NULL) {
        return NULL; //return false
    }

    while (temp->next != NULL) {
        if (strcmp(temp->id, str) == 0) {
	    return temp; //return true
	}
        temp = temp->next;
    }

    if (temp != NULL) {
        if(strcmp(temp->id, str) == 0) {
	    return temp; //return true
	}
    }

    return NULL; //return false;

}

//checks to see if an assembly exists in the inventory
//@param str: id of the assembly to check
//return int: 0 false, 1 for true
int id_in_assembly(char * str) {

    assembly_t * temp = invp->assembly_list;

    if (temp == NULL) {
        return 0; //return false
    }

    while (temp->next != NULL) {
        if (strcmp(temp->id, str) == 0) {
	    return 1; //return true
	}
        temp = temp->next;
    }

    if (temp != NULL) {
        if(strcmp(temp->id, str) == 0) {
	    return 1; //return true
	}
    }

    return 0; //return false;

}

//prints the parts needed struct in alphabetical order
//@param parts: pointer to the items_needed_t struct
//containing the parts to be printed
void print_parts_needed(items_needed_t * parts) {

    if (parts->item_count == 0) {
        //do not show anything
    	return;
    }

    printf("Parts needed:\n");
    printf("-------------\n");
    printf("Part ID     quantity\n");
    printf("=========== ========\n");

    item_t ** item = to_item_array(parts->item_count, parts->item_list);
    qsort(item, parts->item_count, sizeof(item[0]), item_compare);

    for (int i = 0; i < parts->item_count; i++) {
        printf("%-11s %8d\n", item[i]->id, item[i]->quantity);
    }

    free(item);

}

//stock will stock an assembly inventory
//and call make / get functions inorder to get parts
//that may be required to do so
//@param str: the assembly id to be stocked
void stock(char * str) {

    items_needed_t * parts = malloc(sizeof(items_needed_t));
    parts->item_list = NULL;
    parts->item_count = 0;

    //skip over request type
    strtok(str, " ");
    char * id = strtok(NULL, " ");
    int n = strtol(strtok(NULL, " "), NULL, 10);

    if (!id_in_assembly(id)) {
        fprintf(stderr, "!!! %s: assembly ID is not in the inventory\n", id);
	free(parts);
	return;
    }

    if (n <= 0) {
        fprintf(stderr, "!!! %d: illegal quantity for ID %s\n", n, id);
	free(parts);
        return;
    }

    //get the assembly object with the id
    assembly_t * temp = get_assembly(id);

    if (temp->capacity == temp->on_hand || temp->capacity == 0) {
        free(parts);
        return; //no instances should be manufactured
    }

    if (temp->on_hand + n > temp->capacity) {
        n = temp->capacity - temp->on_hand;
    }

    make(invp, id, n, parts);

    temp->on_hand += n;

    //print and free parts
    print_parts_needed(parts);
    free(parts);

}

//tokenize_addPart just breaks up the addPart
//request into its appropriate pieces while also error checking
//@param str: the request
void tokenize_addPart(char * str) {

    //addPart ID
    strtok(str, " "); //skip over request type
    char * id = strtok(NULL, " ");

    //error handling
    if (id[0] != 'P') {
        fprintf(stderr, "!!! %s: part ID must start with 'P'\n", id);
	return;
    }

    if (strlen(id) > 11) {
        fprintf(stderr, "!!! %s: part ID too long\n", id);
	return;
    }

    add_part(invp, id);
}

//creates an item object and returns a pointer to it
//@param id: id of item
//@param quantity: quantity of the item
//@return item_t *: a poitner to a new item struct
item_t * create_item(char * id, char * quantity) {

    item_t * item = malloc(sizeof(item_t));
    strcpy(item->id, id);
    item->quantity = strtol(quantity, NULL, 10);
    item->next = NULL;

    return item;

}

//creates the items needed when the optional arguments
//are supplied in the addAssembly request.
//this also parses them out and error checks to make sure
//the addAssembly request is legal
//@param str: string containing addAssembly optional request
//@return items_needed_t *: a pointer to a new items needed struct
items_needed_t * create_items_needed(char * str) {

    items_needed_t * items_needed = malloc(sizeof(items_needed));
    items_needed->item_list = NULL;
    items_needed->item_count = 0;

    char * item_id = strtok(str, " ");
    char * item_quantity = strtok(NULL, " ");
    int item_q = strtol(item_quantity, NULL, 10);

    if (item_id[0] == 'A') {
        if (!id_in_assembly(item_id)){
	    fprintf(stderr, "!!! %s: part/assembly ID is not in the "
			    "inventory\n", item_id);
	    free(items_needed);
	    return NULL;
	}
    } else {
        if (!part_in_inventory(item_id)) {
	    fprintf(stderr, "!!! %s: part/assembly ID is not in the " 
			    "inventory\n", item_id);
	    free(items_needed);
	    return NULL;
	} 
    }	

    if (item_q <= 0) {
        if (item_id[0] == 'A') {
	    fprintf(stderr, "!!! %d: illegal quantity for ID Assembly\n"
			    , item_q);
	    free(items_needed);
	} else {
	    fprintf(stderr, "!!! %d: illegal quantity for ID Part\n"
			    , item_q);
	    free(items_needed);
	}
	return NULL;
    }

    //make our first item with first ordered pair
    items_needed->item_list = create_item(item_id, item_quantity);
    items_needed->item_count += 1;
         
    while(1) { 
        
        item_id = strtok(NULL, " ");
        item_quantity = strtok(NULL, " ");

        if (item_id == NULL) {
	    break;
	}

        //check to make sure assembly id or part id exists in inventory
	if (item_id[0] == 'A') {
	    if (!id_in_assembly(item_id)){
	        fprintf(stderr, "!!! %s: part/assembly ID is not in the "
				"inventory\n", item_id);
		free(items_needed);
		return NULL;
	    }
	} else {
	   if (!part_in_inventory(item_id)) {
	       fprintf(stderr, "!!! %s: part/assembly ID is not in the " 
			       "inventory\n", item_id);
	       free(items_needed);
	       return NULL;
	   } 
	}	

        if (strtol(item_quantity, NULL, 10) <= 0) {
            if (item_id[0] == 'A') {
	        fprintf(stderr, "!!! %d: illegal quantity for ID Assembly\n",
			       	item_q);
		free(items_needed);
	    } else {
	        fprintf(stderr, "!!! %d: illegal quantity for ID Part\n",
			       	item_q);
		free(items_needed);
	    }
	    return NULL;
        }

	//add duplicates together!
	
	//if its a part check if it already exists in
	//list and if it does combine quantities, add one
	//to the item_count and continue
	//
	//same to be done with assemblies
	
	int dup = 0;
        
	if (item_id[0] == 'A') {
	    item_t * temp = items_needed->item_list;
	    while (temp != NULL) {
	        if (strcmp(temp->id, item_id) == 0) {
		    temp->quantity += strtol(item_quantity, NULL, 10);
		    dup = 1;
		    break;
		}
		temp = temp->next;
	    }
	} 

	if (item_id[0] == 'P') {
	    item_t * temp = items_needed->item_list;
	    while (temp != NULL) {
	        if (strcmp(temp->id, item_id) == 0) {
		    temp->quantity += strtol(item_quantity, NULL, 10);
		    dup = 1;
		    break;
		}
		temp = temp->next;
	    }
	}

	if (dup) {
	    continue;
	}

	//add the next item into the items_needed linked list
        item_t * temp = items_needed->item_list;

       	//fixes input 9 diff but now input 8 does not work correctly
	item_t * new_item = create_item(item_id, item_quantity);
	new_item->next = temp;
	items_needed->item_list = new_item;
	items_needed->item_count += 1;
    }
    return items_needed;

}

//tokenizes the addAssembly request for further error checking
//@param str: the addAssembly request directly from the user
void tokenize_addAssembly(char * str) {
    
    //trim the string before processing to avoid errors
    str = trim(str);
    //skip over request type
    strtok(str, " ");
    char * id = strtok(NULL, " ");

    if (strlen(id) > 11) {
        fprintf(stderr, "!!! %s: assembly ID too long\n", id);
	return;
    }

    char * p_capacity = strtok(NULL, " ");
    int capacity = strtol(p_capacity, NULL, 10);

    if (id[0] != 'A') {
        fprintf(stderr, "!!! %s: assembly ID must start with 'A'\n", id);
	return;
    }

    if (strlen(id) > 11) {
        fprintf(stderr, "!!! %s: assembly ID too long\n", id);
	return;
    }
    
    if (capacity < 0) {
        fprintf(stderr, "!!! %d: illegal capacity for ID %s\n", capacity, id);
	return;
    }
    
    //Use remaining to create items_needed struct
    char * remaining = strtok(NULL, "");
    
    if (remaining == NULL) {
	items_needed_t * items = malloc(sizeof(items_needed_t));
	items->item_list = NULL;
	items->item_count = 0;

        add_assembly(invp, id, capacity, items);
	return;
    }

    items_needed_t * items = create_items_needed(remaining);

    if (items == NULL) {
        return;
    }

    add_assembly(invp, id, capacity, items);
}

//removes comments from a given char *
//@param str: the string to have its comments removed
void remove_comments(char * str) {

    char * temp = strchr(str, '#');
    if (temp != NULL) {
        *temp = '\0';
    }

}

//prints the inventory in alphabetical order
//prints differently depending on supplied argument or not
//@param str: the id to be printed if specified
void inventory(char * str) {
   	
    //skip the first part
    strtok(str, " ");
    char * id = strtok(NULL, " ");

    //print everything
    if (id == NULL) {

    	printf("Assembly inventory:\n");
        printf("-------------------\n");

        if (invp->assembly_list == NULL) {
	    printf("EMPTY INVENTORY\n");
	    return;
	}
        printf("Assembly ID Capacity On Hand\n");
        printf("=========== ======== =======\n");

	assembly_t ** temp = to_assembly_array(invp->assembly_count,
		       	invp->assembly_list);

	qsort(temp, invp->assembly_count, sizeof(temp[0]), assembly_compare);
        
	for (int i = 0; i < invp->assembly_count; i++) {

	    if (temp[i]->on_hand < ( (double) temp[i]->capacity / 2) || 
			    temp[i]->on_hand == 0) {

	        printf("%-11s %8d %7d*\n", temp[i]->id, temp[i]->capacity,
			       	temp[i]->on_hand);
	    } else {

	        printf("%-11s %8d %7d\n", temp[i]->id, temp[i]->capacity,
			       	temp[i]->on_hand);
	    }
	}

	free(temp);

    } else {

	if (id[0] != 'A') {
	    fprintf(stderr, "!!! %s: assembly ID must start with 'A'\n", id);
	    return;
	}

	//make sure list is not null
	if (invp->assembly_list == NULL) {
	    return;
	}

        //only print with respect to ID
	printf("Assembly ID:  %s\n", id);
        
	assembly_t * temp = invp->assembly_list;
	int found = 0; //false
	while (temp != NULL) {
	    if (strcmp(temp->id, id) == 0) {
                found = 1;
		break;
	    }
	    temp = temp->next;
	}

        if (found == 0) {
	    fprintf(stderr, "!!! %s: assembly ID not in inventory\n", id);
	    return;
	}

	printf("bin capacity: %d\n", temp->capacity);
	printf("on hand:      %d\n", temp->on_hand);

	//now print assembly parts (items)  list for this ID only
	printf("Parts list:\n");
	printf("-----------\n");
	
	if (temp->items == NULL) {
	    printf("NO PARTS\n");
	    return;
	}

	printf("Part ID     quantity\n");
	printf("=========== ========\n");

	item_t ** item = to_item_array(temp->items->item_count, 
			temp->items->item_list);
	qsort(item, temp->items->item_count, sizeof(item[0]), item_compare);

	for (int i = 0; i < temp->items->item_count; i++) {
	    printf("%-11s %8d\n", item[i]->id, item[i]->quantity);
	}
	free(item);
    }
}


//free's the needed parts structs generated by the
//fulfill order and restock requests
//@param parts: a pointer to an items_needed struct to be freeded
void free_needed_parts(items_needed_t * parts) {

    item_t * head = parts->item_list;
    item_t * temp;
    while (head != NULL) {
    
        temp = head;
	head = head->next;
	free(temp);
    
    }

    free(parts);

}


//manufactures the assemblies required to complete a customer order
//@param items: an items_needed_t struct to fulfill the order
void fulfill_order(items_needed_t * items) {
	
    //create list of parts needed to manufacture assemblies
    items_needed_t * parts = malloc(sizeof(items_needed_t));
    parts->item_count = 0;
    parts->item_list = NULL;

    //item list created by tokenize function
    item_t * item = items->item_list;

    while (item != NULL) {
        get(invp, item->id, item->quantity, parts);
	item = item->next;
    }

    //print needed parts and free malloced space
    free_needed_parts(items);
    print_parts_needed(parts);
    free_needed_parts(parts);
}

//tokenizes the fulfill order request and error checks
//@param str: the request typed in by the user from stdin
void tokenize_fulfill_order(char * str) {

    items_needed_t * items_needed = malloc(sizeof(items_needed));
    items_needed->item_list = NULL;
    items_needed->item_count = 0;

    //chip away the request name
    strtok(str, " ");

    char * item_id = strtok(NULL, " ");
    char * item_quantity = strtok(NULL, " ");
 
    while(1) {

	if (item_id == NULL || item_quantity == NULL) {
	    break;
	}
    
        if (item_id[0] != 'A') {
	    fprintf(stderr, "!!! %s: assembly ID is not in the inventory " 
			    "-- order canceled\n", item_id);
	    free_needed_parts(items_needed);
	    return;
	}

        if (!id_in_assembly(item_id)) {
	    fprintf(stderr, "!!! %s: assembly ID is not in the inventory "
			    "-- order canceled\n", item_id);
	    free_needed_parts(items_needed);
	    return;
	}

        if (strtol(item_quantity, NULL, 10) <= 0) {
	    fprintf(stderr, "!!! %ld: illegal order quantity for ID %s " 
			    "-- order canceled\n", 
			    strtol(item_quantity, NULL, 10), item_id);
	    free_needed_parts(items_needed);
	    return;
	}

	int dup = 0;

	item_t * temp = items_needed->item_list;
	while (temp != NULL) {
	    if(strcmp(temp->id, item_id) == 0) {
	        temp->quantity += strtol(item_quantity, NULL, 10);
		dup = 1;
		break;
	    }
	    temp = temp->next;
	}

	if (dup) {
	    item_id = strtok(NULL, " ");
	    item_quantity = strtok(NULL, " ");
	    continue;
	}

	if (items_needed->item_list == NULL) {
	    items_needed->item_list = create_item(item_id, item_quantity);
	    item_id = strtok(NULL, " ");
	    item_quantity = strtok(NULL, " ");
	    items_needed->item_count += 1;
	    continue;
	} 
	
        temp = items_needed->item_list;
	while (temp->next != NULL) {
	    temp = temp->next;
	}

	temp->next = create_item(item_id, item_quantity);
       	items_needed->item_count += 1;

	item_id = strtok(NULL, " ");
	item_quantity = strtok(NULL, " ");

    }

    fulfill_order(items_needed);

}

//restocks the entire inventory if given no argument
//or a specific assembly if supplied an argument
//@param str: the id of an assembly
void restock(char * str) {

    items_needed_t * parts = malloc(sizeof(items_needed_t));
    parts->item_list = NULL;
    parts->item_count = 0;

    strtok(str, " ");
    char * id = strtok(NULL, " ");

   
    //optional ID was not supplied
    if (id == NULL) {

        assembly_t * temp = invp->assembly_list;
        
        while (temp != NULL) {
	    if (temp->on_hand < (double) (temp->capacity) / 2 ) {
		int n = temp->capacity - temp->on_hand;
	        printf(">>> restocking assembly %s with %d items\n", temp->id,
			       	n);
	        make(invp, temp->id, n, parts);
	        temp->on_hand += n;	
	    }
	    temp = temp->next;
	}
       
    } else {

        //check to make sure assembly is in inventory
        if (!id_in_assembly(id)) {
            fprintf(stderr, "!!! %s: assembly ID is not in the inventory\n", 
			    id);
	    free(parts->item_list);
	    free(parts);
	    return;
        }
 
        assembly_t * temp = invp->assembly_list;
	while (temp != NULL) {
	    if (strcmp(temp->id, id) == 0) {
	        if (temp->on_hand < (double) (temp->capacity) / 2) {
		    int n = temp->capacity - temp->on_hand;
		    printf(">>> restocking assembly %s with %d items\n", 
				    temp->id, n);
		    make(invp, temp->id, n, parts);
		    temp->on_hand += n;
		    break; //safe to break as no duplicate ID's are allowed
		}
	    }
	}
    }

    print_parts_needed(parts);
    free_needed_parts(parts);

}

//cleans out the quantity of an assembly
//@param invp: inventory pointer
//@param str: the id of the assembly to clean out
void empty(inventory_t * invp, char * str) {

    strtok(str, " ");
    char * id = strtok(NULL, " ");

    if (id[0] != 'A') {
        fprintf(stderr, "!!! %s: ID not an assembly\n", id);
	return;
    }

    if (!id_in_assembly(id)) {
        fprintf(stderr, "!!! %s: assembly ID is not in the inventory\n", id);
	return;
    }
    
    assembly_t * temp = invp->assembly_list;
    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0) {
	    temp->on_hand = 0;
	    return;
	}
	temp = temp->next;
    }
}

//Clear wipes the invntory
//struct clean of all structs freeing their
//memory and resetting the program
//back to its start state
void clear() {

    part_t * part_head = invp->part_list;
    part_t * part_temp;

    while (part_head != NULL) {
   
        part_temp = part_head;
	part_head = part_head->next;
	free(part_temp);
   
    }

    invp->part_list = NULL;
    invp->part_count = 0;

    assembly_t * assembly_head = invp->assembly_list;
    assembly_t * assembly_temp;

    while (assembly_head != NULL) {

        assembly_temp = assembly_head;
	assembly_head = assembly_head->next;

        items_needed_t * items_needed = assembly_temp->items;
	item_t * item_head = items_needed->item_list;
	item_t * item_temp;

	while(item_head != NULL) {
	
	    item_temp = item_head;
	    item_head = item_head->next;
	    free(item_temp);

	}

        free(items_needed);
        free(assembly_temp);
    
    }

    invp->assembly_list = NULL;
    invp->assembly_count = 0;

}

//gets the required parts and items neccessary to maufacture an assembly
//this is a recursive call inline with the make function
//@param invp: inventory pointer
//@param id: the id to get parts for
//@param n: the number of parts to get
//@param parts: the parts needed linked list to be printed after completion
void get(inventory_t * invp, char * id, int n, items_needed_t * parts) {

    if (n <= 0) {
        return; //nothing to get
    }

    //check to makesure ID is in assembly
    if (!id_in_assembly(id)) {
        return; //assembly id not found, return
    }

    assembly_t * temp = get_assembly(id);

    //temp points to our assembly id object
    if (temp->on_hand >= n) {
        temp->on_hand = temp->on_hand - n;
    } else {
        make(invp, id, n - temp->on_hand, parts);
	temp->on_hand = 0;
    }
}

//make does the manufacturing after getting the parts from the get function
//@param invp: poiner to inventory struct
//@param id: the id of the assembly to make
//@param n: the number to manufacture
//@param parts: the linked list of parts needed to be printed out post call.
void make(inventory_t * invp, char * id, int n, items_needed_t * parts) {

    if (n <= 0) {
        return; //nothing to make
    }

    //check to makesure ID is in assembly
    if (!id_in_assembly(id)) {
        return; //assembly id not found, return
    }

    assembly_t * temp = get_assembly(id);
 
    //we now have the assembly object
    printf(">>> make %d units of assembly %s\n", n, id);

    if (temp->items->item_list == NULL) {
    	return;
    }

    item_t * temp_item = temp->items->item_list;

    while (temp_item != NULL) {

        //check if item is Assembly or Part
        if (temp_item->id[0] == 'P') {
	    add_item(parts, temp_item->id, n * temp_item->quantity);
	} else {
	    get(invp, temp_item->id, n * temp_item->quantity, parts);
	}
        temp_item = temp_item->next;	
    }
}

//prints the help menu displaying all possible user commands
//and there required arguments and optional arguments.
//The optional arguments are surrounded by an [].
void help() {

    printf("Requests:\n");
    printf("%4s%s\n", "", "addPart");
    printf("%4s%s\n", "", "addAssembly ID capacity [x1 n1 [x2 n2 ...]]");
    printf("%4s%s\n", "", "fulfillOrder [x1 n1 [x2 n2 ...]]");
    printf("%4s%s\n", "", "stock ID n");
    printf("%4s%s\n", "", "restock [ID]");
    printf("%4s%s\n", "", "empty ID");
    printf("%4s%s\n", "", "inventory [ID]");
    printf("%4s%s\n", "", "parts");
    printf("%4s%s\n", "", "help");
    printf("%4s%s\n", "", "clear");
    printf("%4s%s\n", "", "quit");


}

//Start of Sorting Methods

//converts part linked list into an array
//@param count: number of parts
//@param part_list: the linked list of parts to be converted
//@return an array of parts to be sorted
part_t ** to_part_array(int count, part_t * part_list) {

    part_t ** parts = malloc(sizeof(part_t) * count);
    
    part_t * temp = part_list;
    int i = 0;
    while (temp != NULL) {
        parts[i] = temp;
	temp = temp->next;
	i++;
    }
    return parts;
}

//converts the invp assembly linked list into an array
//@oaram count: length of linked list
//@param assembly_list: the list to be converted
//@return an array of assembly structs
assembly_t ** to_assembly_array(int count, assembly_t * assembly_list) {

    assembly_t ** assemblys = malloc(sizeof(assembly_t) * count);
    assembly_t * temp = assembly_list;

    int i = 0;
    while (temp != NULL) {
        assemblys[i] = temp;
	temp = temp->next;
	i++;
    }
    return assemblys;
}

//converts items linked list to an array
//@param count: length of items linked list
//@param item_list: the linked list to be converted
//@return an array of item_t structs to be sorted
item_t ** to_item_array(int count, item_t * item_list) {

    item_t ** items = malloc(sizeof(item_t) * count);

    item_t * temp = item_list;
    int i = 0;
    while (temp != NULL) {
        items[i] = temp;
	temp = temp->next;
	i++;
    }
    return items;
}

//compare function for parts
//@param p1: pointer to part one
//@param p2: pointer to part two
//@return 1: p1 > p2, -1 p1 < p2, 0 if p1 == p2
int part_compare(const void * p1, const void * p2) {

    part_t ** part_one = (part_t **) p1;
    part_t ** part_two = (part_t **) p2;

    return strcmp((*part_one)->id, (*part_two)->id);

}


//compare function for assembly
//@param p1: pointer to assembly one
//@param p2: pointer to assembly two
//@return 1: a1 > a2, -1 a1 < a2, 0 if a1 == a2
int assembly_compare(const void * a1, const void * a2) {

    assembly_t ** as_one = (assembly_t **) a1;
    assembly_t ** as_two = (assembly_t **) a2;

    return strcmp((*as_one)->id, (*as_two)->id);
}


//compare function for items
//@param p1: pointer to item one
//@param p2: pointer to item two
//@return 1: i1 > i2, -1 i1 < i2, 0 if i1 == i2
int item_compare(const void * i1, const void *i2) {

    item_t ** item_one = (item_t **) i1;
    item_t ** item_two = (item_t **) i2;

    return strcmp((*item_one)->id, (*item_two)->id);
}


//the main method is responsible for determing
//if the program ought to read from stdin
//or a file. Once this has been determined the main function
//constantly reads, trims, strips, and parses
//the request entered by the user or read from each line
//and passes it onto the correct request function.
//@param argc: number of arguments
//@param argv: an array of charater pointers to arguments in string form
//@return an int representing exit status.
//EXUT_FAILURE is returned when file cannot be opened (just one example)
int main(int argc, char * argv[]) {

    if (argc > 2) {
        fprintf(stderr, "usage: ./inventory [file]\n");
	return EXIT_FAILURE;
    }

    //Allocate space for inventory
    invp = malloc(sizeof(inventory_t));
    invp->part_list = NULL;
    invp->part_count = 0;
    invp->assembly_list = NULL;
    invp->assembly_count = 0;

    FILE * fp;

    //Read from file
    if (argc == 2) {
        fp = fopen(argv[1], "r+");
	if (fp == NULL) {
	    fprintf(stderr, "%s: No such file or directory\n", argv[1]);
	    return EXIT_FAILURE;
	}
    } else {

        //Read from stdin
	fp = stdin;
    }

    char * buf = (char*) malloc( sizeof( char) * MAX_LINE);
    size_t n;
    ssize_t  num;

    //read from file
    while ( ( num = getline( &buf, &n, fp) ) != -1 ) {

        //trim the line here
	buf = trim(buf);

	//check to see if buf is a comment
	if (buf[0] == '#'){
	    continue;
	}

        //remove comments after buf
	remove_comments(buf);

	if (strlen(buf) != 0) {
	   	
	    //check if string is quit
	    if (strcmp(buf, "quit") == 0) {
		printf("+ quit\n");
	        break;
	    }
	} else {
	    continue;
	}

	char * token = NULL;
	char * new_buf = NULL;
	char * ret = strchr(buf, ' ');

	//check if we need to tokenize buf
	if ( ret  == NULL) {
	    token = buf;    
	} else {

	    //make a copy of buf or else strtok will modify it permanently
	    new_buf = malloc( strlen(buf) * sizeof(char) + 1 );
	    new_buf = strncpy(new_buf, buf, strlen(buf));
	    token = strtok(new_buf, " ");
	}

	buf = trim(buf);

	//use token to determine what 
	//type of inventory request we are reading	
        if (strcmp(token, "addPart") == 0) {
	    printf("+ %s\n", buf);
	    tokenize_addPart(buf);    		
	} else if (strcmp(token, "addAssembly") == 0) {
            printf("+ %s\n", buf);
            tokenize_addAssembly(buf);	
	} else if (strcmp(token, "fulfillOrder") == 0) {
	    printf("+ %s\n", buf);	
	    tokenize_fulfill_order(buf);
	} else if (strcmp(token, "stock") == 0) {
	    printf("+ %s\n", buf);
	    stock(buf);
	} else if (strcmp(token, "restock") == 0) {
	    printf("+ %s\n", buf);
	    restock(buf);
	} else if (strcmp(token, "empty") == 0) {
	    printf("+ %s\n", buf);
	    empty(invp, buf);
	} else if (strcmp(token, "inventory") == 0) {
	    printf("+ %s\n", buf);
	    inventory(buf);
	} else if (strcmp(token, "parts") == 0) {
            printf("+ %s\n", buf);
	    parts();
	} else if (strcmp(token, "help") == 0) {
	    printf("+ %s\n", buf);
	    help();
	} else if (strcmp(token, "clear") == 0) {
	    printf("+ %s\n", buf);
	    clear();
	} else {
	    printf("+ %s\n", buf);
	    fprintf(stderr, "!!! %s: unknown command\n", buf);
	}
	free(new_buf);
    }

    //free the buffer
    free(buf);

    //free the inventory
    clear();
    free(invp);

    //close the file
    fclose(fp);

    return EXIT_SUCCESS;
    
}
