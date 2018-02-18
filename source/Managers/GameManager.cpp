#include "../pch.h"
#include "GameManager.h"
#include "../Engine/GameEngine.h"

GUIFactory guiFactory;
GFXAssetManager assMan;
GUIOverlay guiOverlay;


unique_ptr<PromptDialog> GameManager::errorDialog;
unique_ptr<PromptDialog> GameManager::warningDialog;
Dialog* GameManager::showDialog = NULL;


GameManager::~GameManager() {

	if (blendState)
		delete blendState;
	showDialog = NULL;
	menuScreen.reset();
	currentScreen = NULL;
}


bool GameManager::initializeGame(GameEngine* gmngn, HWND hwnd, ComPtr<ID3D11Device> dvc) {

	gameEngine = gmngn;
	device = dvc;

	// get graphical assets from xml file
	if (!docAssMan.load_file("assets/AssetManifest.xml")) {
		GameEngine::errorMessage(L"Could not read AssetManifest file!",
			L"Fatal Read Error!", !Globals::FULL_SCREEN);
		return false;
	}

	xml_node gfxAssetNode = docAssMan.child("root").child("gfx");

	if (!assMan.initialize(gfxAssetNode, device)) {
		GameEngine::errorMessage(L"Failed to load GFX Assets!",
			L"Fatal Error!", !Globals::FULL_SCREEN);
		return false;
	}
	if (!guiFactory.initialize(hwnd, device, gameEngine->getDeviceContext(),
		gameEngine->getSwapChain(), gameEngine->getSpriteBatch(), &mouse,
		"assets/AssetManifest.xml")) {

		GameEngine::errorMessage(L"Failed to load GUIFactory",
			L"Fatal Error", !Globals::FULL_SCREEN);
		return false;
	}

	mouse.loadMouseIcon(&guiFactory, "Mouse Arrow");
	blendState = new CommonStates(device.Get());

	initErrorDialogs();

	guiOverlay.initialize();

	menuScreen.reset(new MenuManager());
	menuScreen->setGameManager(this);
	if (!menuScreen->initialize(device))
		return false;

	currentScreen = menuScreen.get();

	ShowCursor(false);

	return true;
}


void GameManager::initErrorDialogs() {
	Vector2 dialogPos, dialogSize;
	dialogSize = Vector2(float(Globals::WINDOW_WIDTH) / 2, float(Globals::WINDOW_HEIGHT) / 2);
	dialogPos = dialogSize;
	dialogPos.x -= dialogSize.x / 2;
	dialogPos.y -= dialogSize.y / 2;
	errorDialog.reset(guiFactory.createDialog(dialogSize, dialogPos, false, true, 5));
	errorDialog->setTint(Color(0, 120, 207));
	unique_ptr<Button> quitButton;
	quitButton.reset(guiFactory.createButton());
	quitButton->setText(L"Exit Program");
	quitButton->setActionListener(new QuitButtonListener(this));
	//quitButton->setMatrixFunction([&]()-> Matrix { return camera->translationMatrix(); });
	errorDialog->setCancelButton(move(quitButton));

	ScrollBarDesc scrollBarDesc;
	scrollBarDesc.upButtonImage = "ScrollBar Up Custom";
	scrollBarDesc.upPressedButtonImage = "ScrollBar Up Pressed Custom";
	scrollBarDesc.trackImage = "ScrollBar Track Custom";
	scrollBarDesc.scrubberImage = "Scrubber Custom";
	warningDialog.reset(guiFactory.createDialog(dialogPos, dialogSize, false, true, 3));
	warningDialog->setScrollBar(scrollBarDesc);
	//warningDialog->setMatrixFunction([&]()-> Matrix { return camera->translationMatrix(); });
	warningDialog->setTint(Color(0, 120, 207));
	warningDialog->setCancelButton(L"Continue");
	unique_ptr<Button> quitButton2;
	quitButton2.reset(guiFactory.createButton());
	quitButton2->setText(L"Exit Program");
	quitButton2->setActionListener(new QuitButtonListener(this));
	warningDialog->setConfirmButton(move(quitButton2));

	showDialog = warningDialog.get();
}


void GameManager::reloadGraphicsAssets() {
	device.Reset();
	device = gameEngine->getDevice();
	blendState = new CommonStates(device.Get());
	assMan.reInitDevice(device);
	guiFactory.reInitDevice(device, gameEngine->getDeviceContext(), gameEngine->getSpriteBatch());

	mouse.reloadGraphicsAsset(&guiFactory);
	menuScreen->reloadGraphicsAssets();
	guiOverlay.reloadGraphicsAssets();
	errorDialog->reloadGraphicsAsset();
	warningDialog->reloadGraphicsAsset();
}


void GameManager::update(double deltaTime) {
	if (showDialog->isOpen())
		showDialog->update(deltaTime);
	else
		currentScreen->update(deltaTime);
	guiOverlay.update(deltaTime);

}


void GameManager::draw(SpriteBatch* batch) {

	batch->Begin(SpriteSortMode_FrontToBack, blendState->NonPremultiplied());
	{
		currentScreen->draw(batch);
		guiOverlay.draw(batch);
		showDialog->draw(batch);
		mouse.draw(batch);
	}
	batch->End();
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

void GameManager::refreshTexture() {
	menuScreen->refreshTexture();
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


