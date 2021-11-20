#pragma once

#define BITS_PER_INT (sizeof(int) * 8)

inline int GetBitForBitnum(int bitNum)
{
	static int bitsForBitnum[] =
	{
		(1 << 0),
		(1 << 1),
		(1 << 2),
		(1 << 3),
		(1 << 4),
		(1 << 5),
		(1 << 6),
		(1 << 7),
		(1 << 8),
		(1 << 9),
		(1 << 10),
		(1 << 11),
		(1 << 12),
		(1 << 13),
		(1 << 14),
		(1 << 15),
		(1 << 16),
		(1 << 17),
		(1 << 18),
		(1 << 19),
		(1 << 20),
		(1 << 21),
		(1 << 22),
		(1 << 23),
		(1 << 24),
		(1 << 25),
		(1 << 26),
		(1 << 27),
		(1 << 28),
		(1 << 29),
		(1 << 30),
		(1 << 31),
	};

	return bitsForBitnum[(bitNum) & (BITS_PER_INT - 1)];
}


typedef enum
{
	BITBUFERROR_VALUE_OUT_OF_RANGE = 0,		// Tried to write a value with too few bits.
	BITBUFERROR_BUFFER_OVERRUN,				// Was about to overrun a buffer.

	BITBUFERROR_NUM_ERRORS
} BitBufErrorType;

static void CallErrorHandler(int errorType, const char* pDebugName)
{
	assert(false && errorType && pDebugName);
}

//-----------------------------------------------------------------------------
// Helpers.
//-----------------------------------------------------------------------------

inline int BitByte(int bits)
{
	// return PAD_NUMBER( bits, 8 ) >> 3;
	return (bits + 7) >> 3;
}

//-----------------------------------------------------------------------------
// namespaced helpers
//-----------------------------------------------------------------------------
namespace bitbuf
{
	// ZigZag Transform:  Encodes signed integers so that they can be
	// effectively used with varint encoding.
	//
	// varint operates on unsigned integers, encoding smaller numbers into
	// fewer bytes.  If you try to use it on a signed integer, it will treat
	// this number as a very large unsigned integer, which means that even
	// small signed numbers like -1 will take the maximum number of bytes
	// (10) to encode.  ZigZagEncode() maps signed integers to unsigned
	// in such a way that those with a small absolute value will have smaller
	// encoded values, making them appropriate for encoding using varint.
	//
	//       int32 ->     uint32
	// -------------------------
	//           0 ->          0
	//          -1 ->          1
	//           1 ->          2
	//          -2 ->          3
	//         ... ->        ...
	//  2147483647 -> 4294967294
	// -2147483648 -> 4294967295
	//
	//        >> encode >>
	//        << decode <<

	inline uint32_t ZigZagEncode32(int32_t n)
	{
		// Note:  the right-shift must be arithmetic
		return(n << 1) ^ (n >> 31);
	}

	inline int32_t ZigZagDecode32(uint32_t n)
	{
		return(n >> 1) ^ -static_cast<int32_t>(n & 1);
	}

	inline uint64_t ZigZagEncode64(int64_t n)
	{
		// Note:  the right-shift must be arithmetic
		return(n << 1) ^ (n >> 63);
	}

	inline int64_t ZigZagDecode64(uint64_t n)
	{
		return(n >> 1) ^ -static_cast<int64_t>(n & 1);
	}

	const int kMaxVarintBytes = 10;
	const int kMaxVarint32Bytes = 5;
}

//-----------------------------------------------------------------------------
enum EBitCoordType
{
	kCW_None,
	kCW_LowPrecision,
	kCW_Integral
};

class bf_read;

// Precalculated bit masks for WriteUBitLong. Using these tables instead of 
// doing the calculations gives a 33% speedup in WriteUBitLong.
uint32_t g_BitWriteMasks[32][33];

// (1 << i) - 1
uint32_t g_ExtraMasks[32];

class CBitWriteMasksInit
{
public:
	CBitWriteMasksInit()
	{
		for (unsigned int startbit = 0; startbit < 32; startbit++)
		{
			for (unsigned int nBitsLeft = 0; nBitsLeft < 33; nBitsLeft++)
			{
				unsigned int endbit = startbit + nBitsLeft;
				g_BitWriteMasks[startbit][nBitsLeft] = GetBitForBitnum(startbit) - 1;
				if (endbit < 32)
					g_BitWriteMasks[startbit][nBitsLeft] |= ~(GetBitForBitnum(endbit) - 1);
			}
		}

		for (unsigned int maskBit = 0; maskBit < 32; maskBit++)
			g_ExtraMasks[maskBit] = GetBitForBitnum(maskBit) - 1;
	}
};
CBitWriteMasksInit g_BitWriteMasksInit;

