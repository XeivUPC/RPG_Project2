#pragma once
class IInitializable {
public:
	virtual bool Init() = 0;
	virtual bool Start() = 0;
	virtual ~IInitializable() {}
};