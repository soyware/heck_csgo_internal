#pragma once

#if defined( PLAT_LITTLE_ENDIAN )
#define LZSS_ID				(('S'<<24)|('S'<<16)|('Z'<<8)|('L'))
#else
#define LZSS_ID				(('L'<<24)|('Z'<<16)|('S'<<8)|('S'))
#endif

// bind the buffer for correct identification
struct lzss_header_t
{
	unsigned int	id;
	unsigned int	actualSize;	// always little endian
};

class CUtlBuffer;

#define DEFAULT_LZSS_WINDOW_SIZE 4096

class CLZSS
{
public:
	unsigned char* Compress(unsigned char* pInput, int inputlen, unsigned int* pOutputSize);
	unsigned char* CompressNoAlloc(unsigned char* pInput, int inputlen, unsigned char* pOutput, unsigned int* pOutputSize);
	unsigned int	Uncompress(unsigned char* pInput, unsigned char* pOutput);
	//unsigned int	Uncompress( unsigned char *pInput, CUtlBuffer &buf );
	unsigned int	SafeUncompress(unsigned char* pInput, unsigned char* pOutput, unsigned int unBufSize);
	bool			IsCompressed(unsigned char* pInput);
	unsigned int	GetActualSize(unsigned char* pInput);

	// windowsize must be a power of two.
	FORCEINLINE CLZSS(int nWindowSize = DEFAULT_LZSS_WINDOW_SIZE);

private:
	// expected to be sixteen bytes
	struct lzss_node_t
	{
		unsigned char* pData;
		lzss_node_t* pPrev;
		lzss_node_t* pNext;
		char			empty[4];
	};

	struct lzss_list_t
	{
		lzss_node_t* pStart;
		lzss_node_t* pEnd;
	};

	void			BuildHash(unsigned char* pData);
	lzss_list_t* m_pHashTable;
	lzss_node_t* m_pHashTarget;
	int             m_nWindowSize;

};

FORCEINLINE CLZSS::CLZSS(int nWindowSize)
{
	m_nWindowSize = nWindowSize;
}


#define LZSS_LOOKSHIFT		4
#define LZSS_LOOKAHEAD		( 1 << LZSS_LOOKSHIFT )

//-----------------------------------------------------------------------------
// Returns true if buffer is compressed.
//-----------------------------------------------------------------------------
bool CLZSS::IsCompressed(unsigned char* pInput)
{
	lzss_header_t* pHeader = (lzss_header_t*)pInput;
	if (pHeader && pHeader->id == LZSS_ID)
	{
		return true;
	}

	// unrecognized
	return false;
}

//-----------------------------------------------------------------------------
// Returns uncompressed size of compressed input buffer. Used for allocating output
// buffer for decompression. Returns 0 if input buffer is not compressed.
//-----------------------------------------------------------------------------
unsigned int CLZSS::GetActualSize(unsigned char* pInput)
{
	lzss_header_t* pHeader = (lzss_header_t*)pInput;
	if (pHeader && pHeader->id == LZSS_ID)
	{
		return LittleLong(pHeader->actualSize);
	}

	// unrecognized
	return 0;
}

void CLZSS::BuildHash(unsigned char* pData)
{
	lzss_list_t* pList;
	lzss_node_t* pTarget;

	int targetindex = (unsigned int)(uintptr_t(pData) & 0xFFFFFFFF) & (m_nWindowSize - 1);
	pTarget = &m_pHashTarget[targetindex];
	if (pTarget->pData)
	{
		pList = &m_pHashTable[*pTarget->pData];
		if (pTarget->pPrev)
		{
			pList->pEnd = pTarget->pPrev;
			pTarget->pPrev->pNext = 0;
		}
		else
		{
			pList->pEnd = 0;
			pList->pStart = 0;
		}
	}

	pList = &m_pHashTable[*pData];
	pTarget->pData = pData;
	pTarget->pPrev = 0;
	pTarget->pNext = pList->pStart;
	if (pList->pStart)
	{
		pList->pStart->pPrev = pTarget;
	}
	else
	{
		pList->pEnd = pTarget;
	}
	pList->pStart = pTarget;
}