// ----------------------------------------------------------------------------
// Used for serialization
//-----------------------------------------------------------------------------

class bf_write
{
public:
	bf_write();

	// nMaxBits can be used as the number of bits in the buffer. 
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_write(void* pData, int nBytes, int nMaxBits = -1);
	bf_write(const char* pDebugName, void* pData, int nBytes, int nMaxBits = -1);

	// Start writing to the specified buffer.
	// nMaxBits can be used as the number of bits in the buffer. 
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	void			StartWriting(void* pData, int nBytes, int iStartBit = 0, int nMaxBits = -1);

	// Restart buffer writing.
	void			Reset();

	// Get the base pointer.
	unsigned char* GetBasePointer() { return m_pData; }

	// Enable or disable assertion on overflow. 99% of the time, it's a bug that we need to catch,
	// but there may be the occasional buffer that is allowed to overflow gracefully.
	void			SetAssertOnOverflow(bool bAssert);

	// This can be set to assign a name that gets output if the buffer overflows.
	const char*		GetDebugName();
	void			SetDebugName(const char* pDebugName);


	// Seek to a specific position.

	void			SeekToBit(int bitPos);


	// Bit functions.

	void			WriteOneBit(int nValue);
	void			WriteOneBitNoCheck(int nValue);
	void			WriteOneBitAt(int iBit, int nValue);

	// Write signed or unsigned. Range is only checked in debug.
	void			WriteUBitLong(unsigned int data, int numbits, bool bCheckRange = true);
	void			WriteSBitLong(int data, int numbits);

	// Tell it whether or not the data is unsigned. If it's signed,
	// cast to unsigned before passing in (it will cast back inside).
	void			WriteBitLong(unsigned int data, int numbits, bool bSigned);

	// Write a list of bits in.
	bool			WriteBits(const void* pIn, int nBits);

	// writes an unsigned integer with variable bit length
	void			WriteUBitVar(unsigned int data);

	// writes a varint encoded integer
	void			WriteVarInt32(uint32_t data);
	void			WriteVarInt64(uint64_t data);
	void			WriteSignedVarInt32(int32_t data);
	void			WriteSignedVarInt64(int64_t data);
	int				ByteSizeVarInt32(uint32_t data);
	int				ByteSizeVarInt64(uint64_t data);
	int				ByteSizeSignedVarInt32(int32_t data);
	int				ByteSizeSignedVarInt64(int64_t data);

	// Copy the bits straight out of pIn. This seeks pIn forward by nBits.
	// Returns an error if this buffer or the read buffer overflows.
	bool			WriteBitsFromBuffer(class bf_read* pIn, int nBits);

	void			WriteBitAngle(float fAngle, int numbits);
	void			WriteBitCoord(const float f);
	void			WriteBitCoordMP(const float f, EBitCoordType coordType);
	void 			WriteBitCellCoord(const float f, int bits, EBitCoordType coordType);
	void			WriteBitFloat(float val);
	void			WriteBitVec3Coord(const Vector& fa);
	void			WriteBitNormal(float f);
	void			WriteBitVec3Normal(const Vector& fa);
	void			WriteBitAngles(const QAngle& fa);


	// Byte functions.

	void			WriteChar(int val);
	void			WriteByte(unsigned int val);
	void			WriteShort(int val);
	void			WriteWord(unsigned int val);
	void			WriteLong(int32_t val);
	void			WriteLongLong(int64_t val);
	void			WriteFloat(float val);
	bool			WriteBytes(const void* pBuf, int nBytes);

	// Returns false if it overflows the buffer.
	bool			WriteString(const char* pStr);
	bool			WriteString(const wchar_t* pStr);


	// Status.

	// How many bytes are filled in?
	int				GetNumBytesWritten() const;	
	int				GetNumBitsWritten() const;
	int				GetMaxNumBits() const;
	int				GetNumBitsLeft() const;
	int				GetNumBytesLeft() const;
	unsigned char* GetData();
	const unsigned char* GetData() const;

	// Has the buffer overflowed?
	bool			CheckForOverflow(int nBits);
	inline bool		IsOverflowed() const { return m_bOverflow; }

	inline void		SetOverflowFlag();

	// The current buffer.
	unsigned char*	m_pData;
	int				m_nDataBytes;
	int				m_nDataBits;

