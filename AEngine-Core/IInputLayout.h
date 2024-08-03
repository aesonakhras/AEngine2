#pragma once
#include <memory>


class ILayout {
public:
    virtual void Bind() = 0;
    virtual void UnBind() = 0;
    virtual ~ILayout() = default;
};

template <typename T>
class IInputLayout {
public:
	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	virtual ~IInputLayout() = default;
};


//explination of Type erasure: https://davekilian.com/cpp-type-erasure.html
template <typename T>
class InputLayoutWrapper : public ILayout {
public:
    InputLayoutWrapper(std::shared_ptr<IInputLayout<T>> layout) : m_Layout(layout) {}

    void Bind() override {
        m_Layout->Bind();
    }

    void UnBind() override {
        m_Layout->UnBind();
    }

private:
    std::shared_ptr<IInputLayout<T>> m_Layout;
};