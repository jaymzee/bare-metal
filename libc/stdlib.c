#include <stdlib.h>
#include <ctype.h>

/*----------------------------------------------------------------------
 * Convert an ASCII string into an integer.
 * Arguments:
 *      str     String of ASCII digits, possibly preceded by white
 *              space. For bases greater than 10, either lower- or
 *              upper-case digits may be used.
 *
 * Results:
 *      The return value is the integer equivalent of string. If there
 *      are no decimal digits in string, then 0 is returned.
 *
 * Side effects:
 *      None.
 *--------------------------------------------------------------------*/
int
atoi(char *str)
{
    int result = 0;
    unsigned int digit;
    int sign;

    // Skip any leading blanks.
    while (isspace(*str)) {
        str++;
    }

    // Check for a sign.
    if (*str == '-') {
        sign = 1;
        str++;
    } else {
        sign = 0;
        if (*str == '+') {
            str++;
        }
    }

    for ( ; ; str++) {
        digit = *str - '0';
        if (digit > 9) {
            break;
        }
        result = (10 * result) + digit;
    }

    if (sign) {
        return -result;
    }
    return result;
}

long
atol(char *str)
{
    long result = 0;
    unsigned int digit;
    int sign; // bool

    // Skip any leading blanks.
    while (isspace(*str)) {
        str++;
    }

    // Check for a sign.
    if (*str == '-') {
        sign = 1;
        str++;
    } else {
        sign = 0;
        if (*str == '+') {
            str++;
        }
    }

    for ( ; ; str++) {
        digit = *str - '0';
        if (digit > 9) {
            break;
        }
        result = (10 * result) + digit;
    }

    if (sign) {
        return -result;
    }
    return result;
}

int rand_r(unsigned int *seedp)
{
  unsigned int next = *seedp;
  int result;

  next *= 1103515245;
  next += 12345;
  result = (unsigned int) (next / 65536) % 2048;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  *seedp = next;

  return result;
}

#define NALLOC 4096  // units to allocate at once
                     // if sizeof HEADER is 16 (the case x86_64)
                     // 4096 units is 64KB, this is how much
                     // morecore will allocate at a time

typedef struct header { // free block header
    struct header *next;  // next free block
    size_t size; // size of this free block in units
} Header;

static Header base;  // empty list to get started
static Header *freeptr = NULL;   // start of free list
static Header *morecore(size_t nu);

/*
 * allocate memory
 */
void *malloc(size_t nbytes)
{
    Header *currptr, *prevptr;
    size_t nunits;

    /* calculate the number of memory units needed to provide at least
     * nbytes of memory (plus the unit for Header)
     */
    nunits = 1 + (nbytes + sizeof(Header) - 1) / sizeof(Header);

    // case: no free list exists; we have to initialize
    if (freeptr == NULL) {
        // create degenerate free list; base points to itself and has size 0
        base.next = &base;
        base.size = 0;

        // set free list starting point to base address
        freeptr = &base;
    }
    // initialize pointers to two consecutive blocks in the free list
    prevptr = freeptr;
    currptr = prevptr->next;

    /* step through the free list looking for a block of memory large enough
     * to fit nunits of memory. If the list is traversed without finding such
     * a block, then morecore is called to request more memory from the OS.
     */
    for (; ; prevptr = currptr, currptr = currptr->next) {
        /* case: found a block of memory in free list large enough to fit
         * nunits of memory into. Partition block if necessary, remove it from
         * the free list, and return the address of the block (after moving
         * past the header).
         */
        if (currptr->size >= nunits) {
            /* case: block is exactly the right size; remove the block from
             * the free list by pointing the previous block to the next block.
             */
            if (currptr->size == nunits) {
                /* Note that his line wouldn't work as intended if we were
                 * down to only 1 block. However we would never make it here
                 * in that scenario because the block at &base has size 0 and
                 * the conditional will fail.
                 */
                prevptr->next = currptr->next;
            /* case: block is larger than needed; allocate tail end of the
             * block to the user.
             */
            } else {
                // partition off the end of the block and create new header
                currptr->size -= nunits;
                currptr += currptr->size;
                currptr->size = nunits;
                // currptr points the control structure for the block to return
            }
            /* Set global starting position to the previous pointer. Next call
             * to malloc will start either at the remaining part of the
             * partitioned block if a partition occurred or at the block after
             * the selected block if not.
             */
            freeptr = prevptr;
            /* return the location of the start of memory (move past the
             * header)
             */
            return (void *)(currptr + 1);
        }
        /* case: we've wrapped around the free list without finding a block
         * large enough. Call morecore to request more memory from the OS.
         */
        if (currptr == freeptr) {
            /* morecore returns freeptr; the reason we have to assign currptr
             * to it again is that there is a call to free inside morecore that
             * can potentially change freeptr. Thus we reassign so that we can
             * be assured the newly added block is found before
             * (currptr==freeptr) again.
             */
            if ((currptr = morecore(nunits)) == NULL) {
                return NULL;
            }
        }
    }
}

