#pragma once

#include <pugixml.hpp>
#include <string>

class xmlManager {
protected:
	pugi::xml_document doc;
public:
	bool LoadFile(std::string filename);
	void SaveFile(std::string filename);
};

/*loading doc
pugi::xml_document doc;

pugi::xml_parse_result result = doc.load_file("tree.xml");

std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
*/

/*accessing data
for (pugi::xml_node tool = tools.child("Tool"); tool; tool = tool.next_sibling("Tool"))
{
	std::cout << "Tool " << tool.attribute("Filename").value();
	std::cout << ": AllowRemote " << tool.attribute("AllowRemote").as_bool();
	std::cout << ", Timeout " << tool.attribute("Timeout").as_int();
	std::cout << ", Description '" << tool.child_value("Description") << "'\n";
}
*/

/*modifying data

-- SETTING NODE AND ATTRIBUTE

pugi::xml_node node = doc.child("node");

// change node name
std::cout << node.set_name("notnode");
std::cout << ", new node name: " << node.name() << std::endl;

// change comment text
std::cout << doc.last_child().set_value("useless comment");
std::cout << ", new comment text: " << doc.last_child().value() << std::endl;

// we can't change value of the element or name of the comment
std::cout << node.set_value("1") << ", " << doc.last_child().set_name("2") << std::endl;

----------


pugi::xml_attribute attr = node.attribute("id");

// change attribute name/value
std::cout << attr.set_name("key") << ", " << attr.set_value("345");
std::cout << ", new attribute: " << attr.name() << "=" << attr.value() << std::endl;

// we can use numbers or booleans
attr.set_value(1.234);
std::cout << "new attribute value: " << attr.value() << std::endl;

// we can also use assignment operators for more concise code
attr = true;
std::cout << "final attribute value: " << attr.value() << std::endl;

----------

-- ADDING NODE AND ATTRIBUTE

// add node with some name
pugi::xml_node node = doc.append_child("node");

// add description node with text child
pugi::xml_node descr = node.append_child("description");
descr.append_child(pugi::node_pcdata).set_value("Simple node");

// add param node before the description
pugi::xml_node param = node.insert_child_before("param", descr);

// add attributes to param node
param.append_attribute("name") = "version";
param.append_attribute("value") = 1.1;
param.insert_attribute_after("type", param.attribute("name")) = "float";

----------

-- REMOVING NODE AND ATTRIBUTE

// remove description node with the whole subtree
pugi::xml_node node = doc.child("node");
node.remove_child("description");

// remove id attribute
pugi::xml_node param = node.child("param");
param.remove_attribute("value");

// we can also remove nodes/attributes by handles
pugi::xml_attribute id = param.attribute("name");
param.remove_attribute(id);

----------

-- SAVING

std::cout << "Saving result: " << doc.save_file("save_file_output.xml") << std::endl;


*/