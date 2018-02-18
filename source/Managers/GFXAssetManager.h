#pragma once
#include <map>
//#include "../../DXTKGui/BaseGraphics/GraphicsAsset.h"
#include "../CustomGUI/AnimatedButton.h"


class GFXAssetManager {
public:
	virtual ~GFXAssetManager();

	bool initialize(xml_node gfxAssetsNode, ComPtr<ID3D11Device> device);

	void reInitDevice(ComPtr<ID3D11Device> device);

	/** Creates a new unique_ptr sprite from assets. */
	unique_ptr<Sprite> getSpriteFromAsset(const char_t* assetName);
	Animation* getAnimation(const char_t* animationName);
	GraphicsAsset* const getAsset(const char_t* assetName);
	AssetSet* const getAssetSet(const char_t* setName);



	AnimatedButton* createAnimatedButton(const char_t* animatedButtonName,
		Vector2 position = Vector2::Zero);
	

private:

	xml_node gfxAssetsNode;

	map<string, unique_ptr<GraphicsAsset> > assetMap;
	map<string, unique_ptr<Animation> > animationMap;
	map<string, unique_ptr<AssetSet> > setMap;


	bool getGFXAssetsFromXML(ComPtr<ID3D11Device> device);

	ComPtr<ID3D11Device> device;
};