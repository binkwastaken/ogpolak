#pragma once
#include "../interfaces/manager.h"
#include <unordered_map>

class CSchemaManager {
public:
    bool Init();
    template <typename T>
    T& GetOffset(const char* class_name, const char* property_name, void* ptr);

private:

    typedef struct _schema_offset_t
    {
        std::string class_name;
        std::string property_name;
        std::uint32_t offset;
    } schema_offset_t;

    std::unordered_map<std::string, std::unordered_map<std::string, schema_offset_t>> schema_data;

    std::uint32_t GetSchema(std::string class_name, std::string property_name);
};
inline CSchemaManager* g_pSchemaManager = new CSchemaManager();

template<typename T>
inline T& CSchemaManager::GetOffset(const char* class_name, const char* property_name, void* ptr)
{
        return *(T*)((uintptr_t)ptr + GetSchema(class_name, property_name));
}
