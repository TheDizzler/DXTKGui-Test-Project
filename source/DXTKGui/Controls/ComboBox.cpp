#include "ComboBox.h"


ComboBox::ComboBox(GUIFactory* factory, shared_ptr<MouseController> mouseController,
	const Vector2& pos, const int len, size_t itemHeight, const int maxItemsShown)
	: GUIControl(factory, mouseController) {

	position = pos;
	width = len;

	maxDisplayItems = maxItemsShown;

}


ComboBox::~ComboBox() {

	if (actionListener)
		delete actionListener;

}

#include "../GUIFactory.h"
bool ComboBox::initialize(shared_ptr<FontSet> fnt,
	ListBox* lstBx, const pugi::char_t* buttonName, bool enumerateList) {


	comboListButton.reset(
		(ImageButton*) guiFactory->createImageButton(buttonName));

	comboListButton->setPosition(
		Vector2(position.x + width - comboListButton->getWidth(), position.y));
	comboListButton->setActionListener(new ShowListBoxListener(this));

	frame.reset(guiFactory->createRectangleFrame(
		position, Vector2(width, comboListButton->getScaledHeight())));

	listBox.reset(lstBx);
	listBox->setActionListener(new ListBoxListener(this));

	selectedLabel.reset(guiFactory->createTextLabel(
		Vector2(position.x + textMarginX, position.y + textMarginY)));
	selectedLabel->setActionListener(new SelectedOnClick(this));

	selectedBackgroundSprite.reset(guiFactory->createRectangle(position,
		Vector2(width, comboListButton->getScaledHeight()), Color(.5, .5, .5, 1)));

	height = frame->getHeight();

	texturePanel.reset(guiFactory->createPanel());



	return true;
}

void ComboBox::setScrollBar(ScrollBarDesc& scrollBarDesc) {
	listBox->setScrollBar(scrollBarDesc);
}

bool ComboBox::update(double deltaTime) {

	if (frame->contains(mouse->getPosition()) && mouse->clicked()) {
		comboListButton->onClick();
		refreshTexture = true;
	}

	if (selectedLabel->update(deltaTime))
		refreshTexture = true;
	if (comboListButton->update(deltaTime))
		refreshTexture = true;
	if (frame->update())
		refreshTexture = true;


	if (isOpen) {
		if (listBox->update(deltaTime))
			refreshTexture = true;
	}

	if (refreshTexture) {
		texturePanel->setTexture(texturize());
		refreshTexture = false;
		return true;
	}
	return false;
}

void ComboBox::draw(SpriteBatch* batch) {

	if (isOpen) {
		listBox->draw(batch);
	}

	texturePanel->draw(batch);
	/*selectedBackgroundSprite->draw(batch);
	selectedLabel->draw(batch);
	comboListButton->draw(batch);
	frame->draw(batch);*/
}


unique_ptr<GraphicsAsset> ComboBox::texturize() {
	return move(guiFactory->createTextureFromIElement2D(this));
}

void ComboBox::textureDraw(SpriteBatch * batch, ComPtr<ID3D11Device> device) {

	selectedBackgroundSprite->draw(batch);
	selectedLabel->draw(batch);
	comboListButton->draw(batch);
	frame->draw(batch);
}

void ComboBox::setPosition(const Vector2& pos) {
	Vector2 moveDiff = pos - position;
	position = pos;
	selectedBackgroundSprite->moveBy(moveDiff);
	selectedLabel->moveBy(moveDiff);
	comboListButton->moveBy(moveDiff);
	frame->moveBy(moveDiff);

	listBox->moveBy(moveDiff);
	texturePanel->setTexturePosition(pos);
}

void ComboBox::moveBy(const Vector2 & moveVector) {
	position += moveVector;
	selectedBackgroundSprite->moveBy(moveVector);
	selectedLabel->moveBy(moveVector);
	comboListButton->moveBy(moveVector);
	frame->moveBy(moveVector);
	listBox->moveBy(moveVector);
	texturePanel->setTexturePosition(position);
}


void ComboBox::show() {

	isOpen = !isOpen;

}

void ComboBox::hide() {
	isOpen = false;
}

void ComboBox::resizeBox() {

	comboListButton->setPosition(
		Vector2(position.x + width - comboListButton->getWidth(), position.y));
	frame->setDimensions(position, Vector2(width, comboListButton->getHeight()));
	selectedBackgroundSprite->setDimensions(position,
		Vector2(width, comboListButton->getHeight()));
}

void ComboBox::alwaysShowScrollBar(bool alwaysShow) {
	listBox->alwaysShowScrollBar(alwaysShow);
}

void ComboBox::setFont(const pugi::char_t * font) {
}

void ComboBox::setSelected(size_t newIndex) {
	listBox->setSelected(newIndex);
	selectedLabel->setText(listBox->getSelected()->toString());
}

ListItem* ComboBox::getSelected() {
	return listBox->getSelected();
}

ListItem* ComboBox::getItem(size_t index) {
	return listBox->getItem(index);
}

void ComboBox::setText(wstring text) {
}

const Vector2& XM_CALLCONV ComboBox::measureString() const {
	return Vector2::Zero;
}

const Vector2 & ComboBox::getPosition() const {
	return position;
}

const int ComboBox::getWidth() const {
	return width;
}

const int ComboBox::getHeight() const {
	return height;
}

bool ComboBox::clicked() {
	return isClicked;
}

bool ComboBox::pressed() {
	return isPressed;
}

bool ComboBox::hovering() {
	return isHover;
}

void ComboBox::addItem(ListItem* item) {
	listBox->addItem(item);
	// may have to do some resizing here
	width = listBox->getWidth();
	resizeBox();
}

void ComboBox::addItems(vector<ListItem*> items) {
	listBox->addItems(items);
	width = listBox->getWidth();
	resizeBox();
}

void ComboBox::clear() {
	listBox->clear();
	resizeBox();
}

void ComboBox::ShowListBoxListener::onClick(Button* button) {
	comboBox->show();
}

void ComboBox::ShowListBoxListener::onPress(Button* button) {
}

void ComboBox::ShowListBoxListener::onHover(Button* button) {
}

void ComboBox::ListBoxListener::onClick(ListBox* listbox, int selectedItemIndex) {
	comboBox->onClick();
}

