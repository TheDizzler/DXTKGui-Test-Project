#include "../pch.h"
#include "AnimatedButton.h"
#include "../Managers/GameManager.h"

/** ***** Animated Button ***** **/
AnimatedButton::AnimatedButton(GUIFactory* factory, MouseController* mouseController,
	Animation* anim, Vector2 pos) : Selectable(factory, mouseController) {

	animation = anim;
	animationName = animation->animationName;

	if (animation->animationFrames.size() > 0)
		currentFrameIndex = 0;

	position = pos;
	hitArea.position = position;
	hitArea.size = Vector2((float) getWidth(), (float) getHeight());
	center = Vector2(FLOAT(getWidth()) / 2, FLOAT(getHeight()) / 2);
}

AnimatedButton::~AnimatedButton() {
	if (actionListener != NULL)
		delete actionListener;
}

void AnimatedButton::reloadGraphicsAsset() {
	animation = assMan.getAnimation(animationName.c_str());
}


bool AnimatedButton::updateSelect(double deltaTime) {
	return update(deltaTime);
}

bool AnimatedButton::update(double deltaTime) {

	if (hitArea.contains(mouse->getPosition())) {
		//lastWasHover = true;
		if (!isPressed) {
			timeHovering += deltaTime;
			onHover();
		}
	} else/* if (lastWasHover)*/ {
		//lastWasHover = false;
		isHover = false;
		isPressed = false;
		if (timeHovering > 0) {
			timeHovering -= deltaTime;
			isOpen = false;
			if (timeHovering < 0) {
				timeHovering = timePerFrame;
				--currentFrameIndex;
				if (currentFrameIndex < 0) {
					currentFrameIndex = 0;
				} else
					adjustPosition(currentFrameIndex + 1);
			}
		}
	}


	if (isPressed && !mouse->leftButton()) {
		isClicked = true;
		onClick();
	} else {
		isClicked = false;
		if (!isHover) {
			isPressed = false;
		} else if (isOpen && mouse->pressed()) {
			isPressed = true;
			onPress();
		}
	}

	return false;
}


void AnimatedButton::draw(SpriteBatch* batch) {

	batch->Draw(animation->texture.Get(), position,
		&animation->animationFrames[currentFrameIndex]->sourceRect, tint, rotation,
		origin, scale, SpriteEffects_None, layerDepth);
}

void AnimatedButton::adjustPosition(int lastFrame) {

	Vector2 oldPos = position;
	Vector2 newPos = oldPos;

	Vector2 oldSize = Vector2(
		FLOAT(animation->animationFrames[lastFrame]->sourceRect.right
			- animation->animationFrames[lastFrame]->sourceRect.left),
		FLOAT(animation->animationFrames[lastFrame]->sourceRect.bottom
			- animation->animationFrames[lastFrame]->sourceRect.top));

	Vector2 newSize = Vector2((float) getWidth(), (float) getHeight());
	Vector2 difference = newSize - oldSize;

	newPos -= difference / 2;
	setPosition(newPos);
}


void AnimatedButton::setFont(const pugi::char_t* font) {
}

void AnimatedButton::setText(wstring text) {
}


/** Not used in Animated Button */
const Vector2 XM_CALLCONV AnimatedButton::measureString() const {
	return Vector2::Zero;
}

const Vector2& AnimatedButton::getPosition() const {
	return position;
}

const int AnimatedButton::getWidth() const {
	return animation->animationFrames[currentFrameIndex]->sourceRect.right
		- animation->animationFrames[currentFrameIndex]->sourceRect.left;
}

const int AnimatedButton::getHeight() const {
	return animation->animationFrames[currentFrameIndex]->sourceRect.bottom
		- animation->animationFrames[currentFrameIndex]->sourceRect.top;
}

void AnimatedButton::setLayerDepth(const float newDepth, bool frontToBack) {
	layerDepth = newDepth;
}

bool AnimatedButton::clicked() {
	return isClicked;
}

bool AnimatedButton::pressed() {
	return isPressed;
}

bool AnimatedButton::hovering() {
	return isHover;
}

void AnimatedButton::setToUnpressedState() {
}

void AnimatedButton::setToHoverState() {
}

void AnimatedButton::setToSelectedState() {
}

void AnimatedButton::setActionListener(ActionListener* iOnC) {
	if (actionListener != NULL)
		delete actionListener;
	onClickFunction = &ActionListener::onClick;
	onHoverFunction = &ActionListener::onHover;
	onPressFunction = &ActionListener::onPress;
	actionListener = iOnC;
}

void AnimatedButton::onClick() {
	if (actionListener != NULL) {
		(actionListener->*onClickFunction)(this);
	} else {
		currentFrameIndex = (int) animation->animationFrames.size() - 1;
	}

	isClicked = isPressed = false;
}

void AnimatedButton::onPress() {
	if (actionListener != NULL) {
		(actionListener->*onPressFunction)(this);
	} else
		currentFrameIndex = (int) animation->animationFrames.size() - 2;
}

void AnimatedButton::onHover() {
	isHover = true;
	if (actionListener != NULL) {
		(actionListener->*onHoverFunction)(this);
	} else {
		if (timeHovering > timePerFrame) {
			timeHovering = 0;
			++currentFrameIndex;
			if (currentFrameIndex > animation->animationFrames.size() - 3) {
				currentFrameIndex = (int) animation->animationFrames.size() - 3;
				isOpen = true;
			} else
				adjustPosition(currentFrameIndex - 1);
		}
	}
}

void AnimatedButton::resetState() {
	setToUnpressedState();
}
