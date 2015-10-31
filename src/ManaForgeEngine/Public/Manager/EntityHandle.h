#pragma once

template<typename BaseManager>
struct EntityHandle
{
	friend BaseManager;
private:
	explicit EntityHandle(size_t handle = SIZE_MAX, size_t entityID_ = SIZE_MAX) : GUID{ handle }, entityID{ entityID_ } {}
public:
	const size_t GUID;
	const size_t entityID;
};