	// Where we are in the buffer.
	int				m_iCurBit;


	// Errors?
	bool			m_bOverflow;

	bool			m_bAssertOnOverflow;
	const char*		m_pDebugName;
};


//-----------------------------------------------------------------------------
// Inlined methods
//-----------------------------------------------------------------------------

// How many bytes are filled in?
inline int bf_write::GetNumBytesWritten() const
{
	return BitByte(m_iCurBit);
}

inline int bf_write::GetNumBitsWritten() const
{
	return m_iCurBit;
}

inline int bf_write::GetMaxNumBits() const
{
	return m_nDataBits;
}

inline int bf_write::GetNumBitsLeft() const
{
	return m_nDataBits - m_iCurBit;
}

inline int bf_write::GetNumBytesLeft() const
{
	return GetNumBitsLeft() >> 3;
}

inline unsigned char* bf_write::GetData()
{
	return m_pData;
}

inline const unsigned char* bf_write::GetData() const
{
	return m_pData;
}

inline bool bf_write::CheckForOverflow(int nBits)
{
	if (m_iCurBit + nBits > m_nDataBits)
	{
		SetOverflowFlag();
		CallErrorHandler(BITBUFERROR_BUFFER_OVERRUN, GetDebugName());
	}

	return m_bOverflow;
}

inline void bf_write::SetOverflowFlag()
{
	if (m_bAssertOnOverflow)
	{
		assert(false);
	}

	m_bOverflow = true;
}

inline void bf_write::WriteOneBitNoCheck(int nValue)
{
	if (nValue)
		m_pData[m_iCurBit >> 3] |= (1 << (m_iCurBit & 7));
	else
		m_pData[m_iCurBit >> 3] &= ~(1 << (m_iCurBit & 7));

	++m_iCurBit;
}

inline void bf_write::WriteOneBit(int nValue)
{
	if (!CheckForOverflow(1))
		WriteOneBitNoCheck(nValue);
}

inline void	bf_write::WriteOneBitAt(int iBit, int nValue)
{
	if (iBit + 1 > m_nDataBits)
	{
		SetOverflowFlag();
		CallErrorHandler(BITBUFERROR_BUFFER_OVERRUN, GetDebugName());
		return;
	}

	if (nValue)
		m_pData[iBit >> 3] |= (1 << (iBit & 7));
	else
		m_pData[iBit >> 3] &= ~(1 << (iBit & 7));
}

inline void bf_write::WriteUBitLong(unsigned int curData, int numbits, bool bCheckRange)
{
#ifdef _DEBUG
	// Make sure it doesn't overflow.
	if (bCheckRange && numbits < 32)
	{
		if (curData >= (uint32_t)(1 << numbits))
		{
			CallErrorHandler(BITBUFERROR_VALUE_OUT_OF_RANGE, GetDebugName());
		}
	}
	assert(numbits >= 0 && numbits <= 32);
#endif

	extern uint32_t g_BitWriteMasks[32][33];

	// Bounds checking..
	if ((m_iCurBit + numbits) > m_nDataBits)
	{
		m_iCurBit = m_nDataBits;
		SetOverflowFlag();
		CallErrorHandler(BITBUFERROR_BUFFER_OVERRUN, GetDebugName());
		return;
	}

	int nBitsLeft = numbits;
	int iCurBit = m_iCurBit;

	// Mask in a dword.
	unsigned int iDWord = iCurBit >> 5;
	assert((iDWord * 4 + sizeof(int32_t)) <= (unsigned int)m_nDataBytes);

	uint32_t iCurBitMasked = iCurBit & 31;

	uint32_t dword = LoadLittleDWord((uint32_t*)m_pData, iDWord);

	dword &= g_BitWriteMasks[iCurBitMasked][nBitsLeft];
	dword |= curData << iCurBitMasked;

	// write to stream (lsb to msb ) properly
	StoreLittleDWord((uint32_t*)m_pData, iDWord, dword);

	// Did it span a dword?
	int nBitsWritten = 32 - iCurBitMasked;
	if (nBitsWritten < nBitsLeft)
	{
		nBitsLeft -= nBitsWritten;
		curData >>= nBitsWritten;

		// read from stream (lsb to msb) properly 
		dword = LoadLittleDWord((uint32_t*)m_pData, iDWord + 1);

		dword &= g_BitWriteMasks[0][nBitsLeft];
		dword |= curData;

		// write to stream (lsb to msb) properly 
		StoreLittleDWord((uint32_t*)m_pData, iDWord + 1, dword);
	}

	m_iCurBit += numbits;
}


