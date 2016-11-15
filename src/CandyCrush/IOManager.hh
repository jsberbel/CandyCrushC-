#pragma once
#include <XML/rapidxml_utils.hpp>
#include "Resource.hh"
#include "Logger.hh"
using namespace Logger;

namespace IOManager {

	void TestXML(std::string &&filename) {
		rapidxml::file<> xmlFile(RESOURCE_FILE(filename));
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		rapidxml::xml_node<> *root_node = doc.first_node("document");
		for (rapidxml::xml_node<> * node = root_node->first_node("node"); node; node = node->next_sibling()) {
			Print("Node: ", 
				  node->first_attribute("att1")->value(),
				  " - ",
				  node->first_attribute("att2")->value());
		}
	}

}