static void *program_break;

int brk(void *addr)
{
    program_break = addr;
    return 0;
}

void *sbrk(size_t nbytes)
{
    char *old_break = program_break;
    program_break = old_break + nbytes;
    return old_break;
}

/*
 * ask system for more memory
 */
static Header *morecore(size_t nunits)
{
    void *freemem;      // the address of the newly created memory
    Header *insertp;    // header pointer

    /* Obtaining memory from OS is a comparatively expensive process operation,
     * so obtain at least NALLOC blocks of memory and partition as needed.
     */
    if (nunits < NALLOC) {
        nunits = NALLOC;
    }

    /* Request that the OS increment the program's data space. sbrk changes the
     * location of the program break, which defines the end of the process's
     * data segment.
     */
    freemem = sbrk(nunits * sizeof(Header));
    // case: unable to allocate more memory; my_sbrk returns NULL on error
    if (freemem == NULL) {
        return NULL;
    }

    // construct new block
    insertp = (Header *)freemem;
    insertp->size = nunits;

    /* Insert block into the free list so that it is available for malloc.
     * Note that we add 1 to the address to point the pointer past the header.
     */
    free(insertp + 1);

    /* Returns the start of the free list; recall that freeptr has been set to
     * the block immediately proceeding the newly allocated memory (by free).
     * Thus by returning this value, the calling function can immediately find
     * the new memory by follwing the pointer to the next block.
     */
    return freeptr;
}

/*
 * free memory
 */
void free(void *ptr)
{
    Header *insertp, *currp;

    // find address of block header for the data to be inserted
    insertp = (Header *)ptr - 1;

    /* Step through the free list looking for the position in the list to place
     * the insertion block. In the typical circumstance this would be the block
     * immediately to the left of the insertion block; this checked for by
     * finding a block that is to the left of the insertion block and such that
     * the following block in the list is to the right of the insertion block.
     * However this check doesn't check for one such case, and misses another.
     * We still have to check for the cases where either the insertion block is
     * either to the left of every other block owned by malloc (the case that
     * is missed), or to the right of every block owned by malloc (the case
     * not checked for). These last two cases are what is checked for by the
     * condition inside the loop.
     */
    for (currp = freeptr; !(currp < insertp && insertp < currp->next);
         currp = currp->next) {
        /* currp >= currp->next implies that the current block is the rightmost
         * block in the free list. Then if the insertion block is to the right
         * of that block, then it is the new rightmost block; conversely if it
         * is to the left of the block that currp points to (which is the
         * current leftmost block), then the insertion block is the new
         * leftmost block. Note that this conditional handles the case where
         * we only have 1 block in the free list (this case is the reason that
         * we need >= in the first test rather than just >).
         */
        if (currp >= currp->next &&
            (currp < insertp || insertp < currp->next)) {
            break;  // freed block at start or end of arena
        }
    }

    /* Having found the correct location in the free list to place the
     * insertion block, now we have to (i) link it to the next block, and (ii)
     * link the previous block to it.  These are the tasks of the next two
     * if/else pairs.
     */

    /* case: the end of the insertion block is adjacent to the beginning of
     * another block of data owned by malloc.  Absorb the block on the right
     * into the block on the left (i.e. the previously existing block is
     * absorbed into the insertion block).
     */
    if (insertp + insertp->size == currp->next) { // join to upper nbr
        insertp->size += currp->next->size;
        insertp->next = currp->next->next;
    /* case: the insertion block is not left-adjacent to the beginning of
     * another block of data owned by malloc.  Set the insertion block member
     * to point to the next block in the list.
     */
    } else {
        insertp->next = currp->next;
    }

    /* case: the end of another block of data owned by malloc is adjacent to
     * the beginning of the insertion block.  Absorb the block on the right
     * into the block on the left (i.e. the insertion block is absorbed into
     * the preceeding block).
     */
    if (currp + currp->size == insertp) {
        currp->size += insertp->size;
        currp->next = insertp->next;
    /* case: the insertion block is not right-adjacent to the end of another
     * block of data owned by malloc.  Set the previous block in the list to
     * point to the insertion block.
     */
    } else {
        currp->next = insertp;
    }
    /* Set the free pointer list to start the block previous to the insertion
     * block.  This makes sense because calls to malloc start their search for
     * memory at the next block after freep, and the insertion block has as
     * good a chance as any of containing a reasonable amount of memory since
     * we've just added some to it.  It also coincides with calls to morecore
     * from kandr_malloc because the next search in the iteration looks at
     * exactly the right memory block.
     */
    freeptr = currp;
}
