#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//#include "constant.h"

#include "bmp2pbm.h"
#include "SignPad.h"
#include "BmpToPrnLogo.h"
#include "pub.h"
#include "osal.h"


/********************** Internal macros declaration ************************/
#define		WidthLIMIT	32768
#define		HeightLIMIT	32768
#define		FMODE_READ	0
#define		FMODE_WRITE	1

/********************** Internal structure declaration *********************/
typedef struct SIPImage
{			
	unsigned short	width,height;
	unsigned short	depth;			/* 1,8,16,32 */
	unsigned short	bitmap_pad;		/* 8,16,32 */
	int	bytes_per_line;
	char data[1024*12];
}SIPImage;


typedef struct BMPImage
{
	int	bmp_width;
	int	bmp_height;
	int	bmp_depth;
	int	bmp_bytesperline;

	unsigned long  bfOffBits;
}BMPImage;

typedef struct PBMFILE 
{
	int		fid;
	int		mode;
	int		cline;
	int		width;
	int		height;
	int		pixel;
	int		flbytes;
	int		linebytes;
	
	int		filetype;	/* = 0 : 1bit/pixel     */
						/*   1 : 8bit grayscale */
						/*   2 : 24bit color */
}PBMFILE;

/********************** Internal functions declaration *********************/

int bmp_Readheader(int fid, BMPImage *bmp);
long GetImageInfo(int fid, long lOffset, int iNumberOfChars);
int bmp_readline(int fid, char *buf, BMPImage *bmp);
int sip_CreateImage(unsigned short width,unsigned short height,unsigned short depth, SIPImage *imgtmp);
unsigned short sip_getlinebytes(unsigned short len,unsigned short depth,unsigned short pad);
int setsize(int width,int height,int pixel, PBMFILE *pbm);
int checksize(PBMFILE *pbm);
char *sip_getimgptr(SIPImage *image, unsigned short y);
int writeline(int line, void *buf, PBMFILE *pbm);
int seekto(int line, PBMFILE *pbm);
int pbm_open(char *path, char *type, PBMFILE *pbm);
int pbm_close(PBMFILE *pbm);
int pbm_read_header(PBMFILE *pbm);
int pbm_write_header(PBMFILE *pbm);
int sip_DestroyImage(SIPImage *image);

/********************** Internal variables declaration *********************/
static int	sgiPnm_depth;

/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

int Bmp2Pbm(char *pszAppFileName, char *pszOutFileName)
{
	char	*p;
	int	x, y;
	int	fd, iRet;
	BMPImage bmp;
	PBMFILE	 pbm;
	SIPImage image;

	//�ж��ļ���
	if (pszAppFileName == NULL)
	{
		return -1;
	}
	
	//���ļ�
	fd = open((char *)pszAppFileName, O_RDWR);
	if (fd < 0)
	{
		return fd;
	}
	
	//BMP ����
	memset(&bmp, 0, sizeof(BMPImage));
	memset(&pbm, 0, sizeof(PBMFILE));
	memset(&image, 0, sizeof(SIPImage));
	iRet = bmp_Readheader(fd, &bmp);
	if (iRet)
	{
		return -2;
	}

	sgiPnm_depth = bmp.bmp_depth;
	if ( 16 == sgiPnm_depth )  
	{
		sgiPnm_depth = 24;
	}

	iRet = sip_CreateImage((unsigned short)(bmp.bmp_width), (unsigned short)(bmp.bmp_height), (unsigned short)sgiPnm_depth, &image);
	if (iRet == 0)
	{
		return -3;
	}

	//pbm ����
	setsize(bmp.bmp_width, bmp.bmp_height, sgiPnm_depth, &pbm);

	iRet = pbm_open((char *)pszOutFileName, "w", &pbm);
	if (iRet)
	{
		return -4;
	}

	lseek(fd, bmp.bfOffBits, SEEK_SET);
	for (y = 0; y < bmp.bmp_height; y++)
	{
		p = sip_getimgptr(&image, (ushort)(bmp.bmp_height - y -1));
		if (bmp_readline(fd, p, &bmp))
		{
			return -5;
		}
	}

	for (y = 0; y < bmp.bmp_height; y++)
	{
		p = (char *)sip_getimgptr(&image, (ushort)y);
		if (bmp.bmp_depth == 1)
		{
			for ( x=0 ; x<((bmp.bmp_width +7) /8) ; x++ )  
			{
				p[x] = ~p[x];
			}
		}

		if (writeline(-1, p, &pbm))
		{
			return -6;
		}
	}

	pbm_close(&pbm);

	sip_DestroyImage(&image);

	close(fd);

	return 0;
}

