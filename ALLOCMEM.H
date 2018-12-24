// Guard Against Multiple Inclusion
// --------------------------------
   #if !defined(ALLOCMEM_H)
      #define ALLOCMEM_H


// Constants
// ---------
   #define ALLOCMEM_MODNAME "AllocMem.DLL"


// Function Declarations
// ---------------------
   PVOID EXPENTRY AllocMemAlloc      (LONG);
   VOID  EXPENTRY AllocMemFree       (PVOID);
   ULONG EXPENTRY AllocMemQuerySize  (PVOID);
   PVOID EXPENTRY AllocMemReAlloc    (PVOID, LONG);


// End of AllocMem.H
// -----------------
   #endif
