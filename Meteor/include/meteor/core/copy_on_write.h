#pragma once

namespace mtr
{
	template<typename T>
	class CopyOnWrite
	{
	public:
		using SharedPtr = std::shared_ptr<T>;

		CopyOnWrite() = default;
		CopyOnWrite(SharedPtr&& ptr);
		CopyOnWrite(const CopyOnWrite& other) = default;

		CopyOnWrite& operator=(const CopyOnWrite& other) = default;

		T& get_mut();

		const T& operator*() const;
		const T* operator->() const;
		operator bool() const;

	private:
		SharedPtr m_ptr;
	};

#pragma region Implementation
	template<typename T>
	CopyOnWrite<T>::CopyOnWrite(SharedPtr&& ptr) :
		m_ptr{ std::move(ptr) }
	{}

	template<typename T>
	T& CopyOnWrite<T>::get_mut()
	{
		if (m_ptr.use_count() > 1)
		{
			m_ptr = std::make_shared<T>(*m_ptr);
		}

		return *m_ptr;
	}

	template<typename T>
	const T& CopyOnWrite<T>::operator*() const
	{
		return *m_ptr;
	}

	template<typename T>
	const T* CopyOnWrite<T>::operator->() const
	{
		return m_ptr.get();
	}

	template<typename T>
	CopyOnWrite<T>::operator bool() const
	{
		return m_ptr;
	}
#pragma endregion
}
