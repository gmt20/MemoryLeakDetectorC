#ifndef
#define Leak_Detector_Header

#define  OUTPUT_FILE			   "/home/leak_info.txt"
#define  malloc(size) 	    	   	xmalloc (size, __FILE__, __LINE__)
#define  calloc(elements, size)  	xcalloc (elements, size, __FILE__, __LINE__)
#define  free(mem_ref) 		  	 	xfree(mem_ref)

struct _Linked_List_Node
{
	void* address;
	unsigned int size;
	char file_name[256];
	unsigned int line;
};

typedef struct _Linked_List_Node Linked_List_Node;

struct _Linked_List
{
	Linked_List_Node node;
	struct _Linked_List *next;
	
};

typedef struct _Linked_List Linked_List;

// function signatures for MLD library

void add(Linked_List_Node cur_node);
void erase(unsigned position);
void clear(void);

void * xmalloc(unsigned int size, const char * file, unsigned int line);
void * xcalloc(unsigned int elements, unsigned int size, const char * file, unsigned int line);
void * xfree(void * mem_ref);

void add_node (void * mem_ref, unsigned int size,  const char * file, unsigned int line);
void remove_node (void * mem_ref);
void report_leak(void);


#endif