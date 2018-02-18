#pragma once
#include "../../DXTKGui/Controls/GUIControl.h"

class AnimatedButton : public Selectable {
public:
	AnimatedButton(GUIFactory* factory, MouseController* mouseController,
		Animation* animation, Vector2 position);
	virtual ~AnimatedButton();

	virtual void reloadGraphicsAsset() override;

	/* For use in SelectionManager only. */
	virtual bool updateSelect(double deltaTime) override;
	virtual bool update(double deltaTime) override;
	virtual void draw(SpriteBatch* batch) override;

	/* Not used in Animated Button. */
	virtual void setFont(const pugi::char_t * font = "Default Font") override;
	/* Not used in Animated Button. */
	virtual void setText(wstring text) override;

	virtual const Vector2 XM_CALLCONV measureString() const override;
	virtual const Vector2& getPosition() const override;
	virtual const int getWidth() const override;
	virtual const int getHeight() const override;

	virtual void setLayerDepth(const float newDepth, bool frontToBack = true) override;

	virtual bool clicked() override;
	virtual bool pressed() override;
	virtual bool hovering() override;

	virtual void setToUnpressedState();
	virtual void setToHoverState();
	virtual void setToSelectedState();

	class ActionListener {
	public:
		virtual void onClick(AnimatedButton* button) = 0;
		virtual void onPress(AnimatedButton* button) = 0;
		virtual void onHover(AnimatedButton* button) = 0;
		virtual void afterHover(AnimatedButton* button) {
		};
	};


	void setActionListener(ActionListener* iOnC);

	virtual void onClick() override;
	virtual void onPress() override;
	virtual void onHover() override;
	virtual void resetState() override;

	double timeHovering = 0;
	double timePerFrame = .167;
	bool isOpen = false;
	int currentFrameIndex = -1;

	
	void adjustPosition(int lastFrame);
private:
	typedef void (ActionListener::*OnClickFunction) (AnimatedButton*);
	ActionListener* actionListener = NULL;
	OnClickFunction onClickFunction;
	OnClickFunction onPressFunction;
	OnClickFunction onHoverFunction;

	Animation* animation;
	string animationName;

	/** The origin point for adjusting position of animated frames. */
	/** Because frames of animation aren't always the same size... */
	Vector2 center;

	bool lastWasHover = false;

};
