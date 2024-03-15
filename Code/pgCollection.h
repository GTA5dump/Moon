#pragma once
	template<typename T> class pgCollection
	{
	private:
		T*			m_pData;
		UINT16		m_pCount;
		UINT16		m_pSize;

	public:
		T* begin()
		{
			return m_pData;
		}

		T* end()
		{
			return (m_pData + m_pCount);
		}

		T* at(UINT16 index)
		{
			return &m_pData[index];
		}
	};

	template<typename T> class pgPtrCollection
	{
	private:
		T**			m_pData;
		UINT16		m_pCount;
		UINT16		m_pSize;

	public:
		T** begin()
		{
			return m_pData;
		}

		T** end()
		{
			return (m_pData + m_pCount);
		}

		T* at(UINT16 index)
		{
			return m_pData[index];
		}

		UINT16 count()
		{
			return m_pCount;
		}

		void set(UINT16 index, T* ptr)
		{
			m_pData[index] = ptr;
		}
	};