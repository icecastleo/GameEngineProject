#include "Engine.h"

#include "NativeWindow.h"
#include "RenderingSystem.h"
#include "TransformSystem.h"
#include "Scene.h"
#include "Entity.h"
#include "ScriptComponent.h"
#include "SceneManager.h"

#ifdef HAS_EDITOR
#include "Editor.h"
#endif

using std::vector;

Engine* Engine::_instance = nullptr;

bool Engine::Init()
{
	if (nullptr != _instance)
		return false;

	// other module could use this;
	_instance = this;


	nativeWindow = new NativeWindow();

	if (!nativeWindow->Init())
		return false;

	// FIXME: get world entities 
	//entities = new vector<Entity *>();

	//Entity *entity = new Entity();
	//entities->push_back(entity);

	renderingSystem = new RenderingSystem();
	transformSystem = new TransformSystem();
	//physicsSystem = new PhysicsSystem();

	if (!renderingSystem->Init(nativeWindow))
		return false;

	inputManager = &InputManager::instance();
	inputManager->SetWindowHandle(nativeWindow->GetWindowHandle());

#ifdef HAS_EDITOR
	Editor* editor = new Editor();
	editor->Init(); // singleton initilized, it's ok to leave the pointer
#endif

	// TODO !
	// hardcoded debug code, 
	// scene loading should be in scripts or config file
	// and should be done by SceneManager
	//currentScene = SceneManager::LoadScene("Scene1");
	//currentScene = SceneManager::CreateNewScene("Scene1");

	//SceneManager::SaveScene(currentScene);
	InitScene();
	currentScene->Enter();  // should be called anyway
	//;
	return true;
}

int Engine::Run()
{
	running = true;

	InitTimer();

	while (running)
	{
		// deltaTime and totalTime
		UpdateTimer();

		// native window event processiing
		nativeWindow->ProcessEvent();

#ifdef HAS_GUI
		(GUI::instance().Update(nativeWindow->GetWindowWidth(), nativeWindow->GetWindowHeight(), &running));
#endif

		nativeWindow->CalculateFrameStats(totalTime);
		if (nativeWindow->WindowIsClosed())
		{
			running = false;
		}

		// process input events
		inputManager->UpdateInput(deltaTime);
		if (inputManager->GetQuit())
		{
			Quit(0);
		}

		if (nullptr != currentScene)
		{
			// TODO
			//Transform System
			transformSystem->update(deltaTime, currentScene->componentManager->GetAllComponents<Transform>());
		}

		// scripting system
		// TODO


#ifdef HAS_EDITOR
		Editor::instance()->Update(deltaTime, totalTime);
#else
		// TODO !
		// hardcoded debug code, 
		// this should be done by SceneManager
		if (nullptr != currentScene)
		{
			currentScene->Tick(deltaTime, totalTime);
		}
#endif

		// particle system update
		renderingSystem->Update(deltaTime, totalTime);

		// rendering System
#ifdef HAS_EDITOR
		renderingSystem->DrawScene(Editor::instance()->GetEditorCamera(), currentScene);
#else
		renderingSystem->DrawScene(currentScene->GetCamera(), currentScene);
#endif

		// TODO
		//physicsSystem->update(deltaTime, currentScene->componentManager->rigidBodyComponents);
	}

	// GUI Cleanup
#ifdef HAS_GUI
	GUI::instance().End();
#endif

	CleanUp();

	return 0;
}

void Engine::LoadScene(std::string name)
{
	if (name.empty())
		return;

	Scene* newScene = SceneManager::LoadScene(name);
	if (nullptr == newScene)
		return;

	if (nullptr != currentScene)
		currentScene->Exit();
	delete currentScene;
	currentScene = nullptr;
	currentScene = newScene;
	currentScene->Enter();
}

void Engine::SaveScene()
{
	if (currentScene->GetName().empty())
		return;
	SceneManager::SaveScene(currentScene);
}

void Engine::InitScene()
{
#ifdef HAS_EDITOR
	currentScene = nullptr;
	currentScene = SceneManager::CreateNewScene("NewScene");
#else
	LoadScene("Scene1");
#endif // HAS_EDITOR

}

void Engine::Quit(int exitCode)
{
	PostQuitMessage(exitCode);
}

void Engine::CleanUp()
{
	// TODO !
	// hardcoded debug code, 
	// this should be done by SceneManager
	if (nullptr != currentScene)
	{
		currentScene->Exit();
	}
	delete currentScene;

#ifdef HAS_EDITOR
	Editor::instance()->CleanUp();
#endif

	delete renderingSystem;
	delete transformSystem;
	//delete physicsSystem;
	delete nativeWindow;
}

void Engine::InitTimer()
{
	__int64 perfFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
	perfCounterSeconds = 1.0 / (double)perfFreq;

	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	startTime = now;
	currentTime = now;
	previousTime = now;
}

void Engine::UpdateTimer()
{
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	currentTime = now;

	deltaTime = max((float)((currentTime - previousTime) * perfCounterSeconds), 0.0f);

	// Calculate the total time from start to now
	totalTime = (float)((currentTime - startTime) * perfCounterSeconds);

	// Save current time for next frame
	previousTime = currentTime;
}
