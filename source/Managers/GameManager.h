#pragma once

#include "MenuManager.h"
#include "../../DXTKGui/GUIFactory.h"
#include "../../DXTKGui/Controllers/Joystick.h"
#include "../Screens/GUIOverlay.h"
#include "GFXAssetManager.h"

#include <CommonStates.h>


class GameEngine;

extern GUIFactory guiFactory;
extern GFXAssetManager assMan;
extern GUIOverlay guiOverlay;

/** The lowest level of class where game code should be included.
	Everything below this (GameEngine downward) should generally go unmodified. */
class GameManager {
public:

	virtual ~GameManager();


	bool initializeGame(GameEngine* gameEngine, HWND hwnd, ComPtr<ID3D11Device> device);

	void reloadGraphicsAssets();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);


	void loadLevel(const wchar_t* file);
	void loadMainMenu();

	void pause();
	void exit();

	void controllerRemoved(ControllerSocketNumber controllerSocket,
		PlayerSlotNumber slotNumber);
	void newController(shared_ptr<Joystick> newStick);

	vector<ComPtr<IDXGIAdapter> > getAdapterList();
	vector<ComPtr<IDXGIOutput> > getDisplayList();
	vector<ComPtr<IDXGIOutput> > getDisplayListFor(size_t displayIndex);
	vector<ComPtr<IDXGIOutput> > getDisplayListFor(ComPtr<IDXGIAdapter> adapter);
	vector<DXGI_MODE_DESC> getDisplayModeList(size_t displayIndex);
	vector<DXGI_MODE_DESC> getDisplayModeList(ComPtr<IDXGIOutput> display);

	void refreshTexture();
	void refreshDisplayModeList();


	bool setAdapter(size_t adapterIndex);
	bool setDisplayMode(size_t displayModeIndex);
	bool setFullScreen(bool isFullScreen);

	size_t getSelectedAdapterIndex();
	size_t getSelectedDisplayIndex();
	size_t getSelectedDisplayModeIndex();


	/** If game set to FullScreen, Windows MessageBox will never display even
	if showMessageBox set to TRUE. */
	static void errorMessage(wstring message, wstring title = L"Fatal Error",
		bool showMessageBox = false) {

		message += L"\n";
		if (!Globals::FULL_SCREEN && showMessageBox)
			MessageBox(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONERROR);

		title += L" >> " + message;
		OutputDebugString(title.c_str()); // always output debug just in case

		if (!showMessageBox && guiFactory.initialized) {
			showWarningDialog(message, title);
		}
	}

	static void showErrorDialog(wstring message, wstring title) {
		errorDialog->show();
		errorDialog->setTitle(title);
		errorDialog->setText(message);
		showDialog = errorDialog.get();
	}

	static void showWarningDialog(wstring message, wstring title) {
		warningDialog->show();
		warningDialog->setTitle(title);
		warningDialog->setText(message);
		warningDialog->setTextTint(Color(1, 0, 0, 1));
		showDialog = warningDialog.get();
	}

private:

	Screen* currentScreen = 0;
	Screen* lastScreen = 0;
	unique_ptr<MenuManager> menuScreen;


	GameEngine* gameEngine;
	ComPtr<ID3D11Device> device;
	CommonStates* blendState;

	pugi::xml_document docAssMan;

	void initErrorDialogs();

	/* Critical error dialog. Exits game when dismissed. */
	static unique_ptr<PromptDialog> errorDialog;
	/* Minor error dialog. Choice between exit game and continue. */
	static unique_ptr<PromptDialog> warningDialog;
	static Dialog* showDialog;


};


class QuitButtonListener : public Button::ActionListener {
public:
	QuitButtonListener(GameManager* gm) : gameManager(gm) {
	}
	virtual void onClick(Button* button) override {
		gameManager->exit();
	}
	virtual void onPress(Button* button) override {
	}
	virtual void onHover(Button* button) override {
	}
	virtual void resetState(Button* button) override {
	}

	GameManager* gameManager;
};