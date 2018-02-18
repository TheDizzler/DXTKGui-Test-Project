#include "../pch.h"
#include "GameEngine.h"
#include "CommonStates.h"


GameEngine::~GameEngine() {


	if (audioEngine != NULL)
		audioEngine->Suspend();

}


bool GameEngine::initEngine(HWND hw, HINSTANCE hInstance) {

	if (!initD3D(hw)) {
		GameEngine::errorMessage(L"Direct3D Initialization Failed", L"Error");
		return false;
	}

	if (!initRawInput(hwnd)) {
		GameEngine::errorMessage(L"Raw Input Init failed", L"Error");
		return false;
	}

	// Initialize Audio Engine
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
//#ifdef _DEBUG
//	audioFlags = audioFlags | AudioEngine_Debug;
//#endif
	audioEngine = make_unique<AudioEngine>(audioFlags);
	retryAudio = false;

	if (!audioEngine->IsAudioDevicePresent()) {
		// no audio device found. Operating in silent mode.
	}

	if (!game.initializeGame(this, hwnd, device)) {
		GameEngine::errorMessage(L"Game Manager failed to load.", L"Critical Failure");
		return false;
	}


	gameInitialized = true;
	return true;
}

void GameEngine::onAudioDeviceChange() {
	retryAudio = true;
}

void GameEngine::reloadGraphicsAssets() {
	game.reloadGraphicsAssets();
}

void GameEngine::setChangeDisplaySettings(DisplayChangeType type, size_t variable) {
	changeDisplay = true;
	changeType = type;
	changeVariable = variable;
}



bool warningCanceled = false;
void GameEngine::run(double deltaTime) {

	update(deltaTime);
	render();

	if (changeDisplay) {
		switch (changeType) {
			case DisplayChangeType::FULL_SCREEN:
			{
				bool toFullScreen = true;
				if (changeVariable == 0)
					toFullScreen = false;
				setFullScreen(toFullScreen);
				mouse.resetPressed();
			}
			break;
			case DisplayChangeType::DISPLAY_MODE:
				changeDisplayMode(changeVariable);
				game.refreshTexture();
				break;
			case DisplayChangeType::DISPLAY_ADAPTER:
				setAdapter(changeVariable);
				game.refreshDisplayModeList();
				break;
		}
		changeDisplay = false;

		//OutputDebugString(L"Done\n");
	}

	if (!audioEngine->IsAudioDevicePresent() && !warningCanceled) {
		// no audio device found. Operating in silent mode.
		showWarningDialog(L"No audio device found. Operating in Silent Mode.\nEnd Message...",
			L"Audio Engine failure");
		warningCanceled = true;
	}

	if (retryAudio) {
		retryAudio = false;
		if (audioEngine->Reset()) {
			// restart looped sounds
		}
	} else if (!audioEngine->Update()) {
		if (audioEngine->IsCriticalError()) {
			showWarningDialog(L"Audio device lost!", L"Audio Engine failure");
			retryAudio = true;
		}
	}
}

void GameEngine::update(double deltaTime) {

	mouse.saveMouseState();
	keys.saveKeyState();
	slotManager.updateGamePads();

	game.update(deltaTime);
}



void GameEngine::render() {

	deviceContext->ClearRenderTargetView(renderTargetView.Get(), Colors::PeachPuff);
	game.draw(batch.get());
	swapChain->Present(0, 0);
}

void GameEngine::suspend() {

	stopFullScreen();
	game.pause();
	if (audioEngine != NULL)
		audioEngine->Suspend();
}

void GameEngine::resume() {

	setFullScreen(Globals::FULL_SCREEN);
	if (audioEngine != NULL)
		audioEngine->Resume();
}

void GameEngine::exit() {
	if (swapChain.Get() != NULL)
		swapChain->SetFullscreenState(false, NULL);
	DestroyWindow(hwnd);
}

void GameEngine::controllerRemoved(ControllerSocketNumber controllerSocket,
	PlayerSlotNumber slotNumber) {

	game.controllerRemoved(controllerSocket, slotNumber);
}

void GameEngine::newController(shared_ptr<Joystick> newStick) {

	game.newController(newStick);
}