unsigned char* CLZSS::CompressNoAlloc(unsigned char* pInput, int inputLength, unsigned char* pOutputBuf, unsigned int* pOutputSize)
{
	if (inputLength <= sizeof(lzss_header_t) + 8)
		return NULL;

	// create the compression work buffers, small enough (~64K) for stack
	m_pHashTable = (lzss_list_t*)stackalloc(256 * sizeof(lzss_list_t));
	memset(m_pHashTable, 0, 256 * sizeof(lzss_list_t));
	m_pHashTarget = (lzss_node_t*)stackalloc(m_nWindowSize * sizeof(lzss_node_t));
	memset(m_pHashTarget, 0, m_nWindowSize * sizeof(lzss_node_t));

	// allocate the output buffer, compressed buffer is expected to be less, caller will free
	unsigned char* pStart = pOutputBuf;
	// prevent compression failure (inflation), leave enough to allow dribble eof bytes
	unsigned char* pEnd = pStart + inputLength - sizeof(lzss_header_t) - 8;

	// set the header
	lzss_header_t* pHeader = (lzss_header_t*)pStart;
	pHeader->id = LZSS_ID;
	pHeader->actualSize = LittleLong(inputLength);

	unsigned char* pOutput = pStart + sizeof(lzss_header_t);
	unsigned char* pLookAhead = pInput;
	unsigned char* pWindow = pInput;
	unsigned char* pEncodedPosition = NULL;
	unsigned char* pCmdByte = NULL;
	int putCmdByte = 0;

	while (inputLength > 0)
	{
		pWindow = pLookAhead - m_nWindowSize;
		if (pWindow < pInput)
		{
			pWindow = pInput;
		}

		if (!putCmdByte)
		{
			pCmdByte = pOutput++;
			*pCmdByte = 0;
		}
		putCmdByte = (putCmdByte + 1) & 0x07;

		int encodedLength = 0;
		int lookAheadLength = inputLength < LZSS_LOOKAHEAD ? inputLength : LZSS_LOOKAHEAD;

		lzss_node_t* pHash = m_pHashTable[pLookAhead[0]].pStart;
		while (pHash)
		{
			int matchLength = 0;
			int length = lookAheadLength;
			while (length-- && pHash->pData[matchLength] == pLookAhead[matchLength])
			{
				matchLength++;
			}
			if (matchLength > encodedLength)
			{
				encodedLength = matchLength;
				pEncodedPosition = pHash->pData;
			}
			if (matchLength == lookAheadLength)
			{
				break;
			}
			pHash = pHash->pNext;
		}

		if (encodedLength >= 3)
		{
			*pCmdByte = (*pCmdByte >> 1) | 0x80;
			*pOutput++ = ((pLookAhead - pEncodedPosition - 1) >> LZSS_LOOKSHIFT);
			*pOutput++ = ((pLookAhead - pEncodedPosition - 1) << LZSS_LOOKSHIFT) | (encodedLength - 1);
		}
		else
		{
			encodedLength = 1;
			*pCmdByte = (*pCmdByte >> 1);
			*pOutput++ = *pLookAhead;
		}

		for (int i = 0; i < encodedLength; i++)
		{
			BuildHash(pLookAhead++);
		}

		inputLength -= encodedLength;

		if (pOutput >= pEnd)
		{
			// compression is worse, abandon
			return NULL;
		}
	}

	if (inputLength != 0)
	{
		// unexpected failure
		assert(0);
		return NULL;
	}

	if (!putCmdByte)
	{
		pCmdByte = pOutput++;
		*pCmdByte = 0x01;
	}
	else
	{
		*pCmdByte = ((*pCmdByte >> 1) | 0x80) >> (7 - putCmdByte);
	}

	*pOutput++ = 0;
	*pOutput++ = 0;

	if (pOutputSize)
	{
		*pOutputSize = pOutput - pStart;
	}

	return pStart;
}

//-----------------------------------------------------------------------------
// Compress an input buffer. Caller must free output compressed buffer.
// Returns NULL if compression failed (i.e. compression yielded worse results)
//-----------------------------------------------------------------------------
unsigned char* CLZSS::Compress(unsigned char* pInput, int inputLength, unsigned int* pOutputSize)
{
	unsigned char* pStart = (unsigned char*)malloc(inputLength);
	unsigned char* pFinal = CompressNoAlloc(pInput, inputLength, pStart, pOutputSize);
	if (!pFinal)
	{
		free(pStart);
		return NULL;
	}

	return pStart;
}

