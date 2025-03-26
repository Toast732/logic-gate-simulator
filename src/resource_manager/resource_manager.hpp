#include "map"
#include "string"
#include "memory"
#include "raylib-cpp.hpp"

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

template <typename ResourceType> class ResourceManager {
private:
	inline static std::map<std::string, std::shared_ptr<ResourceType>> s_resources;
public:
	/*ResourceManager() {
		//this->s_resources = std::map<std::string, std::shared_ptr<ResourceType>>();
	}*/
	static void Register(const std::string& name, const std::shared_ptr<ResourceType>& resource) {
		TraceLog(LOG_INFO, "[RESOURCE MANAGER] Created Resource %s", name.c_str());
		s_resources[name] = resource;
	}
	static std::shared_ptr<ResourceType> Get(const std::string& name) {
		if (s_resources[name]) {
			return s_resources[name];
		}
		else {
			TraceLog(LOG_ERROR, "[RESOURCE MANAGER] Resource not found: %s", name.c_str());
		}
	}
	static ResourceType GetCopy(const std::string& name) {
		return *(s_resources[name].get());
	}
	static ResourceType* GetPointer(const std::string& name) {
		return s_resources[name].get();
	}
};
#endif