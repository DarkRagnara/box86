#ifndef __DYNABLOCK_H_
#define __DYNABLOCK_H_

typedef struct x86emu_s x86emu_t;

typedef struct dynablock_s dynablock_t;

typedef struct dynablocklist_s dynablocklist_t;

uint32_t X31_hash_code(void* addr, int len);
dynablocklist_t* NewDynablockList(uintptr_t base, uintptr_t text, int textsz, int nolinker, int direct);
void FreeDynablockList(dynablocklist_t** dynablocks);
void FreeDynablock(dynablock_t* db);
void MarkDynablock(dynablock_t* db);
void MarkDynablockList(dynablocklist_t** dynablocks);
void ProtectDynablock(dynablock_t* db);
void ProtectkDynablockList(dynablocklist_t** dynablocks);
void ProtectDirectDynablock(dynablocklist_t* dynablocks, uintptr_t addr, uintptr_t size);

uintptr_t StartDynablockList(dynablocklist_t* db);
uintptr_t EndDynablockList(dynablocklist_t* db);
void FreeDirectDynablock(dynablocklist_t* dynablocks, uintptr_t addr, uintptr_t size);
void MarkDirectDynablock(dynablocklist_t* dynablocks, uintptr_t addr, uintptr_t size);

// Handling of Dynarec block (i.e. an exectable chunk of x86 translated code)
dynablock_t* DBGetBlock(x86emu_t* emu, uintptr_t addr, int create, dynablock_t** current);   // return NULL if block is not found / cannot be created. Don't create if create==0

// source is linked to dest (i.e. source->table[x] = dest->block), so add a "mark" in dest, add a "linked" info to source
void AddMark(dynablock_t* source, dynablock_t* dest, void** table);
// remove a Table mark (and also remove lined info from other dynablock, if any)
void RemoveMark(void** table);

// Create and Add an new dynablock in the list, handling direct/map
dynablock_t *AddNewDynablock(dynablocklist_t* dynablocks, uintptr_t addr, int with_marks, int* created);

#endif //__DYNABLOCK_H_