bf_write::bf_write()
{
	m_pData = NULL;
	m_nDataBytes = 0;
	m_nDataBits = -1; // set to -1 so we generate overflow on any operation
	m_iCurBit = 0;
	m_bOverflow = false;
	m_bAssertOnOverflow = true;
	m_pDebugName = NULL;
}

bf_write::bf_write(const char* pDebugName, void* pData, int nBytes, int nBits)
{
	m_bAssertOnOverflow = true;
	m_pDebugName = pDebugName;
	StartWriting(pData, nBytes, 0, nBits);
}

bf_write::bf_write(void* pData, int nBytes, int nBits)
{
	m_bAssertOnOverflow = true;
	m_pDebugName = NULL;
	StartWriting(pData, nBytes, 0, nBits);
}

void bf_write::StartWriting(void* pData, int nBytes, int iStartBit, int nBits)
{
	// Make sure it's dword aligned and padded.
	assert((nBytes % 4) == 0);
	assert(((uintptr_t)pData & 3) == 0);

	// The writing code will overrun the end of the buffer if it isn't dword aligned, so truncate to force alignment
	nBytes &= ~3;

	m_pData = (unsigned char*)pData;
	m_nDataBytes = nBytes;

	if (nBits == -1)
	{
		m_nDataBits = nBytes << 3;
	}
	else
	{
		assert(nBits <= nBytes * 8);
		m_nDataBits = nBits;
	}

	m_iCurBit = iStartBit;
	m_bOverflow = false;
}

void bf_write::Reset()
{
	m_iCurBit = 0;
	m_bOverflow = false;
}


void bf_write::SetAssertOnOverflow(bool bAssert)
{
	m_bAssertOnOverflow = bAssert;
}


const char* bf_write::GetDebugName()
{
	return m_pDebugName;
}


void bf_write::SetDebugName(const char* pDebugName)
{
	m_pDebugName = pDebugName;
}


void bf_write::SeekToBit(int bitPos)
{
	m_iCurBit = bitPos;
}


// Sign bit comes first
void bf_write::WriteSBitLong(int data, int numbits)
{
	// Do we have a valid # of bits to encode with?
	assert(numbits >= 1);

	// Note: it does this wierdness here so it's bit-compatible with regular integer data in the buffer.
	// (Some old code writes direct integers right into the buffer).
	if (data < 0)
	{
#ifdef _DEBUG
		if (numbits < 32)
		{
			// Make sure it doesn't overflow.

			if (data < 0)
			{
				assert(data >= -(GetBitForBitnum(numbits - 1)));
			}
			else
			{
				assert(data < (GetBitForBitnum(numbits - 1)));
			}
		}
#endif

		WriteUBitLong((unsigned int)(0x80000000 + data), numbits - 1, false);
		WriteOneBit(1);
	}
	else
	{
		WriteUBitLong((unsigned int)data, numbits - 1);
		WriteOneBit(0);
	}
}

#if _WIN32
inline unsigned int BitCountNeededToEncode(unsigned int data)
{
#if defined(_X360)
	return (32 - CountLeadingZeros(data + 1)) - 1;
#else
	unsigned long firstBit;
	_BitScanReverse(&firstBit, data + 1);
	return firstBit;
#endif
}
#endif	// _WIN32

// writes an unsigned integer with variable bit length
void bf_write::WriteUBitVar(unsigned int n)
{
	if (n < 16)
		WriteUBitLong(n, 6);
	else
		if (n < 256)
			WriteUBitLong((n & 15) | 16 | ((n & (128 | 64 | 32 | 16)) << 2), 10);
		else
			if (n < 4096)
				WriteUBitLong((n & 15) | 32 | ((n & (2048 | 1024 | 512 | 256 | 128 | 64 | 32 | 16)) << 2), 14);
			else
			{
				WriteUBitLong((n & 15) | 48, 6);
				WriteUBitLong((n >> 4), 32 - 4);
			}
}

