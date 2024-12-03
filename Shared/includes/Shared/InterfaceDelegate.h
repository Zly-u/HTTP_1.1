#pragma once

namespace DelegatesInteral
{
	template<bool IsConst, typename Object, typename RetVal, typename ...Args>
	struct MemberFunction;

	template<typename Object, typename RetVal, typename ...Args>
	struct MemberFunction<false, Object, RetVal, Args...>
	{
		using FuncionSignatureType = RetVal(Object::*)(Args...);
	};

	template<typename Object, typename RetVal, typename ...Args>
	struct MemberFunction<true, Object, RetVal, Args...>
	{
		using FuncionSignatureType = RetVal(Object::*)(Args...) const;
	};

	static void* (*Alloc)(size_t size) = [](size_t size) { return malloc(size); };
	static void  (*Free)(void* pPtr)   = [](void*  pPtr) { free(pPtr); };
}

namespace Delegates
{
	using AllocateCallback = void* (*)(size_t size);
	using FreeCallback = void(*)(void* pPtr);
	inline void SetAllocationCallbacks(AllocateCallback allocateCallback, FreeCallback freeCallback)
	{
		DelegatesInteral::Alloc = allocateCallback;
		DelegatesInteral::Free = freeCallback;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

class Interface_DelegateBase
{
public:
	Interface_DelegateBase() = default;
	virtual ~Interface_DelegateBase() noexcept = default;

	virtual const void* GetOwner() const { return nullptr; }
	virtual void Clone(void* pDestination) = 0;
};



template<typename RetVal, typename... Args>
class InterfaceDelegate : public Interface_DelegateBase
{
public:
	virtual RetVal Execute(Args&&... args) = 0;
};