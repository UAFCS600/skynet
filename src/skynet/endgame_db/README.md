## Endgame Database
 This section is the code for the cake checkers endgame database (http://www.fierz.ch/download.php). The original
 C code was written for visual C on windows and has been modified to be hopefully cross-platform.

## Files
 Database files:
 * db_.cpr contains database slices for this database level.
 * db_.idx contains database indexing information for this database level.
 
 Access code:
 * dblookup.h contains macro definitions for main c code.
 * dblookup.c contains the main c database access code that has been modified to work in gcc.
 
 Custom headers:
 * bit_lookup.h defines a platform independent version of the _BitScan functions for finding MSB and LSB in 64 bit unsigned integers.
 * virtual_addr.h defines platform independent virtual address space functions.

## Working
 * Code compiles.
 * Database files are being located and 'possibly' being loaded into memory.
 * MSB and LSB implementations working correctly.

## Issues
 * All queries resulting outside of database, unclear if this is an issue with db_init or with dblookup functions.
 * Many more I am sure...
