#include "IO_Flow_Parameter_Set.h"
#include <string>
#include <set>
#include <cstring>
#include <algorithm>

//All serialization and deserialization functions should be replaced by a C++ reflection implementation
void IO_Flow_Parameter_Set::Serialize(Utils::XmlWriter& xmlwriter)
{
	std::string attr = "Type";
	std::string val;
	switch (Type)
	{
	case Flow_Type::SYNTHETIC:
		val = "SYNTHETIC";
		break;
	case Flow_Type::TRACE:
		val = "TRACE";
		break;
	default:
		break;
	}
	xmlwriter.Write_attribute_string(attr, val);

	attr = "Priority_Class";
	switch (Priority_Class)
	{
	case IO_Flow_Priority_Class::URGENT:
		val = "URGENT";
		break;
	case IO_Flow_Priority_Class::HIGH:
		val = "HIGH";
		break;
	case IO_Flow_Priority_Class::MEDIUM:
		val = "MEDIUM";
		break;
	case IO_Flow_Priority_Class::LOW:
		val = "LOW";
		break;
	default:
		break;
	}
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Device_Level_Data_Caching_Mode";
	switch (Device_Level_Data_Caching_Mode)
	{
	case SSD_Components::Caching_Mode::TURNED_OFF:
		val = "TURNED_OFF";
		break;
	case SSD_Components::Caching_Mode::READ_CACHE:
		val = "READ_CACHE";
		break;
	case SSD_Components::Caching_Mode::WRITE_CACHE:
		val = "WRITE_CACHE";
		break;
	case SSD_Components::Caching_Mode::WRITE_READ_CACHE:
		val = "WRITE_READ_CACHE";
		break;
	}
	xmlwriter.Write_attribute_string(attr, val);

	attr = "Channel_IDs";
	val = "";
	for (int i = 0; i < Channel_No; i++)
	{
		if (i > 0)
			val += ",";
		val += std::to_string(Channel_IDs[i]);
	}
	xmlwriter.Write_attribute_string(attr, val);

	attr = "Chip_IDs";
	val = "";
	for (int i = 0; i < Chip_No; i++)
	{
		if (i > 0)
			val += ",";
		val += std::to_string(Chip_IDs[i]);
	}
	xmlwriter.Write_attribute_string(attr, val);

	attr = "Die_IDs";
	val = "";
	for (int i = 0; i < Die_No; i++)
	{
		if (i > 0)
			val += ",";
		val += std::to_string(Die_IDs[i]);
	}
	xmlwriter.Write_attribute_string(attr, val);

	attr = "Plane_IDs";
	val = "";
	for (int i = 0; i < Plane_No; i++)
	{
		if (i > 0)
			val += ",";
		val += std::to_string(Plane_IDs[i]);
	}
	xmlwriter.Write_attribute_string(attr, val);
}

