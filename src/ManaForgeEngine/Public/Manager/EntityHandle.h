#pragma once

template<typename BaseManager>
struct EntityHandle
{
	friend BaseManager;
	explicit EntityHandle(size_t handle = SIZE_MAX) : GUID{ handle } {}
	EntityHandle(const EntityHandle<BaseManager>& other) = default;

	EntityHandle<BaseManager>& operator=(const EntityHandle<BaseManager>& other) = default;

	size_t GUID;
};