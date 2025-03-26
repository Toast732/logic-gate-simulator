#include "vector"
#include "string"
#include "memory"

#ifndef OBJECT_STORAGE_HPP
#define OBJECT_STORAGE_HPP

template <typename ObjectType> class ObjectStorage {
	private:
		inline static std::vector<ObjectType> s_objects;
	public:
		static void Add(const std::string& name, const ObjectType object) {
			s_objects.push_back(object);
		}
		static std::vector<ObjectType> GetAll(const std::string& name) {
			return s_objects;
		}
};
#endif