void IO_Flow_Parameter_Set::Deserialize(rapidxml::xml_node<> *node)
{
	for (auto param = node->first_node(); param; param = param->next_sibling())
	{
		if (strcmp(param->name(), "Device_Level_Data_Caching_Mode") == 0)
		{
			std::string val = param->value();
			std::transform(val.begin(), val.end(), val.begin(), ::toupper);
			if(strcmp(val.c_str(), "TURNED_OFF") == 0)
				Device_Level_Data_Caching_Mode = SSD_Components::Caching_Mode::TURNED_OFF;
			else if (strcmp(val.c_str(), "WRITE_CACHE") == 0)
				Device_Level_Data_Caching_Mode = SSD_Components::Caching_Mode::WRITE_CACHE;
			else if (strcmp(val.c_str(), "READ_CACHE") == 0)
				Device_Level_Data_Caching_Mode = SSD_Components::Caching_Mode::READ_CACHE;
			else if (strcmp(val.c_str(), "WRITE_READ_CACHE") == 0)
				Device_Level_Data_Caching_Mode = SSD_Components::Caching_Mode::WRITE_READ_CACHE;
			else PRINT_ERROR("Wrong caching mode definition for input flow")
		}
		else if (strcmp(param->name(), "Type") == 0)
		{
			std::string val = param->value();
			std::transform(val.begin(), val.end(), val.begin(), ::toupper);
			if (strcmp(val.c_str(), "SYNTHETIC") == 0)
				Type = Flow_Type::SYNTHETIC;
			else if (strcmp(val.c_str(), "TRACE") == 0)
				Type = Flow_Type::TRACE;
			else PRINT_ERROR("Wrong input flow type")
		}
		else if (strcmp(param->name(), "Priority_Class") == 0)
		{
			std::string val = param->value();
			std::transform(val.begin(), val.end(), val.begin(), ::toupper);
			if (strcmp(val.c_str(), "URGENT") == 0)
				Priority_Class = IO_Flow_Priority_Class::URGENT;
			else if (strcmp(val.c_str(), "HIGH") == 0)
				Priority_Class = IO_Flow_Priority_Class::HIGH;
			else if (strcmp(val.c_str(), "MEDIUM") == 0)
				Priority_Class = IO_Flow_Priority_Class::MEDIUM;
			else if (strcmp(val.c_str(), "LOW") == 0)
				Priority_Class = IO_Flow_Priority_Class::LOW;
			else PRINT_ERROR("Wrong priority class definition for input flow")
		}
		else if (strcmp(param->name(), "Channel_IDs") == 0)
		{
			std::set<int> ids;
			char tmp[1000], *tmp2;
			strncpy(tmp, param->value(), 1000);
			std::string id = strtok(tmp, ",");
			while (1)
			{
				std::string::size_type sz;
				ids.insert(std::stoi(id, &sz));
				tmp2 = strtok(NULL, ",");
				if (tmp2 == NULL)
					break;
				else
					id = tmp2;
			}
			Channel_No = ids.size();
			Channel_IDs = new flash_block_ID_type[Channel_No];
			int i = 0;
			for (auto it = ids.begin(); it != ids.end(); it++)
				Channel_IDs[i++] = *it;
		}
		else if (strcmp(param->name(), "Chip_IDs") == 0)
		{
			std::set<int> ids;
			char tmp[1000], *tmp2;
			strncpy(tmp, param->value(), 1000);
			std::string id = strtok(tmp, ",");
			while (1)
			{
				std::string::size_type sz;
				ids.insert(std::stoi(id, &sz));
				tmp2 = strtok(NULL, ",");
				if (tmp2 == NULL)
					break;
				else
					id = tmp2;
			}
			Chip_No = ids.size();
			Chip_IDs = new flash_block_ID_type[Chip_No];
			int i = 0;
			for (auto it = ids.begin(); it != ids.end(); it++)
				Chip_IDs[i++] = *it;
		}
		else if (strcmp(param->name(), "Die_IDs") == 0)
		{
			std::set<int> ids;
			char tmp[1000], *tmp2;
			strncpy(tmp, param->value(), 1000);
			std::string id = strtok(tmp, ",");
			while (1)
			{
				std::string::size_type sz;
				ids.insert(std::stoi(id, &sz));
				tmp2 = strtok(NULL, ",");
				if (tmp2 == NULL)
					break;
				else
					id = tmp2;
			}
			Die_No = ids.size();
			Die_IDs = new flash_block_ID_type[Die_No];
			int i = 0;
			for (auto it = ids.begin(); it != ids.end(); it++)
				Die_IDs[i++] = *it;
		}
		else if (strcmp(param->name(), "Plane_IDs") == 0)
		{
			std::set<int> ids;
			char tmp[1000], *tmp2;
			strncpy(tmp, param->value(), 1000);
			std::string id = strtok(tmp, ",");
			while (1)
			{
				std::string::size_type sz;
				ids.insert(std::stoi(id, &sz));
				tmp2 = strtok(NULL, ",");
				if (tmp2 == NULL)
					break;
				else
					id = tmp2;
			}
			Plane_No = ids.size();
			Plane_IDs = new flash_block_ID_type[Plane_No];
			int i = 0;
			for (auto it = ids.begin(); it != ids.end(); it++)
				Plane_IDs[i++] = *it;
		}
	}
}

void IO_Flow_Parameter_Set_Synthetic::Serialize(Utils::XmlWriter& xmlwriter)
{
	std::string tmp;
	tmp = "IO_Flow_Parameter_Set_Synthetic";
	xmlwriter.Write_open_tag(tmp);
	IO_Flow_Parameter_Set::Serialize(xmlwriter);

	std::string attr = "Read_Percentage";
	std::string val = std::to_string(Read_Percentage);
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Address_Distribution";
	val = std::to_string(Read_Percentage);
	switch (Address_Distribution)
	{
	case Host_Components::Address_Distribution_Type::STREAMING:
		val = "STREAMING";
		break;
	case Host_Components::Address_Distribution_Type::HOTCOLD_RANDOM:
		val = "HOTCOLD_RANDOM";
		break;
	case Host_Components::Address_Distribution_Type::UNIFORM_RANDOM:
		val = "UNIFORM_RANDOM";
		break;
	}
	xmlwriter.Write_attribute_string(attr, val);
	 

	attr = "Percentage_of_Hot_Region";
	val = std::to_string(Percentage_of_Hot_Region);
	xmlwriter.Write_attribute_string(attr, val);
	
	attr = "Request_Size_Distribution";
	switch (Request_Size_Distribution)
	{
	case Host_Components::Request_Size_Distribution_Type::FIXED:
		val = "FIXED";
		break;
	case Host_Components::Request_Size_Distribution_Type::NORMAL:
		val = "NORMAL";
		break;
	}
	xmlwriter.Write_attribute_string(attr, val);

	attr = "Average_Request_Size";
	val = std::to_string(Average_Request_Size);
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Variance_Request_Size";
	val = std::to_string(Variance_Request_Size);
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Seed";
	val = std::to_string(Seed);
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Average_No_of_Reqs_in_Queue";
	val = std::to_string(Average_No_of_Reqs_in_Queue);
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Stop_Time";
	val = std::to_string(Stop_Time);
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Total_Requests_To_Generate";
	val = std::to_string(Total_Requests_To_Generate);
	xmlwriter.Write_attribute_string(attr, val);

	xmlwriter.Write_close_tag();
}

