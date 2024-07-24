#pragma once


template <typename T>
class IIinputLayout {
	
	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	virtual ~IIinputLayout() {};
};