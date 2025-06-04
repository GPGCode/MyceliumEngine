#pragma once
#include <cstdint>

class Entity
{
	public:
		using IDType = std::uint32_t;

		Entity() : id(0) {}
		explicit Entity(IDType id) : id(id) {}

		IDType GetID() const { return id; }

		bool operator==(const Entity& other) const { return id == other.id; }
		bool operator!=(const Entity& other) const { return id != other.id; }
		operator bool() const { return id != 0; }

private:
	IDType id;

};