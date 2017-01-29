#ifndef _UTILS_H_
#define _UTILS_H_

//###################### Macros

#define uniq(LOW,HEIGHT)	((HEIGHT << 8)|LOW)			// 2x 8Bit 	--> 16Bit
#define LOW_BYTE(x)        	(x & 0xff)					// 16Bit 	--> 8Bit
#define HIGH_BYTE(x)       	((x >> 8) & 0xff)			// 16Bit 	--> 8Bit

#define sbi(ADDRESS,BIT) 	((ADDRESS) |= (1<<(BIT)))	// set Bit
#define cbi(ADDRESS,BIT) 	((ADDRESS) &= ~(1<<(BIT)))	// clear Bit
#define	toggle(ADDRESS,BIT)	((ADDRESS) ^= (1<<(BIT)))		// Bit umschalten
#define	bis(ADDRESS,BIT)	(ADDRESS & (1<<(BIT)))		// bit is set?
#define	bic(ADDRESS,BIT)	(!(ADDRESS & (1<<(BIT))))		// bit is clear?

#endif
