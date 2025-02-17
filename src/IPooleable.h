#pragma once
class IPooleable {
public:
	virtual void InitPoolObject() = 0;
	virtual void ResetPoolObject() = 0;
	virtual ~IPooleable() {}
};