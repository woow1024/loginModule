#pragma once

template <typename T> class new_singletonbase
{
public:
	static T* GetInstance() 
	{ 
		if (m_pInstance == NULL)
		{
			m_pInstance = new T;
			m_pInstance->InitInstance();
		}

		return m_pInstance; 
	}

	static bool IsInit()
	{
		return m_pInstance != NULL;
	}

	void Destroy()
	{
		delete this;
		m_pInstance = NULL;
	}
protected:
	virtual ~new_singletonbase() {}
	new_singletonbase() {}

	void InitInstance() {}

private:
	static T* m_pInstance;
};

template <typename T> T* new_singletonbase<T>::m_pInstance = NULL;

#define IMPLEMENT_NEW_SINGLETON_MODE(class_name)	\
	friend  new_singletonbase<class_name>;	\
	protected:				\
	class_name##() {}

template <typename T> class static_singletonbase
{
public:
	static T* GetInstance() 
	{ 
		static T t;

		static bool bCallInit = true;

		if (bCallInit)
		{
			bCallInit = false;
			t.InitInstance();
		}

		return &t; 
	}

protected:
	static_singletonbase() {}
	virtual ~static_singletonbase() {}

	virtual void InitInstance() {}
};

#define IMPLEMENT_STATIC_SINGLETON_MODE(class_name)	\
	friend static_singletonbase<class_name>;	\
	protected:				\
	class_name##() {}	
