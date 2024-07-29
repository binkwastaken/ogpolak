#include "schema.h"
#include "../utils/utils.h"

bool CSchemaManager::Init()
{
	std::vector<std::string> modules = { "client.dll", "engine2.dll", "schemasystem.dll" };


	for (std::string& module : modules)
	{
		auto schema_class = g_pInterfaces->m_Interfaces.pSchemaSystem->FindTypeScopeForModule(module.c_str());

		if (!schema_class)
		{
			continue;
		}

		std::vector<SchemaDeclaredClass*> declarated;

		schema_class->GetDeclaratedClasses(declarated);

		for (auto& dec : declarated)
		{
			if (!dec || !dec->classPtr)
				continue;

			auto class_info = dec->classPtr;
			std::string class_name = dec->name;

			for (int j = 0; j < dec->classPtr->fieldsCount; j++)
			{
				auto field = &dec->classPtr->fields[j];

				if (!field || !field->name)
				{
					continue;
				}

				std::string field_name = field->name;
				std::string binary_name = dec->name;

				schema_offset_t schema_offset;
				schema_offset.class_name = class_name;
				schema_offset.property_name = field_name;
				schema_offset.offset = field->singleInheritanceOffset;

				schema_data[binary_name][field_name] = schema_offset;
			}
		}
	}

	if (schema_data.size() == 0)
	{
		g_pUtils->m_Console.Error("Failed to initialize schema manager. No schema data found");
		return false;
	}

	g_pUtils->m_Console.Success("Found", schema_data.size(), "schemas");
	g_pUtils->m_Console.Log("Schema manager initialized");
	return true;
}

std::uint32_t CSchemaManager::GetSchema(std::string class_name, std::string property_name)
{
	return schema_data[class_name][property_name].offset;
}