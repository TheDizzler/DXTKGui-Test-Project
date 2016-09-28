#pragma once

#include <dinput.h>


#include "../Controls/TextLabel.h"
#include "../Engine/MouseController.h"
#include "../Engine/KeyboardController.h"


class GameManager;

/** An interface class for a game screen; menu screens,
	level screens, etc. */
interface Screen {
public:
	virtual bool initialize(ComPtr<ID3D11Device> device, MouseController* mouse) = 0;
	virtual void setGameManager(GameManager* game) = 0;
	virtual void update(double deltaTime, MouseController* mouse) = 0;
	virtual void draw(SpriteBatch* batch) = 0;
	virtual void pause() = 0;

	/** Used for allowing transition effects between screens. */
	virtual const Vector2& getPosition() const = 0;
	//virtual void setPosition(const Vector2& newPositon) = 0;

	//virtual void setOpenTransition(TransitionEffects::TransitionEffect* effect) = 0;
	//virtual void setCloseTransition(TransitionEffects::TransitionEffect* effect) = 0;
};