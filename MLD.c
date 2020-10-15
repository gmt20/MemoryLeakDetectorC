#include	<stdio.h>
#include	<malloc.h>
#include	<string.h>
#include	"MLD.h"

#undef		malloc
#undef		calloc
#undef 		free


//ptrs to the global linked list of memory nodes

static Linked_List * ptr_start = NULL;
static Linked_List * ptr_next =  NULL;
//change the malloc to xmalloc

void * xmalloc (unsigned int size, const char * file, unsigned int line)
{
	void * ptr = malloc (size);
	if (ptr != NULL) 
	{
		add_node(ptr, size, file, line);
	}
	return ptr;
}

//change the calloc to xcalloc
void * xcalloc (unsigned int elements, unsigned int size, const char * file, unsigned int line)
{
	unsigned total_size;
	void * ptr = calloc(elements , size);
	if(ptr != NULL)
	{
		total_size = elements * size;
		add_node (ptr, total_size, file, line);
	}
	return ptr;
}


//change free to xfree
void xfree(void * mem_ref)
{
	remove_node(mem_ref);
	free(mem_ref);
}

// add memory node to the linked list of memory nodes

void add_node (void * ptr, unsigned int size,  const char * file, unsigned int line)
{
	Linked_List_Node mem_node;

	//create a memory node
	memset( &mem_node, 0, sizeof ( mem_node ) );
	mem_node.address 	= ptr;
	mem_node.size = size;
	strncpy(mem_node.file_name, file, 256);
	mem_node.line = line;
	
	/* add the above node to the list */
	add(mem_node);
}

// helper function to add the memory node to the linked list

void add(Linked_List_Node mem_node)
{

	Linked_List * list = NULL;
	list = (Linked_List *) malloc (sizeof(Linked_List));
	list->node.address = mem_node.address;
	list->node.size = mem_node.size;
	strcpy(list->node.file_name, mem_node.file_name); 
	list->node.line = mem_node.line;
	list->next = NULL;

	if (ptr_start == NULL)	
	{
		ptr_start = list;
		ptr_next = ptr_start;
	}
	else {
		ptr_next->next = list;
		ptr_next = ptr_next->next;				
	}

}
//remove node
void remove_node (void * mem_ref)
{
	unsigned short index;
	Linked_List  * list_head = ptr_start;

	/* check if allocate memory is in our list */
	for(index = 0; list_head != NULL; ++index, list_head = list_head->next)
	{
		if ( list_head->node.address == mem_ref )
		{
			erase ( index );
			break;
		}
	}
}

// erase called by remove node

void erase(unsigned pos)
{

	unsigned index = 0;
	Linked_List * list_head, * temp;
	
	if(pos == 0)
	{
		Linked_List * temp = ptr_start;
		ptr_start = ptr_start->next;
		free(temp);
	}
	else 
	{
		for(index = 0, list_head = ptr_start; index < pos; 
			list_head = list_head->next, ++index)
		{
			if(pos == index + 1)
			{
				temp = list_head->next;
				list_head->next =  temp->next;
				free(temp);
				break;
			}
		}
	}
}



/*
 * writes all info of the unallocated memory into a file
 */
void report_leak(void)
{
	unsigned short index;
	Linked_List * list_head;

	FILE * fp_write = fopen (OUTPUT_FILE, "wt");
	char info[1024];

	if(fp_write != NULL)
	{
		sprintf(info, "%s\n", "Memory Leak Summary");
		fwrite(info, (strlen(info) + 1) , 1, fp_write);
		sprintf(info, "%s\n", "-----------------------------------");	
		fwrite(info, (strlen(info) + 1) , 1, fp_write);
		
		for(list_head = ptr_start; list_head != NULL; list_head = list_head->next)
		{
			sprintf(info, "address : %d\n", list_head->node.address);
			fwrite(info, (strlen(info) + 1) , 1, fp_write);
			sprintf(info, "size    : %d bytes\n", list_head->node.size);			
			fwrite(info, (strlen(info) + 1) , 1, fp_write);
			sprintf(info, "file    : %s\n", list_head->node.file_name);
			fwrite(info, (strlen(info) + 1) , 1, fp_write);
			sprintf(info, "line    : %d\n", list_head->node.line);
			fwrite(info, (strlen(info) + 1) , 1, fp_write);
			sprintf(info, "%s\n", "-----------------------------------");	
			fwrite(info, (strlen(info) + 1) , 1, fp_write);
		}
	}	
	clear();
}


//clear called by report leak

void clear()
{
	Linked_List * temp = ptr_start;
	Linked_List * list_head = ptr_start;

	while(list_head != NULL) 
	{
		list_head = list_head->next;
		free(temp);
		temp = list_head;
	}
}
