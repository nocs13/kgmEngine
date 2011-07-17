#pragma once

void	kgmClearAllocatedMemory(void);
void*	kgmalloc(unsigned int);
void	kgmfree(void*);

template <class X> bool	kgmnew(X*, unsigned int count = 0);
template <class X> void	kgmdelete(X*, unsigned int count = 0);