/*
// BUG BUG:  This code is flaky, don't use until it's debugged!!!
unsigned int CLZSS::Uncompress( unsigned char *pInput, CUtlBuffer &buf )
{
	int cmdByte = 0;
	int getCmdByte = 0;
	unsigned int actualSize = GetActualSize( pInput );
	if ( !actualSize )
	{
		// unrecognized
		return 0;
	}
	unsigned char *pBase = ( unsigned char * )buf.Base();
	pInput += sizeof( lzss_header_t );
	while ( !buf.IsValid() )
	{
		if ( !getCmdByte )
		{
			cmdByte = *pInput++;
		}
		getCmdByte = ( getCmdByte + 1 ) & 0x07;
		if ( cmdByte & 0x01 )
		{
			int position = *pInput++ << LZSS_LOOKSHIFT;
			position |= ( *pInput >> LZSS_LOOKSHIFT );
			int count = ( *pInput++ & 0x0F ) + 1;
			if ( count == 1 )
			{
				break;
			}
			unsigned int pos = buf.TellPut();
			unsigned char *pSource = ( pBase + pos ) - position - 1;
			// BUGBUG:
			// This is failing!!!
			// buf.WriteBytes( pSource, count );
			// So have to iterate them manually
			for ( int i =0; i < count; ++i )
			{
				buf.PutUnsignedChar( *pSource++ );
			}
		}
		else
		{
			buf.PutUnsignedChar( *pInput++ );
		}
		cmdByte = cmdByte >> 1;
	}
	if ( buf.TellPut() != (int)actualSize )
	{
		// unexpected failure
		assert( 0 );
		return 0;
	}
	return buf.TellPut();
}
*/

unsigned int CLZSS::SafeUncompress(unsigned char* pInput, unsigned char* pOutput, unsigned int unBufSize)
{
	unsigned int totalBytes = 0;
	int cmdByte = 0;
	int getCmdByte = 0;

	unsigned int actualSize = GetActualSize(pInput);
	if (!actualSize)
	{
		// unrecognized
		return 0;
	}

	if (actualSize > unBufSize)
	{
		return 0;
	}

	pInput += sizeof(lzss_header_t);

	for (;; )
	{
		if (!getCmdByte)
		{
			cmdByte = *pInput++;
		}
		getCmdByte = (getCmdByte + 1) & 0x07;

		if (cmdByte & 0x01)
		{
			int position = *pInput++ << LZSS_LOOKSHIFT;
			position |= (*pInput >> LZSS_LOOKSHIFT);
			int count = (*pInput++ & 0x0F) + 1;
			if (count == 1)
			{
				break;
			}
			unsigned char* pSource = pOutput - position - 1;

			if (totalBytes + count > unBufSize)
			{
				return 0;
			}

			for (int i = 0; i < count; i++)
			{
				*pOutput++ = *pSource++;
			}
			totalBytes += count;
		}
		else
		{
			if (totalBytes + 1 > unBufSize)
				return 0;

			*pOutput++ = *pInput++;
			totalBytes++;
		}
		cmdByte = cmdByte >> 1;
	}

	if (totalBytes != actualSize)
	{
		// unexpected failure
		assert(0);
		return 0;
	}

	return totalBytes;
}

//-----------------------------------------------------------------------------
// Uncompress a buffer, Returns the uncompressed size. Caller must provide an
// adequate sized output buffer or memory corruption will occur.
//-----------------------------------------------------------------------------
unsigned int CLZSS::Uncompress(unsigned char* pInput, unsigned char* pOutput)
{
	unsigned int totalBytes = 0;
	int cmdByte = 0;
	int getCmdByte = 0;

	unsigned int actualSize = GetActualSize(pInput);
	if (!actualSize)
	{
		// unrecognized
		return 0;
	}

	pInput += sizeof(lzss_header_t);

	for (;; )
	{
		if (!getCmdByte)
		{
			cmdByte = *pInput++;
		}
		getCmdByte = (getCmdByte + 1) & 0x07;

		if (cmdByte & 0x01)
		{
			int position = *pInput++ << LZSS_LOOKSHIFT;
			position |= (*pInput >> LZSS_LOOKSHIFT);
			int count = (*pInput++ & 0x0F) + 1;
			if (count == 1)
			{
				break;
			}
			unsigned char* pSource = pOutput - position - 1;
			for (int i = 0; i < count; i++)
			{
				*pOutput++ = *pSource++;
			}
			totalBytes += count;
		}
		else
		{
			*pOutput++ = *pInput++;
			totalBytes++;
		}
		cmdByte = cmdByte >> 1;
	}

	if (totalBytes != actualSize)
	{
		// unexpected failure
		assert(0);
		return 0;
	}

	return totalBytes;
}