/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * EE kernel glue and utility routines.
 */

#include "kernel.h"

#if defined(F_DIntr) || defined(DOXYGEN)
int DIntr()
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;
	res = eie != 0;

	if (!eie)
		return 0;

	asm (".p2align 3");
	do {
		asm volatile ("di");
		asm volatile ("sync.p");
		asm volatile ("mfc0\t%0, $12" : "=r" (eie));
		eie &= 0x10000;
	} while (eie);

	return res;
}
#endif

#if defined(F_EIntr) || defined(DOXYGEN)
int EIntr()
{
	int eie;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;
	asm volatile ("ei");

	return eie != 0;
}
#endif

#if defined(F_EnableIntc) || defined(DOXYGEN)
int EnableIntc(int intc)
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _EnableIntc(intc);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#if defined(F_DisableIntc) || defined(DOXYGEN)
int DisableIntc(int intc)
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _DisableIntc(intc);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#if defined(F_EnableDmac) || defined(DOXYGEN)
int EnableDmac(int dmac)
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _EnableDmac(dmac);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#if defined(F_DisableDmac) || defined(DOXYGEN)
int DisableDmac(int dmac)
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _DisableDmac(dmac);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#if defined(F_SetAlarm) || defined(DOXYGEN)
int SetAlarm(u16 time, void (*callback)(s32 alarm_id, u16 time, void *common), void *common)
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _SetAlarm(time, callback, common);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#if defined(F_ReleaseAlarm) || defined(DOXYGEN)
int ReleaseAlarm(int alarm_id)
{
	int eie, res;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _ReleaseAlarm(alarm_id);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#if defined(F_iEnableIntc) || defined(DOXYGEN)
int iEnableIntc(int intc)
{
	int res = _iEnableIntc(intc);
	EE_SYNC();

	return res;
}
#endif

#if defined(F_iDisableIntc) || defined(DOXYGEN)
int iDisableIntc(int intc)
{
	int res = _iDisableIntc(intc);
	EE_SYNC();

	return res;
}
#endif

#if defined(F_iEnableDmac) || defined(DOXYGEN)
int iEnableDmac(int dmac)
{
	int res = _iEnableDmac(dmac);
	EE_SYNC();

	return res;
}
#endif

#if defined(F_iDisableDmac) || defined(DOXYGEN)
int iDisableDmac(int dmac)
{
	int res = _iDisableDmac(dmac);
	EE_SYNC();

	return res;
}
#endif

#if defined(F_iSetAlarm) || defined(DOXYGEN)
int iSetAlarm(u16 time, void (*callback)(s32 alarm_id, u16 time, void *common), void *common)
{
	int res = _iSetAlarm(time, callback, common);
	EE_SYNC();

	return res;
}
#endif

#if defined(F_iReleaseAlarm) || defined(DOXYGEN)
int iReleaseAlarm(int alarm_id)
{
	int res = _iReleaseAlarm(alarm_id);
	EE_SYNC();

	return res;
}
#endif

#if defined(F_SyncDCache) || defined(DOXYGEN)
void SyncDCache(void *start, void *end)
{
	int eie;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	_SyncDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));

	if (eie)
		EI();
}
#endif

#if defined(F_iSyncDCache) || defined(DOXYGEN)
void iSyncDCache(void *start, void *end)
{
	_SyncDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));
}
#endif

#if defined(F_InvalidDCache) || defined(DOXYGEN)
void InvalidDCache(void *start, void *end)
{
	int eie;

	asm volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	_InvalidDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));

	if (eie)
		EI();
}
#endif

#if defined(F_iInvalidDCache) || defined(DOXYGEN)
void iInvalidDCache(void *start, void *end)
{
	_InvalidDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));
}
#endif
