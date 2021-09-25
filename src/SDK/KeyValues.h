#pragma once

#define MAKE_3_BYTES_FROM_1_AND_2( x1, x2 ) ( ((( uint16_t )x2) << 8 ) | (uint8_t)(x1) )

// callback to evaluate a $<symbol> during evaluation, return true or false
typedef bool (*GetSymbolProc_t)(const char* pKey);

class KeyValues
{
public:
	uint32_t	m_iKeyName : 24;	// keyname is a symbol defined in KeyValuesSystem
	uint32_t	m_iKeyNameCaseSensitive1 : 8;	// 1st part of case sensitive symbol defined in KeyValueSystem

	// These are needed out of the union because the API returns string pointers
	char*		m_sValue;
	wchar_t*	m_wsValue;

	// we don't delete these
	union
	{
		int		m_iValue;
		float	m_flValue;
		void*	m_pValue;
		unsigned char m_Color[4];
	};

	char		m_iDataType;
	char		m_bHasEscapeSequences; // true, if while parsing this KeyValue, Escape Sequences are used (default false)
	uint16_t	m_iKeyNameCaseSensitive2;	// 2nd part of case sensitive symbol defined in KeyValueSystem;

	KeyValues*	m_pPeer;	// pointer to next key in list
	KeyValues*	m_pSub;	// pointer to Start of a new sub key list
	KeyValues*	m_pChain;// Search here if it's not in our list

	GetSymbolProc_t	m_pExpressionGetSymbolProc;

	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_COMPILED_INT_BYTE,			// hack to collapse 1 byte ints in the compiled format
		TYPE_COMPILED_INT_0,			// hack to collapse 0 in the compiled format
		TYPE_COMPILED_INT_1,			// hack to collapse 1 in the compiled format
		TYPE_NUMTYPES,
	};

	static void* operator new(size_t size)
	{
		return I::KeyValuesSystem->AllocKeyValuesMemory(size);
	}

	static void operator delete(void* pMem)
	{
		I::KeyValuesSystem->FreeKeyValuesMemory(pMem);
	}

	explicit KeyValues(const char* setName)
	{
		typedef void(__thiscall* Fn) (void*, const char*);
		reinterpret_cast<Fn>(Signatures::KeyValuesConstructor)(this, setName);
	}

	const char* GetName() const
	{
		return I::KeyValuesSystem->GetStringForSymbol(MAKE_3_BYTES_FROM_1_AND_2(m_iKeyNameCaseSensitive1, m_iKeyNameCaseSensitive2));
	}

	// Find a keyValue, create it if it is not found.
	// Set bCreate to true to create the key if it doesn't already exist (which ensures a valid pointer will be returned)
	KeyValues* FindKey(const char* keyName, bool bCreate = false)
	{
		// return the current key if a NULL subkey is asked for
		if (!keyName || !keyName[0])
			return this;

		// look for '/' characters deliminating sub fields
		char szBuf[8192] = { 0 };
		const char* subStr = strchr(keyName, '/');
		const char* searchStr = keyName;

		// pull out the substring if it exists
		if (subStr)
		{
			int size = subStr - keyName;
			assert(0 <= size && size < sizeof(szBuf));
			strncpy_s(szBuf, sizeof(szBuf), keyName, size);
			if (strlen(keyName) > 1)
			{
				// If the key name is just '/', we don't treat is as a key with subfields, but use the '/' as a key name directly
				searchStr = szBuf;
			}
		}


		// lookup the symbol for the search string,
		// we do not need the case-sensitive symbol at this time
		// because if the key is found, then it will be found by case-insensitive lookup
		// if the key is not found and needs to be created we will pass the actual searchStr
		// and have the new KeyValues constructor get/create the case-sensitive symbol
		HKeySymbol iSearchStr = I::KeyValuesSystem->GetSymbolForString(searchStr, bCreate);
		if (iSearchStr == INVALID_KEY_SYMBOL)
		{
			// not found, couldn't possibly be in key value list
			return nullptr;
		}

		KeyValues* lastItem = nullptr;
		KeyValues* dat;
		// find the searchStr in the current peer list
		for (dat = m_pSub; dat != nullptr; dat = dat->m_pPeer)
		{
			lastItem = dat;	// record the last item looked at (for if we need to append to the end of the list)

			// symbol compare
			if (dat->m_iKeyName == (uint32_t)iSearchStr)
				break;
		}

		if (!dat && m_pChain)
			dat = m_pChain->FindKey(keyName, false);

		// make sure a key was found
		if (!dat)
		{
			if (bCreate)
			{
				// we need to create a new key
				dat = new KeyValues(searchStr);
				//			Assert(dat != NULL);

				// insert new key at end of list
				if (lastItem)
					lastItem->m_pPeer = dat;
				else
					m_pSub = dat;

				dat->m_pPeer = nullptr;

				// a key graduates to be a submsg as soon as it's m_pSub is set
				// this should be the only place m_pSub is set
				m_iDataType = TYPE_NONE;
			}
			else
				return nullptr;
		}

		// if we've still got a subStr we need to keep looking deeper in the tree
		if (subStr)
		{
			// recursively chain down through the paths in the string
			return dat->FindKey(subStr + 1, bCreate);
		}

		return dat;
	}

	void SetStringValue(char const* strValue)
	{
		// delete the old value
		I::MemAlloc->Free(m_sValue);
		// make sure we're not storing the WSTRING  - as we're converting over to STRING
		I::MemAlloc->Free(m_wsValue);
		m_wsValue = nullptr;

		if (!strValue)
		{
			// ensure a valid value
			strValue = "";
		}

		// allocate memory for the new value and copy it in
		int len = strlen(strValue);
		m_sValue = static_cast<char*>(I::MemAlloc->Alloc(len + 1));
		memcpy(m_sValue, strValue, len + 1);

		m_iDataType = TYPE_STRING;
	}

	void SetString(const char* keyName, const char* value)
	{
		FindKey(keyName, true)->SetStringValue(value);
	}

	//-----------------------------------------------------------------------------
	// Purpose: Get the integer value of a keyName. Default value is returned
	//			if the keyName can't be found.
	//-----------------------------------------------------------------------------
	int GetInt(const char* keyName, int defaultValue)
	{
		KeyValues* dat = FindKey(keyName, false);
		if (dat)
		{
			switch (dat->m_iDataType)
			{
			case TYPE_STRING:
				return atoi(dat->m_sValue);
			case TYPE_WSTRING:
				return _wtoi(dat->m_wsValue);
			case TYPE_FLOAT:
				return (int)dat->m_flValue;
			case TYPE_UINT64:
				// can't convert, since it would lose data
				assert(false);
				return 0;
			case TYPE_INT:
			case TYPE_PTR:
			default:
				return dat->m_iValue;
			};
		}
		return defaultValue;
	}

	bool LoadFromBuffer(char const* resourceName, const char* pBuffer, void* pFileSystem = 0, const char* pPathID = 0, GetSymbolProc_t pfnEvaluateSymbolProc = 0, void* unk = 0)
	{
		typedef bool(__thiscall* Fn) (void*, char const*, const char*, void*, const char*, GetSymbolProc_t, void*);
		return reinterpret_cast<Fn>(Signatures::KeyValuesLoadFromBuffer)(this, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc, unk);
	}
};