void bf_write::WriteVarInt32(uint32_t data)
{
	// Check if align and we have room, slow path if not
	if ((m_iCurBit & 7) == 0 && (m_iCurBit + bitbuf::kMaxVarint32Bytes * 8) <= m_nDataBits)
	{
		uint8_t* target = ((uint8_t*)m_pData) + (m_iCurBit >> 3);

		target[0] = static_cast<uint8_t>(data | 0x80);
		if (data >= (1 << 7))
		{
			target[1] = static_cast<uint8_t>((data >> 7) | 0x80);
			if (data >= (1 << 14))
			{
				target[2] = static_cast<uint8_t>((data >> 14) | 0x80);
				if (data >= (1 << 21))
				{
					target[3] = static_cast<uint8_t>((data >> 21) | 0x80);
					if (data >= (1 << 28))
					{
						target[4] = static_cast<uint8_t>(data >> 28);
						m_iCurBit += 5 * 8;
						return;
					}
					else
					{
						target[3] &= 0x7F;
						m_iCurBit += 4 * 8;
						return;
					}
				}
				else
				{
					target[2] &= 0x7F;
					m_iCurBit += 3 * 8;
					return;
				}
			}
			else
			{
				target[1] &= 0x7F;
				m_iCurBit += 2 * 8;
				return;
			}
		}
		else
		{
			target[0] &= 0x7F;
			m_iCurBit += 1 * 8;
			return;
		}
	}
	else // Slow path
	{
		while (data > 0x7F)
		{
			WriteUBitLong((data & 0x7F) | 0x80, 8);
			data >>= 7;
		}
		WriteUBitLong(data & 0x7F, 8);
	}
}

void bf_write::WriteVarInt64(uint64_t data)
{
	// Check if align and we have room, slow path if not
	if ((m_iCurBit & 7) == 0 && (m_iCurBit + bitbuf::kMaxVarintBytes * 8) <= m_nDataBits)
	{
		uint8_t* target = ((uint8_t*)m_pData) + (m_iCurBit >> 3);

		// Splitting into 32-bit pieces gives better performance on 32-bit
		// processors.
		uint32_t part0 = static_cast<uint32_t>(data);
		uint32_t part1 = static_cast<uint32_t>(data >> 28);
		uint32_t part2 = static_cast<uint32_t>(data >> 56);

		int size;

		// Here we can't really optimize for small numbers, since the data is
		// split into three parts.  Cheking for numbers < 128, for instance,
		// would require three comparisons, since you'd have to make sure part1
		// and part2 are zero.  However, if the caller is using 64-bit integers,
		// it is likely that they expect the numbers to often be very large, so
		// we probably don't want to optimize for small numbers anyway.  Thus,
		// we end up with a hardcoded binary search tree...
		if (part2 == 0)
		{
			if (part1 == 0)
			{
				if (part0 < (1 << 14))
				{
					if (part0 < (1 << 7))
					{
						size = 1; goto size1;
					}
					else
					{
						size = 2; goto size2;
					}
				}
				else
				{
					if (part0 < (1 << 21))
					{
						size = 3; goto size3;
					}
					else
					{
						size = 4; goto size4;
					}
				}
			}
			else
			{
				if (part1 < (1 << 14))
				{
					if (part1 < (1 << 7))
					{
						size = 5; goto size5;
					}
					else
					{
						size = 6; goto size6;
					}
				}
				else
				{
					if (part1 < (1 << 21))
					{
						size = 7; goto size7;
					}
					else
					{
						size = 8; goto size8;
					}
				}
			}
		}
		else
		{
			if (part2 < (1 << 7))
			{
				size = 9; goto size9;
			}
			else
			{
				size = 10; goto size10;
			}
		}

	size10: target[9] = static_cast<uint8_t>((part2 >> 7) | 0x80);
	size9: target[8] = static_cast<uint8_t>((part2) | 0x80);
	size8: target[7] = static_cast<uint8_t>((part1 >> 21) | 0x80);
	size7: target[6] = static_cast<uint8_t>((part1 >> 14) | 0x80);
	size6: target[5] = static_cast<uint8_t>((part1 >> 7) | 0x80);
	size5: target[4] = static_cast<uint8_t>((part1) | 0x80);
	size4: target[3] = static_cast<uint8_t>((part0 >> 21) | 0x80);
	size3: target[2] = static_cast<uint8_t>((part0 >> 14) | 0x80);
	size2: target[1] = static_cast<uint8_t>((part0 >> 7) | 0x80);
	size1: target[0] = static_cast<uint8_t>((part0) | 0x80);

		target[size - 1] &= 0x7F;
		m_iCurBit += size * 8;
	}
	else // slow path
	{
		while (data > 0x7F)
		{
			WriteUBitLong((data & 0x7F) | 0x80, 8);
			data >>= 7;
		}
		WriteUBitLong(data & 0x7F, 8);
	}
}

void bf_write::WriteSignedVarInt32(int32_t data)
{
	WriteVarInt32(bitbuf::ZigZagEncode32(data));
}

