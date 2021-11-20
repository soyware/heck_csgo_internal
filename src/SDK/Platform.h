#pragma once

#define PLAT_LITTLE_ENDIAN 1
#define COMPILER_MSVC32 1
#define COMPILER_MSVC 1


// need macro for constant expression
#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) ) 

//-----------------------------------------------------------------------------
// Stack-based allocation related helpers
//-----------------------------------------------------------------------------
#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )
#define mallocsize( _p )		( _msize( _p ) )

#define stackalloc_aligned( _size, _align )		(void*)( ( ((uintp)alloca( ALIGN_VALUE( ( _size ) + (_align ),  ( _align ) ) )) + ( _align ) ) & ~_align )

// We should probably always just align to 16 bytes, stackalloc just causes too many problems without this behavior. Source2 does it already.
// #define stackalloc( _size )							stackalloc_aligned( _size, 16 )

#define  stackfree( _p )			0
// two-argument ( type, #elements) stackalloc
#define StackAlloc( typ, nelements ) ( ( typ * )	stackalloc_aligned( ( nelements ) * sizeof(typ), 16 ) )


//-----------------------------------------------------------------------------
// Purpose: Standard functions for handling endian-ness
//-----------------------------------------------------------------------------

//-------------------------------------
// Basic swaps
//-------------------------------------

template <typename T>
inline T QWordSwapC( T dw )
{
	// Assert sizes passed to this are already correct, otherwise
	// the cast to uint64_t * below is unsafe and may have wrong results 
	// or even crash.
	static_assert( sizeof( dw ) == sizeof(uint64_t) );

	uint64_t temp;

	temp  =   *((uint64_t*)&dw) 				         >> 56;
	temp |= ((*((uint64_t*)&dw) & 0x00FF000000000000ull) >> 40);
	temp |= ((*((uint64_t*)&dw) & 0x0000FF0000000000ull) >> 24);
	temp |= ((*((uint64_t*)&dw) & 0x000000FF00000000ull) >> 8);
	temp |= ((*((uint64_t*)&dw) & 0x00000000FF000000ull) << 8);
	temp |= ((*((uint64_t*)&dw) & 0x0000000000FF0000ull) << 24);
	temp |= ((*((uint64_t*)&dw) & 0x000000000000FF00ull) << 40);
	temp |= ((*((uint64_t*)&dw) & 0x00000000000000FFull) << 56);

	return *((T*)&temp);
}

//-------------------------------------
// Fast swaps
//-------------------------------------

#define WordSwap  WordSwapAsm
#define DWordSwap DWordSwapAsm

#pragma warning(push)
#pragma warning (disable:4035) // no return value

template <typename T>
inline T WordSwapAsm( T w )
{
	__asm
	{
		mov ax, w
		xchg al, ah
	}
}

template <typename T>
inline T DWordSwapAsm( T dw )
{
	__asm
	{
		mov eax, dw
		bswap eax
	}
}

#pragma warning(pop)

// No ASM implementation for this yet
#define QWordSwap QWordSwapC

//-------------------------------------
// The typically used methods.
//-------------------------------------

// If a swapped float passes through the fpu, the bytes may get changed.
// Prevent this by swapping floats as DWORDs.
#define SafeSwapFloat( pOut, pIn )	(*((uint*)pOut) = DWordSwap( *((uint*)pIn) ))

#define BigShort( val )				WordSwap( val )
#define BigWord( val )				WordSwap( val )
#define BigLong( val )				DWordSwap( val )
#define BigDWord( val )				DWordSwap( val )
#define BigQWord( val )				QWordSwap( val ) 
#define LittleShort( val )			( val )
#define LittleWord( val )			( val )
#define LittleLong( val )			( val )
#define LittleDWord( val )			( val )
#define LittleQWord( val )			( val )

#define SwapShort( val )			BigShort( val )
#define SwapWord( val )				BigWord( val )
#define SwapLong( val )				BigLong( val )
#define SwapDWord( val )			BigDWord( val )

// Pass floats by pointer for swapping to avoid truncation in the fpu
#define BigFloat( pOut, pIn )		SafeSwapFloat( pOut, pIn )
#define LittleFloat( pOut, pIn )	( *pOut = *pIn )
#define SwapFloat( pOut, pIn )		BigFloat( pOut, pIn )

inline uint32_t LoadLittleDWord( uint32_t *base, unsigned int dwordIndex )
{
	return LittleDWord( base[dwordIndex] );
}

inline void StoreLittleDWord( uint32_t *base, unsigned int dwordIndex, uint32_t dword )
{
	base[dwordIndex] = LittleDWord(dword);
}