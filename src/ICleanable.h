#pragma once
class ICleanable {
public:
	virtual bool CleanUp() = 0;
	virtual ~ICleanable() {}
};