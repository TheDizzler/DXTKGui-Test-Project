#pragma once
#include <map>
#include "../../DXTKGui/BaseGraphics/GraphicsAsset.h"

class GFXAssetManager {
public:
	virtual ~GFXAssetManager();

	bool initialize(xml_node gfxAssetsNode, ComPtr<ID3D11Device> device);

	void reInitDevice(ComPtr<ID3D11Device> device);

	unique_ptr<Sprite> getSpriteFromAsset(const char_t* assetName);
	shared_ptr<Animation> getAnimation(const char_t* animationName);
	GraphicsAsset* const getAsset(const char_t* assetName);
	shared_ptr<AssetSet> const getAssetSet(const char_t* setName);

	

private:

	xml_node gfxAssetsNode;

	map<string, unique_ptr<GraphicsAsset> > assetMap;
	map<string, shared_ptr<Animation> > animationMap;
	map<string, shared_ptr<AssetSet> > setMap;


	bool getGFXAssetsFromXML(ComPtr<ID3D11Device> device);

	ComPtr<ID3D11Device> device;
};