int bmp_Readheader(int fid, BMPImage *bmp)
{
	int	iRet, biSize;
	unsigned int	colors;
	unsigned long	biClrUsed;

	//�ļ���λ
	iRet = lseek(fid, 0L, SEEK_SET);
	if(iRet <0) 
	{
		close(fid);
		return iRet;
	}

	//�ж�
	if (19778 != GetImageInfo(fid, 0, 2))
	{
		return -2;
	}
    
    if (0 > GetImageInfo(fid, 2, 4))
	{
		return -2;
	}

    if (0 != GetImageInfo(fid, 6, 2) )  
	{
		return -2;
	}

	if (0 != GetImageInfo(fid, 8, 2) )  
	{
		return -2;
	}

	if (0 > (bmp->bfOffBits = GetImageInfo(fid, 10, 4)))  
	{
		return -2;
	}
	
	if (0 > (biSize = GetImageInfo(fid, 14, 4) ))
	{
		return -2;
	}

	if (0 > (bmp->bmp_width = GetImageInfo(fid, 18, 4))) 
	{
		return -2;
	}

	if (0 > (bmp->bmp_height =GetImageInfo(fid, 22, 4)))
	{
		return -2;
	}
	
	if (1 != GetImageInfo(fid, 26, 2))
	{
		return -2;
	}

	//ȡֵ
	bmp->bmp_depth = GetImageInfo(fid, 28, 2);
	biClrUsed = GetImageInfo(fid, 32, 4);

	colors = 0;
	switch ( bmp->bmp_depth )
	{
		case 1:
			bmp->bmp_bytesperline = ((bmp->bmp_width + 31) /8) & ~0x03;
			colors = 2;
			break;

		case 8:
			bmp->bmp_bytesperline = (bmp->bmp_width + 3) & ~0x03;
			if (biClrUsed == 0) 
			{
				colors = 256;
			}
			else
			{
				colors = biClrUsed;
			}
			break;

		case 16:
			bmp->bmp_bytesperline = (2 * bmp->bmp_width + 3) & ~0x03;
			colors = biClrUsed;
			break;

		case 24:
			bmp->bmp_bytesperline = (3 * bmp->bmp_width + 3) & ~0x03;
			colors = 0;
			break;

		default:
			return -2;
	}
	
	// Skip color table 
    if (bmp->bfOffBits != colors*4+54)
	{
		return -2;
	}
	
	lseek(fid, bmp->bfOffBits, SEEK_SET);
	return 0;
}

long GetImageInfo(int fid, long lOffset, int iNumberOfChars)
{
	int		i, iRet;
	long	lValue;
	uchar	ucDummy;
	uchar	*pTemp;
	
	lValue  = 0L;
	ucDummy = '0';
	pTemp = &ucDummy;
	
	iRet = lseek(fid, lOffset, SEEK_SET);
	if(iRet <0) 
	{
		close(fid);
		return iRet;
	}

	for(i=1; i<=iNumberOfChars; i++)
	{
		iRet = read(fid, pTemp, 1);
		if(iRet <0) 
		{
			close(fid);
			return iRet;
		}

		// calculate value based on adding bytes
		lValue += (long)(*pTemp)<<(8*(i-1));
	}

	return(lValue);
}

int bmp_readline(int fid, char *buf, BMPImage *bmp)
{
	int	x, x3;
	unsigned char	c;
	unsigned short	d;
	unsigned char	Tempbuf[1024];

	memset(Tempbuf, 0, sizeof(Tempbuf));
	if(read(fid, Tempbuf, bmp->bmp_bytesperline) <= 0)
	{
		close(fid);
		return(-5);
	}
	
	switch ( bmp->bmp_depth )
	{
	case 8:
		memcpy((void *)buf, (void *)Tempbuf, bmp->bmp_width);
		break;
		
	case 16:
		for (x=x3=0 ; x < bmp->bmp_width ; x++, x3+=3)
		{
			d = (unsigned short)Tempbuf[2*x+1] << 8 | (unsigned short)Tempbuf[2*x];
			c = (unsigned char)(d >> 7);
			c = (c & 0xf8) | (c >> 5);
			buf[x3]   = c;		/* R */
			c = (unsigned char)(d >> 2);
			c = (c & 0xfc) | (c >> 5);
			buf[x3+1] = c;		/* G */
			c = (unsigned char)(d << 3);
			c |= c >> 5;
			buf[x3+2] = c;		/* B */
		}
		break;
		
	case 24:
		for ( x=0 ; x < (3 * bmp->bmp_width) ; x+=3 )
		{
			buf[x]   = Tempbuf[x+2];	/* R */
			buf[x+1] = Tempbuf[x+1];	/* G */
			buf[x+2] = Tempbuf[x];		/* B */
		}
		break;
		
	default:
		memcpy((void *)buf, (void *)Tempbuf, (bmp->bmp_width +7) /8);
		break;
	}
	
	return 0;
}


