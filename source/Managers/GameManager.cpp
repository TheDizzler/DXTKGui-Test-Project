#include "../pch.h"
#include "GameManager.h"
#include "../Engine/GameEngine.h"

GUIFactory guiFactory;
GFXAssetManager assMan;
GUIOverlay guiOverlay;


GameManager::~GameManager() {
	menuScreen.reset();
	currentScreen = NULL;
}


bool GameManager::initializeGame(GameEngine* gmngn, HWND hwnd, ComPtr<ID3D11Device> dvc) {

	gameEngine = gmngn;
	device = dvc;

	// get graphical assets from xml file
	if (!docAssMan.load_file("assets/AssetManifest.xml")) {
		GameEngine::errorMessage(L"Could not read AssetManifest file!",
			L"Fatal Read Error!");
		return false;
	}

	xml_node gfxAssetNode = docAssMan.child("root").child("gfx");

	if (!assMan.initialize(gfxAssetNode, device)) {
		GameEngine::errorMessage(L"Failed to load GFX Assets!",
			L"Fatal Error!");
		return false;
	}

	guiOverlay.initialize();

	menuScreen.reset(new MenuManager());
	menuScreen->setGameManager(this);
	if (!menuScreen->initialize(device))
		return false;

	currentScreen = menuScreen.get();

	ShowCursor(false);

	return true;
}

void GameManager::reloadGraphicsAssets() {
	mouse.reloadGraphicsAsset(&guiFactory);
	menuScreen->reloadGraphicsAssets();
	guiOverlay.reloadGraphicsAssets();
}


void GameManager::update(double deltaTime) {
	currentScreen->update(deltaTime);
	guiOverlay.update(deltaTime);
}


void GameManager::draw(SpriteBatch* batch) {

	currentScreen->draw(batch);
	guiOverlay.draw(batch);
}


void GameManager::loadLevel(const wchar_t* file) {
// do nothing for this example
}

void GameManager::loadMainMenu() {

	lastScreen = currentScreen;
	currentScreen = menuScreen.get();

}


void GameManager::pause() {

	if (currentScreen != NULL)
		currentScreen->pause();
}


void GameManager::exit() {
	gameEngine->exit();
}



void GameManager::controllerRemoved(ControllerSocketNumber controllerSocket,
	PlayerSlotNumber slotNumber) {
	currentScreen->controllerRemoved(controllerSocket, slotNumber);
}

void GameManager::newController(shared_ptr<Joystick> newStick) {
	currentScreen->newController(newStick);
}



vector<ComPtr<IDXGIAdapter>> GameManager::getAdapterList() {
	return gameEngine->getAdapterList();
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayList() {
	return gameEngine->getDisplayList();
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayListFor(size_t displayIndex) {
	return gameEngine->getDisplayListFor(displayIndex);
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayListFor(
	ComPtr<IDXGIAdapter> adapter) {
	return gameEngine->getDisplayListFor(adapter);
}

vector<DXGI_MODE_DESC> GameManager::getDisplayModeList(size_t displayIndex) {
	return gameEngine->getDisplayModeList(displayIndex);
}

vector<DXGI_MODE_DESC> GameManager::getDisplayModeList(ComPtr<IDXGIOutput> display) {
	return gameEngine->getDisplayModeList(display);
}

void GameManager::refreshDisplayModeList() {

	menuScreen->refreshDisplayModeList();
}

//void GameManager::setDisplayMode(DXGI_MODE_DESC displayMode) {
//	gameEngine->setDisplayMode(displayMode);
//}

bool GameManager::setAdapter(size_t adapterIndex) {
	gameEngine->setChangeDisplaySettings(DisplayChangeType::DISPLAY_ADAPTER, adapterIndex);
	return true;
}

bool GameManager::setDisplayMode(size_t displayModeIndex) {
	gameEngine->setChangeDisplaySettings(DisplayChangeType::DISPLAY_MODE, displayModeIndex);
	return true;
}

bool GameManager::setFullScreen(bool isFullScreen) {
	gameEngine->setChangeDisplaySettings(DisplayChangeType::FULL_SCREEN, isFullScreen);
	return true;
}


size_t GameManager::getSelectedAdapterIndex() {
	return gameEngine->getSelectedAdapterIndex();
}

size_t GameManager::getSelectedDisplayIndex() {
	return gameEngine->getSelectedDisplayIndex();
}

size_t GameManager::getSelectedDisplayModeIndex() {
	return gameEngine->getSelectedDisplayModeIndex();
}
