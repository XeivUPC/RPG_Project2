#pragma once
class IRendereable {
public:
	virtual void Render() = 0;
	virtual ~IRendereable() {}
	int renderLayer = 0;
};