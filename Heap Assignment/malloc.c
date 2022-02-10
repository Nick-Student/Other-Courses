#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ALIGN4(s)         (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)      ((b) + 1)
#define BLOCK_HEADER(ptr)   ((struct _block *)(ptr) - 1)


static int atexit_registered = 0;
static int num_mallocs       = 0;
static int num_frees         = 0;
static int num_reuses        = 0;
static int num_grows         = 0;
static int num_splits        = 0;
static int num_coalesces     = 0;
static int num_blocks        = 0;
static int num_requested     = 0;
static int max_heap          = 0;


/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics( void )
{
  printf("\nheap management statistics\n");
  printf("mallocs:\t%d\n", num_mallocs );
  printf("frees:\t\t%d\n", num_frees );
  printf("reuses:\t\t%d\n", num_reuses );
  printf("grows:\t\t%d\n", num_grows );
  printf("splits:\t\t%d\n", num_splits );
  printf("coalesces:\t%d\n", num_coalesces );
  printf("blocks:\t\t%d\n", num_blocks );
  printf("requested:\t%d\n", num_requested );
  printf("max heap:\t%d\n", max_heap );
}

struct _block 
{
   size_t  size;         /* Size of the allocated _block of memory in bytes */
   struct _block *prev;  /* Pointer to the previous _block of allcated memory   */
   struct _block *next;  /* Pointer to the next _block of allcated memory   */
   bool   free;          /* Is this _block free?                     */
   char   padding[3];
};

struct _block *memory = NULL; // This is to help remember where we were for 

struct _block *heapList = NULL; /* Free list to track the _blocks available */

/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes 
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 * \TODO Implement Next Fit
 * \TODO Implement Best Fit
 * \TODO Implement Worst Fit
 */
struct _block *findFreeBlock(struct _block **last, size_t size) 
{
   struct _block *curr = heapList;
   struct _block *blocks = curr; // a placeholder for use of future looping
   struct _block *heapsize = curr;

#if defined FIT && FIT == 0
   /* First fit */
   while (curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }
#endif

#if defined BEST && BEST == 0
   size_t temp = 10000000; // a large number to compare to
   struct _block *best = NULL; // my temp variable for holding the smallest block leftover
   while (curr != NULL) // while we have something that needs to be allocated
   {
      *last = curr;
      if(curr->free && curr->size > size )
         if((curr->size - size) < temp)   // do it like a bubble sort, with the temp variable getting changed
         {
            temp = curr->size - size;
            best = curr;
         }
      curr = curr->next;
   }

   curr = best;

   

#endif

#if defined WORST && WORST == 0
   size_t temp = 0; // a small number to compare to
   struct _block *worst = NULL; // my temp variable for holding the smallest block leftover
   while (curr != NULL) // while we have something that needs to be allocated
   {
      *last = curr;
      if(curr->free && curr->size > size )
         if((curr->size - size) > temp)   // second verse same as the first but with flipped directions
         {
            temp = curr->size - size;
            worst = curr;
         }
      curr = curr->next;
   }

   curr = worst;


#endif

#if defined NEXT && NEXT == 0

   // Using the next location available with large enough size after an allocation looping back to the first
   if(memory)
   {
      curr = memory;
   }
   while (curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }
   memory = curr;
#endif

   // To find the number of blocks we need to count through all available blocks whether used or not.
   // Need to have this outside all of the fit test cases so it applie to them all, about an hour of confusing infinite loops for that one
   while(blocks) // Start with a placeholder variable that has the location of the original first curr
   {
      num_blocks++;
      blocks = blocks->next;
   }
   // To find the heap size we need to find the amount of blocks and add the size of each block to the heapsize
   while(heapsize)
   {
      max_heap = max_heap + heapsize->size; // heapsize starts at 0, adding the size of each block to the size will give total size
      heapsize = heapsize->next;
   }

   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically 
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) 
{
   /* Request more space from OS */
   struct _block *curr = (struct _block *)sbrk(0);
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   /* OS allocation failed */
   if (curr == (struct _block *)-1) 
   {
      return NULL;
   }

   /* Update heapList if not set */
   if (heapList == NULL) 
   {
      heapList = curr;
   }

   /* Attach new _block to prev _block */
   if (last) 
   {
      last->next = curr;
   }

   /* Update _block metadata */
   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   curr->prev = last;
   return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */
void *malloc(size_t size) 
{

   num_requested = num_requested + size;
   if( atexit_registered == 0 )
   {
      atexit_registered = 1;
      atexit( printStatistics );
   }

   /* Align to multiple of 4 */
   size = ALIGN4(size);

   /* Handle 0 size */
   if (size == 0) 
   {
      return NULL;
   }

   /* Look for free _block */
   struct _block *last = heapList;
   struct _block *next = findFreeBlock(&last, size);

   /* TODO: Split free _block if possible */
   
   if(next != NULL && next->size > size)
   {
      struct _block *old = next;  // hold the old value
      struct _block *new = next;  // set the new value to the original value
      struct _block *new2 = new;  // make another new value that is equal to the first new value
      new->size = old->size - size; // set the size of the new value to be the lower value
      new->free = false;  // flag the block as being used
      new->prev= next->prev;  // make the previous block to the new to be the same as the original
      new2->prev = new;  // set the previous for the second new value, attaching it
      new2->size = old->size - new->size; // change the size of the second new to be the difference in original and first new
      num_splits++;  // increment splits
      next = new;  // Change the current into the new to continue throuhg iterations
      
   }
   
   /* Could not find free _block, so grow heap */
   if (next == NULL) 
   {
      next = growHeap(last, size);
      num_grows++;
   }

   /* Could not find free _block or grow heap, so just return NULL */
   if (next == NULL) 
   {
      return NULL;
   }
   
   if(next->free == false)
   {
      num_reuses++;
   }
   /* Mark _block as in use */
   next->free = false;

   num_mallocs++;
   /* Return data address associated with _block */
   return BLOCK_DATA(next);
}

   // Realloc implemented
void *realloc(void *passedin, size_t size)
{
   void *ptr = malloc(size);
   memcpy(ptr, passedin,size);
   return ptr;

}
   // Calloc implemented
void *calloc(size_t newsize, size_t size)
{
   void *ptr = malloc(newsize * size);
   memset(ptr,0, newsize * size);
   return ptr;
}


/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) 
{
   num_frees++;
   if (ptr == NULL) 
   {
      return;
   }

   /* Make _block as free */
   struct _block *curr = BLOCK_HEADER(ptr);
   assert(curr->free == 0);
   curr->free = true;

   /* TODO: Coalesce free _blocks if needed */

   // For this we need to check both blocks on either side of the current block,
   // if either are free then combine the sizes of those blocks and link

      
      if(curr->next != NULL )
      {
         if (curr->next->free)
         {
            curr->size = curr->size + (curr->next->size); // add the size of the current block to the size of the next block is it is free
            curr->next = curr->next->next;
            num_coalesces++;
         }
      }
      if (curr->prev !=NULL)
      {
         if(curr->prev->free)
         {
            num_coalesces++;
            curr->prev->next = curr->next; 
            curr->prev->size = curr->size + (curr->prev->size);
         }
      }
}


/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/