// Create image //
int sip_CreateImage(unsigned short width,unsigned short height,unsigned short depth, SIPImage *imgtmp)
{
	long		linesize;
	long		bufsize;

	linesize = (long)((unsigned long)sip_getlinebytes(width, depth, 8));
	bufsize = linesize * (long)((unsigned long)height);

	if (bufsize  <= 0)  return (0);

	imgtmp->width  = width;
	imgtmp->height = height;
	imgtmp->depth  = depth;
	imgtmp->bitmap_pad  = 16;
	imgtmp->bytes_per_line  = linesize;

	return (1);	
}

// Get line size in bytes 
unsigned short sip_getlinebytes(unsigned short len,unsigned short depth,unsigned short pad)
{
	unsigned short	l;

	pad /= 8;

	if ( depth == 1 )
	{
		switch (pad)
		{
		case 2:	return( ((len +15) /16) *2 );
		case 4:	return( ((len +31) /32) *4 );
		default:	return( (len +7) /8 );
		}
	}

	l = len * (depth >> 3);

	switch (pad)
	{
	case 2:	return( ((l +1) /2) *2 );
	case 4:	return( ((l +3) /4) *4 );
	default:	return(l);
	}
}

int setsize(int width,int height,int pixel, PBMFILE *pbm)
{
	pbm->width  = width;
	pbm->height = height;
	pbm->pixel  = pixel;

	switch(pixel)
	{
	case 1:
		pbm->filetype = 0;
		break;

	case 8:
		pbm->filetype = 1;
		break;

	case 24:
		pbm->filetype = 2;
		break;
	}

	if (checksize(pbm))  
	{
		return -1;
	}

	return 0;
}

int checksize(PBMFILE *pbm)
{
	switch(pbm->filetype)
	{
	case 0:
	case 1:
	case 2:
		break;

	default:
		return -1;
	}

	if ((unsigned int)pbm->width  > WidthLIMIT) 
	{
		return -1;
	}

	if ((unsigned int)pbm->height > HeightLIMIT) 
	{
		return -1;
	}

	return 0;
}

char *sip_getimgptr(SIPImage *image, unsigned short y)
{
	char	*p;

	p = image->data + (long)y * image->bytes_per_line;

	return p;
}

int writeline(int line, void *buf, PBMFILE *pbm)
{
	if (seekto(line, pbm))  
	{
		return -6;
	}

	if (pbm->mode != FMODE_WRITE)  
	{
		return -6;
	}

	if(write(pbm->fid, buf, pbm->flbytes) < 0)
	{
		close(pbm->fid);
		return(-6);
	}

	++pbm->cline;

	return 0;
}

int seekto(int line, PBMFILE *pbm)
{
	if (-1 != line)
	{
		if (line != pbm->cline)  
		{
			return(-1);
		}
	}

	return 0;
}

int pbm_open(char *path, char *type, PBMFILE *pbm)
{
	int	mode = -1;
	int	iRet;

	if ( 0 == strncmp("r", type, 2) )  mode = FMODE_READ;
	if ( 0 == strncmp("w", type, 2) )  mode = FMODE_WRITE;
	
	if ( -1 == mode )  return(-4);

	pbm->mode = mode;

	switch (mode)
	{
	    case FMODE_READ:
			iRet = open(path, O_RDWR);
			if (iRet < 0)
			{
				return iRet;
			}

			pbm->fid = iRet;

			iRet = pbm_read_header(pbm);
			if (iRet)
			{
				close(pbm->fid);
				return iRet;
			}
			break;

	    case FMODE_WRITE:
			if ( checksize(pbm) ) 
			{
				return(-4);		/* illegal data */
			}
	
			iRet = open(path, O_RDWR|O_CREAT);
			if (iRet < 0)
			{
				return iRet;
			}
			
			pbm->fid = iRet;

			iRet = pbm_write_header(pbm);
			if (iRet)
			{
				close(pbm->fid);
				return iRet;
			}
			break;
	}

	pbm->cline = 0;

	return(0);
}

