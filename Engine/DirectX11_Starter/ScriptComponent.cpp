#include "ScriptComponent.h"
using namespace luabridge;
using namespace std;

ScriptComponent::ScriptComponent(string scriptFile, RigidBody* body)
{
	rigidbody = body; 

	// Define a Lua State object
	L = luaL_newstate();

	// Get script name and convert to C string 
	fileName = "Scripts/" + scriptFile;
	luaL_dofile(L, fileName.c_str());
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	// TO-DO
	// Setup Lua Scripting API to Rigidbody 



	getGlobalNamespace(L)
		.beginNamespace("test")
		.addVariable("x", &x)
		.addVariable("y", &y)
		.addVariable("z", &z)

		.endNamespace();

	cout << "Script Initialized" << endl; 
	
	 
	LuaRef initFunc = getGlobal(L, "Initialize");
	// Call script init
	try {
		initFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}
}

void ScriptComponent::Update()
{
	// Call Lua Update if one exists 
	LuaRef updateFunc = getGlobal(L, "Update");
	try {
		updateFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}

	rigidbody->GetTransform()->SetLocalPosition(x, y, z);
}

ScriptComponent::~ScriptComponent()
{
	LuaRef  releaseFunc = getGlobal(L, "Release");
	try {
		releaseFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}
	releaseFunc = Nil(); 
	lua_close(L);
}