void bf_write::WriteSignedVarInt64(int64_t data)
{
	WriteVarInt64(bitbuf::ZigZagEncode64(data));
}

int	bf_write::ByteSizeVarInt32(uint32_t data)
{
	int size = 1;
	while (data > 0x7F) {
		size++;
		data >>= 7;
	}
	return size;
}

int	bf_write::ByteSizeVarInt64(uint64_t data)
{
	int size = 1;
	while (data > 0x7F) {
		size++;
		data >>= 7;
	}
	return size;
}

int bf_write::ByteSizeSignedVarInt32(int32_t data)
{
	return ByteSizeVarInt32(bitbuf::ZigZagEncode32(data));
}

int bf_write::ByteSizeSignedVarInt64(int64_t data)
{
	return ByteSizeVarInt64(bitbuf::ZigZagEncode64(data));
}

void bf_write::WriteBitLong(unsigned int data, int numbits, bool bSigned)
{
	if (bSigned)
		WriteSBitLong((int)data, numbits);
	else
		WriteUBitLong(data, numbits);
}

bool bf_write::WriteBits(const void* pInData, int nBits)
{

	unsigned char* pIn = (unsigned char*)pInData;
	int nBitsLeft = nBits;

	// Bounds checking..
	if ((m_iCurBit + nBits) > m_nDataBits)
	{
		SetOverflowFlag();
		CallErrorHandler(BITBUFERROR_BUFFER_OVERRUN, GetDebugName());
		return false;
	}

	// Align input to dword boundary
	while (((uintptr_t)pIn & 3) != 0 && nBitsLeft >= 8)
	{
		WriteUBitLong(*pIn, 8, false);
		++pIn;
		nBitsLeft -= 8;
	}

	if (nBitsLeft >= 32)
	{
		if ((m_iCurBit & 7) == 0)
		{
			// current bit is BYTE aligned, do block copy
			int numbytes = nBitsLeft >> 3;
			int numbits = numbytes << 3;

			memcpy(m_pData + (m_iCurBit >> 3), pIn, numbytes);
			pIn += numbytes;
			nBitsLeft -= numbits;
			m_iCurBit += numbits;
		}
		else
		{
			const uint32_t iBitsRight = (m_iCurBit & 31);
			assert(iBitsRight > 0); // should not be aligned, otherwise it would have been handled before
			const uint32_t iBitsLeft = 32 - iBitsRight;
			const int iBitsChanging = 32 + iBitsLeft; // how many bits are changed during one step (not necessary written meaningful)
			unsigned int iDWord = m_iCurBit >> 5;

			uint32_t outWord = LoadLittleDWord((uint32_t*)m_pData, iDWord);
			outWord &= g_BitWriteMasks[iBitsRight][32]; // clear rest of beginning DWORD 

			// copy in DWORD blocks
			while (nBitsLeft >= iBitsChanging)
			{
				uint32_t curData = LittleDWord(*(uint32_t*)pIn);
				pIn += sizeof(uint32_t);

				outWord |= curData << iBitsRight;
				StoreLittleDWord((uint32_t*)m_pData, iDWord, outWord);

				++iDWord;
				outWord = curData >> iBitsLeft;

				nBitsLeft -= 32;
				m_iCurBit += 32;
			}

			// store last word
			StoreLittleDWord((uint32_t*)m_pData, iDWord, outWord);

			// write remaining DWORD 
			if (nBitsLeft >= 32)
			{
				WriteUBitLong(LittleDWord(*((uint32_t*)pIn)), 32, false);
				pIn += sizeof(uint32_t);
				nBitsLeft -= 32;
			}
		}
	}

	// write remaining bytes
	while (nBitsLeft >= 8)
	{
		WriteUBitLong(*pIn, 8, false);
		++pIn;
		nBitsLeft -= 8;
	}

	// write remaining bits
	if (nBitsLeft)
	{
		WriteUBitLong(*pIn, nBitsLeft, false);
	}

	return !IsOverflowed();
}

//bool bf_write::WriteBitsFromBuffer(bf_read* pIn, int nBits)
//{
//	// This could be optimized a little by
//	while (nBits > 32)
//	{
//		WriteUBitLong(pIn->ReadUBitLong(32), 32);
//		nBits -= 32;
//	}
//
//	WriteUBitLong(pIn->ReadUBitLong(nBits), nBits);
//	return !IsOverflowed() && !pIn->IsOverflowed();
//}