int pbm_close(PBMFILE *pbm)
{
	int	iRet;

	iRet = close(pbm->fid);
	if (iRet)
	{
		return iRet;
	}

	return 0;
}

int pbm_read_header(PBMFILE *pbm)
{
	char buf[100];
	int	ft = -1;
	int	maxgray;
//	int	errflag = 0;
	int	i, iRet;

	// the author has made the assumption that number of characters in a line is 80
	// but according to Paul Bourke , the number should be less than 70
	
	iRet = lseek(pbm->fid, 0L, SEEK_CUR);
	if (iRet)
	{
		return iRet;
	}

	memset(buf, 0, sizeof(buf));
	iRet = read(pbm->fid, buf, 79);
	if (iRet < 0)
	{
		close(pbm->fid);
		return iRet;
	}

	if ( 0 == strncmp("P4", buf, 2) )  ft = 0;
	if ( 0 == strncmp("P5", buf, 2) )  ft = 1;
	if ( 0 == strncmp("P6", buf, 2) )  ft = 2;

	if ( -1 == ft )
	{  
		return(-5); 
	}

	pbm->filetype = ft;

	// There is indeed a cycle here. 
	// fp changes whenever fgets is called
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		iRet = read(pbm->fid, buf, 79);
		if (iRet < 0)
		{
			close(pbm->fid);
			return iRet;
		}

		for (i=0; i<79; i++)
		{
			if (buf[i] != '#' && buf[i] != '\n')
			{
				break;
			}
		}

//		if ( *buf != '#' && *buf != '\n' )  break;
	}
     
	// error here when using matlab generated pgms 
	if ( 2 != sscanf(buf, "%d %d", &pbm->width, &pbm->height) )
	{   
		return(-4); 
	}

	// can deal with gray images, color images and bilevel ones
	if ( pbm->filetype == 1 || pbm->filetype == 2 )
	{
		while (1)
		{
			memset(buf, 0, sizeof(buf));
			iRet = read(pbm->fid, buf, 79);
			if (iRet < 0)
			{
				close(pbm->fid);
				return iRet;
			}
			
			for (i=0; i<79; i++)
			{
				if ((buf[i] != '#') && (buf[i] != '\n'))
				{
					break;
				}
			}
		}

		if ( 1 != sscanf(buf, "%d", &maxgray) )  
		{
			return -4;
		}
		if ( maxgray > 255 )  
		{
			return -5;
		}
	}

	if ( checksize(pbm) )  
	{
		return(-5);		/* illegal data */
	}

	switch ( pbm->filetype )
	{
	    case 0:
			pbm->flbytes = pbm->linebytes = (pbm->width + 7) /8;  
			break;

	    case 1:
			pbm->flbytes = pbm->linebytes = pbm->width;  
			break;

	    case 2:
			pbm->flbytes = pbm->linebytes = 3 * pbm->width;  
			break;
	}

	return(0);
}

int pbm_write_header(PBMFILE *pbm)
{
	int		iRet;
	uchar ucTempBuf[100];

	memset(ucTempBuf, 0, sizeof(ucTempBuf));

	switch ( pbm->filetype )
	{
	    case 0:
			sprintf(ucTempBuf, "P4\n%d %d\n", pbm->width, pbm->height);
			iRet = write(pbm->fid, ucTempBuf, strlen(ucTempBuf));
			if (iRet < 0)
			{
				close(pbm->fid);
				return iRet;
			}
			pbm->flbytes = pbm->linebytes = (pbm->width + 7) /8;
			break;

	    case 1:
			sprintf(ucTempBuf, "P5\n%d %d\n255\n", pbm->width, pbm->height);
			iRet = write(pbm->fid, ucTempBuf, strlen(ucTempBuf));
			if (iRet < 0)
			{
				close(pbm->fid);
				return iRet;
			}
			pbm->flbytes = pbm->linebytes = pbm->width;
			break;

	    case 2:
			sprintf((char *)ucTempBuf, "P6\n%d %d\n255\n", pbm->width, pbm->height);
			iRet = write(pbm->fid, ucTempBuf, strlen(ucTempBuf));
			if (iRet < 0)
			{
				close(pbm->fid);
				return iRet;
			}
			pbm->flbytes = pbm->linebytes = 3 * pbm->width;
			break;
	}

	return(0);
}

// Destroy image 
int sip_DestroyImage(SIPImage *image)
{
	memset(image->data, 0, 128);
	return 0;
}
// end of file
