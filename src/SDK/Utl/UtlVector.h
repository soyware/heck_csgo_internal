#pragma once

//-----------------------------------------------------------------------------
// The CUtlVector class:
// A growable array class which doubles in size by default.
// It will always keep all elements consecutive in memory, and may move the
// elements around in memory (via a PvRealloc) when elements are inserted or
// removed. Clients should therefore refer to the elements of the vector
// by index (they should *never* maintain pointers to elements in the vector).
//-----------------------------------------------------------------------------
template< class T, class A = CUtlMemory<T> >
class CUtlVector
{
	typedef A CAllocator;
public:
	explicit CUtlVector(int growSize = 0, int initSize = 0)
		: m_Memory(growSize, initSize), m_Size(0)
	{
		ResetDbgInfo();
	}

	typedef T ElemType_t;
	typedef T* iterator;
	typedef const T* const_iterator;

	// element access
	T& operator[](int i) { return m_Memory[i]; }
	const T& operator[](int i) const { return m_Memory[i]; }
	T& Element(int i) { return m_Memory[i]; }
	const T& Element(int i) const { return m_Memory[i]; }
	T& Head() { return m_Memory[0]; }
	const T& Head() const { return m_Memory[0]; }
	T& Tail() { return m_Memory[m_Size - 1]; }
	const T& Tail() const { return m_Memory[m_Size - 1]; }

	// STL compatible member functions. These allow easier use of std::sort
	// and they are forward compatible with the C++ 11 range-based for loops.
	iterator begin() { return Base(); }
	const_iterator begin() const { return Base(); }
	iterator end() { return Base() + Count(); }
	const_iterator end() const { return Base() + Count(); }

	// Gets the base address (can change when adding elements!)
	T* Base() { return m_Memory.Base(); }
	const T* Base() const { return m_Memory.Base(); }

	// Returns the number of elements in the vector
	int Count() const { return m_Size; }

	/// are there no elements? For compatibility with lists.
	inline bool IsEmpty(void) const { return (Count() == 0); }

	// Is element index valid?
	bool IsValidIndex(int i) const { return (i >= 0) && (i < m_Size); }
	static int InvalidIndex() { return -1; }

	// Finds an element (element needs operator== defined)
	int Find(const T& src) const
	{
		for (int i = 0; i < Count(); ++i)
		{
			if (Element(i) == src)
				return i;
		}
		return -1;
	}

	bool HasElement(const T& src) const { return (Find(src) >= 0); }

	// Only use this if you really know what you're doing!
	int NumAllocated() const { return m_Memory.NumAllocated(); }	

	inline void ResetDbgInfo() { m_pElements = Base(); }

	CAllocator m_Memory;
	int m_Size;

	// For easier access to the elements through the debugger
	// it's in release builds so this can be used in libraries correctly
	T* m_pElements;

private:
	// Can't copy this unless we explicitly do it!
	// Use CCopyableUtlVector<T> to get this functionality
	CUtlVector(CUtlVector const& vec);
};