void bf_write::WriteBitAngle(float fAngle, int numbits)
{
	int d;
	unsigned int mask;
	unsigned int shift;

	shift = GetBitForBitnum(numbits);
	mask = shift - 1;

	d = (int)((fAngle / 360.0) * shift);
	d &= mask;

	WriteUBitLong((unsigned int)d, numbits);
}

void bf_write::WriteBitCoordMP(const float f, EBitCoordType coordType)
{
	bool bIntegral = (coordType == kCW_Integral);
	bool bLowPrecision = (coordType == kCW_LowPrecision);

	int		signbit = (f <= -(bLowPrecision ? COORD_RESOLUTION_LOWPRECISION : COORD_RESOLUTION));
	int		intval = (int)abs(f);
	int		fractval = bLowPrecision ?
		(abs((int)(f * COORD_DENOMINATOR_LOWPRECISION)) & (COORD_DENOMINATOR_LOWPRECISION - 1)) :
		(abs((int)(f * COORD_DENOMINATOR)) & (COORD_DENOMINATOR - 1));


	bool    bInBounds = intval < (1 << COORD_INTEGER_BITS_MP);

	WriteOneBit(bInBounds);

	if (bIntegral)
	{
		// Send the sign bit
		WriteOneBit(intval);
		if (intval)
		{
			WriteOneBit(signbit);
			// Send the integer if we have one.
			// Adjust the integers from [1..MAX_COORD_VALUE] to [0..MAX_COORD_VALUE-1]
			intval--;
			if (bInBounds)
			{
				WriteUBitLong((unsigned int)intval, COORD_INTEGER_BITS_MP);
			}
			else
			{
				WriteUBitLong((unsigned int)intval, COORD_INTEGER_BITS);
			}
		}
	}
	else
	{
		// Send the bit flags that indicate whether we have an integer part and/or a fraction part.
		WriteOneBit(intval);
		// Send the sign bit
		WriteOneBit(signbit);

		if (intval)
		{
			// Adjust the integers from [1..MAX_COORD_VALUE] to [0..MAX_COORD_VALUE-1]
			intval--;
			if (bInBounds)
			{
				WriteUBitLong((unsigned int)intval, COORD_INTEGER_BITS_MP);
			}
			else
			{
				WriteUBitLong((unsigned int)intval, COORD_INTEGER_BITS);
			}
		}
		WriteUBitLong((unsigned int)fractval, bLowPrecision ? COORD_FRACTIONAL_BITS_MP_LOWPRECISION : COORD_FRACTIONAL_BITS);
	}
}

void bf_write::WriteBitCellCoord(const float f, int bits, EBitCoordType coordType)
{
	assert(f >= 0.0f); // cell coords can't be negative
	assert(f < (1 << bits));

	bool bIntegral = (coordType == kCW_Integral);
	bool bLowPrecision = (coordType == kCW_LowPrecision);

	int		intval = (int)abs(f);
	int		fractval = bLowPrecision ?
		(abs((int)(f * COORD_DENOMINATOR_LOWPRECISION)) & (COORD_DENOMINATOR_LOWPRECISION - 1)) :
		(abs((int)(f * COORD_DENOMINATOR)) & (COORD_DENOMINATOR - 1));

	if (bIntegral)
	{
		WriteUBitLong((unsigned int)intval, bits);
	}
	else
	{
		WriteUBitLong((unsigned int)intval, bits);
		WriteUBitLong((unsigned int)fractval, bLowPrecision ? COORD_FRACTIONAL_BITS_MP_LOWPRECISION : COORD_FRACTIONAL_BITS);
	}
}


void bf_write::WriteBitCoord(const float f)
{
	int		signbit = (f <= -COORD_RESOLUTION);
	int		intval = (int)abs(f);
	int		fractval = abs((int)(f * COORD_DENOMINATOR)) & (COORD_DENOMINATOR - 1);


	// Send the bit flags that indicate whether we have an integer part and/or a fraction part.
	WriteOneBit(intval);
	WriteOneBit(fractval);

	if (intval || fractval)
	{
		// Send the sign bit
		WriteOneBit(signbit);

		// Send the integer if we have one.
		if (intval)
		{
			// Adjust the integers from [1..MAX_COORD_VALUE] to [0..MAX_COORD_VALUE-1]
			intval--;
			WriteUBitLong((unsigned int)intval, COORD_INTEGER_BITS);
		}

		// Send the fraction if we have one
		if (fractval)
		{
			WriteUBitLong((unsigned int)fractval, COORD_FRACTIONAL_BITS);
		}
	}
}