void IO_Flow_Parameter_Set_Synthetic::Deserialize(rapidxml::xml_node<> *node)
{
	IO_Flow_Parameter_Set::Deserialize(node);

	for (auto param = node->first_node(); param; param = param->next_sibling())
	{
		if (strcmp(param->name(), "Read_Percentage") == 0)
		{
			std::string val = param->value();
			Read_Percentage = std::stoi(val);
		}
		else if (strcmp(param->name(), "Address_Distribution") == 0)
		{
			std::string val = param->value();
			std::transform(val.begin(), val.end(), val.begin(), ::toupper);
			if (strcmp(val.c_str(), "STREAMING") == 0)
				Address_Distribution = Host_Components::Address_Distribution_Type::STREAMING;
			else if (strcmp(val.c_str(), "HOTCOLD_RANDOM") == 0)
				Address_Distribution = Host_Components::Address_Distribution_Type::HOTCOLD_RANDOM;
			else if (strcmp(val.c_str(), "UNIFORM_RANDOM") == 0)
				Address_Distribution = Host_Components::Address_Distribution_Type::UNIFORM_RANDOM;
			else PRINT_ERROR("Wrong address distribution type for input synthetic flow")
		}
		else if (strcmp(param->name(), "Percentage_of_Hot_Region") == 0)
		{
			std::string val = param->value();
			Percentage_of_Hot_Region = std::stod(val);
		}
		else if (strcmp(param->name(), "Request_Size_Distribution") == 0)
		{
			std::string val = param->value();
			std::transform(val.begin(), val.end(), val.begin(), ::toupper);
			if (strcmp(val.c_str(), "FIXED") == 0)
				Request_Size_Distribution = Host_Components::Request_Size_Distribution_Type::FIXED;
			else if (strcmp(val.c_str(), "NORMAL") == 0)
				Request_Size_Distribution = Host_Components::Request_Size_Distribution_Type::NORMAL;
			else PRINT_ERROR("Wrong request size distribution type for input synthetic flow")
		}
		else if (strcmp(param->name(), "Average_Request_Size") == 0)
		{
			std::string val = param->value();
			Average_Request_Size = std::stoi(val);
		}
		else if (strcmp(param->name(), "Variance_Request_Size") == 0)
		{
			std::string val = param->value();
			Variance_Request_Size = std::stoi(val);
		}
		else if (strcmp(param->name(), "Seed") == 0)
		{
			std::string val = param->value();
			Seed = std::stoi(val);
		}
		else if (strcmp(param->name(), "Average_No_of_Reqs_in_Queue") == 0)
		{
			std::string val = param->value();
			Average_No_of_Reqs_in_Queue = std::stoi(val);
		}
		else if (strcmp(param->name(), "Stop_Time") == 0)
		{
			std::string val = param->value();
			Stop_Time = std::stoll(val);
		}
		else if (strcmp(param->name(), "Total_Requests_To_Generate") == 0)
		{
			std::string val = param->value();
			Total_Requests_To_Generate = std::stoi(val);
		}
	}
}

void IO_Flow_Parameter_Set_Trace_Based::Serialize(Utils::XmlWriter& xmlwriter)
{

	std::string tmp = "IO_Flow_Parameter_Set_Trace_Based";
	xmlwriter.Write_open_tag(tmp);
	IO_Flow_Parameter_Set::Serialize(xmlwriter);

	std::string attr = "File_Path";
	std::string val = File_Path;
	xmlwriter.Write_attribute_string(attr, val);


	attr = "Percentage_To_Be_Executed";
	val = std::to_string(Percentage_To_Be_Executed);
	xmlwriter.Write_attribute_string(attr, val);

	xmlwriter.Write_close_tag();
}

void IO_Flow_Parameter_Set_Trace_Based::Deserialize(rapidxml::xml_node<> *node)
{
	IO_Flow_Parameter_Set::Deserialize(node);

	for (auto param = node->first_node(); param; param = param->next_sibling())
	{
		if (strcmp(param->name(), "Percentage_To_Be_Executed") == 0)
		{
			std::string val = param->value();
			Percentage_To_Be_Executed = std::stod(val);
		}
		else if (strcmp(param->name(), "File_Path") == 0)
		{
			File_Path = param->value();
		}
	}
}