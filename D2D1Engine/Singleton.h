#pragma once
template<class Derived>
class Singleton
{
public:
	Singleton() {}
	~Singleton() {}

	static Derived* GetInstance()
	{
		static Derived instance;
		return &instance;
	}
};



