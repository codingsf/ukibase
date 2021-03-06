/*
 * Manager.cpp
 *
 *  Created on: Jun 4, 2012
 *      Author: nhsan
 */

#include "Manager.h"
#include "services/HelloService.h"
#include <glog/logging.h>
#include "commons/Constant.h"
namespace ukiman
{

Manager::Manager() :
		Component(COMP_MANAGER)
{
}

Manager::~Manager()
{
}

void Manager::init()
{
	Engine& engine = Engine::get_instance();
	string host;
	int port;

	Config& config = Engine::get_instance().get_configuration();
	Setting &sconf = config.getRoot()["general"];
	if (!sconf.lookupValue("host", host))
	{
		throw std::runtime_error("Configuration file do not contain man_host parameter");
	}

	if (!sconf.lookupValue("port", port))
	{
		throw std::runtime_error("Configuration file do not contain man_port parameter");
	}

	/* load server list */
	const Setting &nodes = config.getRoot()["servers"];
	int count = nodes.getLength();

	for (int i = 0; i < count; i++)
	{
		const Setting &server_node = nodes[i];
		int port, id;
		std::string ip, type, name;

		if (!(server_node.lookupValue("name", name) && server_node.lookupValue("ip", ip) && server_node.lookupValue("port", port) && server_node.lookupValue("type", type) && server_node.lookupValue("id", id)))
		{
			throw std::runtime_error("Error in load server connections informations");
		}
		servernode_ptr node(new ServerNode(id, name, type, ip, port));
		this->nodes.push_back(node);
	}

	engine.listen_connection(host, port); //start listen

	/* register services */DLOG(INFO) << "Register HelloService...";
	service_ptr hello_service = boost::make_shared<HelloService>();
	Engine::get_instance().register_service(hello_service);
	hello_service->activate();
}

void Manager::start()
{

}

void Manager::stop()
{

}

} /* namespace ukiman */