void bf_write::WriteBitFloat(float val)
{
	int32_t intVal;

	assert(sizeof(int32_t) == sizeof(float));
	assert(sizeof(float) == 4);

	intVal = *((int32_t*)&val);
	WriteUBitLong(intVal, 32);
}

void bf_write::WriteBitVec3Coord(const Vector& fa)
{
	int		xflag, yflag, zflag;

	xflag = (fa[0] >= COORD_RESOLUTION) || (fa[0] <= -COORD_RESOLUTION);
	yflag = (fa[1] >= COORD_RESOLUTION) || (fa[1] <= -COORD_RESOLUTION);
	zflag = (fa[2] >= COORD_RESOLUTION) || (fa[2] <= -COORD_RESOLUTION);

	WriteOneBit(xflag);
	WriteOneBit(yflag);
	WriteOneBit(zflag);

	if (xflag)
		WriteBitCoord(fa[0]);
	if (yflag)
		WriteBitCoord(fa[1]);
	if (zflag)
		WriteBitCoord(fa[2]);
}

void bf_write::WriteBitNormal(float f)
{
	int	signbit = (f <= -NORMAL_RESOLUTION);

	// NOTE: Since +/-1 are valid values for a normal, I'm going to encode that as all ones
	unsigned int fractval = abs((int)(f * NORMAL_DENOMINATOR));

	// clamp..
	if (fractval > NORMAL_DENOMINATOR)
		fractval = NORMAL_DENOMINATOR;

	// Send the sign bit
	WriteOneBit(signbit);

	// Send the fractional component
	WriteUBitLong(fractval, NORMAL_FRACTIONAL_BITS);
}

void bf_write::WriteBitVec3Normal(const Vector& fa)
{
	int		xflag, yflag;

	xflag = (fa[0] >= NORMAL_RESOLUTION) || (fa[0] <= -NORMAL_RESOLUTION);
	yflag = (fa[1] >= NORMAL_RESOLUTION) || (fa[1] <= -NORMAL_RESOLUTION);

	WriteOneBit(xflag);
	WriteOneBit(yflag);

	if (xflag)
		WriteBitNormal(fa[0]);
	if (yflag)
		WriteBitNormal(fa[1]);

	// Write z sign bit
	int	signbit = (fa[2] <= -NORMAL_RESOLUTION);
	WriteOneBit(signbit);
}

void bf_write::WriteBitAngles(const QAngle& fa)
{
	// FIXME:
	Vector tmp(fa.x, fa.y, fa.z);
	WriteBitVec3Coord(tmp);
}

void bf_write::WriteChar(int val)
{
	WriteSBitLong(val, sizeof(char) << 3);
}

void bf_write::WriteByte(unsigned int val)
{
	WriteUBitLong(val, sizeof(unsigned char) << 3);
}

void bf_write::WriteShort(int val)
{
	WriteSBitLong(val, sizeof(short) << 3);
}

void bf_write::WriteWord(unsigned int val)
{
	WriteUBitLong(val, sizeof(unsigned short) << 3);
}

void bf_write::WriteLong(int32_t val)
{
	WriteSBitLong(val, sizeof(int32_t) << 3);
}

void bf_write::WriteLongLong(int64_t val)
{
	UINT* pLongs = (UINT*)&val;

	// Insert the two DWORDS according to network endian
	const short endianIndex = 0x0100;
	BYTE* idx = (BYTE*)&endianIndex;
	WriteUBitLong(pLongs[*idx++], sizeof(int32_t) << 3);
	WriteUBitLong(pLongs[*idx], sizeof(int32_t) << 3);
}

void bf_write::WriteFloat(float val)
{
	// Pre-swap the float, since WriteBits writes raw data
	LittleFloat(&val, &val);

	WriteBits(&val, sizeof(val) << 3);
}

bool bf_write::WriteBytes(const void* pBuf, int nBytes)
{
	return WriteBits(pBuf, nBytes << 3);
}

bool bf_write::WriteString(const char* pStr)
{
	if (pStr)
	{
		do
		{
			WriteChar(*pStr);
			++pStr;
		} while (*(pStr - 1) != 0);
	}
	else
	{
		WriteChar(0);
	}

	return !IsOverflowed();
}

bool bf_write::WriteString(const wchar_t* pStr)
{
	if (pStr)
	{
		do
		{
			WriteShort(*pStr);
			++pStr;
		} while (*(pStr - 1) != 0);
	}
	else
	{
		WriteShort(0);
	}

	return !IsOverflowed();
}
