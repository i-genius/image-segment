/*
* @File: anigaussm.h
* @Author: tangxuan
*
* @Created on January 8, 2017, 1:01 AM
*/

#ifndef __ANIGAUSSM_H__
#define __ANIGAUSSM_H__

#ifndef PI
#define PI 3.14159265
#endif


/* define the input buffer type, e.g. "float" */
#define SRCTYPE float

/* define the output buffer type, should be at least "float" */
#define DSTTYPE float

/* the function prototypes */
void anigauss(SRCTYPE *input, DSTTYPE *output, int sizex, int sizey, DSTTYPE sigmav, DSTTYPE sigmau, DSTTYPE phi, int orderv, int orderu);

void YvVfilterCoef(DSTTYPE sigma, DSTTYPE *filter);

void TriggsM(DSTTYPE *filter, DSTTYPE *M);

#endif /* ANIGAUSSM_H */

