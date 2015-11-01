#pragma once

template<typename BaseManager>
struct EntityHandle
{
	friend BaseManager;
private:
	explicit EntityHandle(size_t handle = SIZE_MAX) : GUID{ handle } {}
public:
	const size_t GUID;
};