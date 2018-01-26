#include "../pch.h"
#include "GFXAssetManager.h"
#include "../../DXTKGui/StringHelper.h"
#include "../Engine/GameEngine.h"


GFXAssetManager::~GFXAssetManager() {
	assetMap.clear();
	animationMap.clear();
	setMap.clear();
}

bool GFXAssetManager::initialize(xml_node gfxAN, ComPtr<ID3D11Device> dev) {

	gfxAssetsNode = gfxAN;

	device = dev;
	if (!getGFXAssetsFromXML(device)) {
		GameEngine::errorMessage(L"Sprite retrieval from Asset Manifest failed.",
			L"Epic Fail");
		return false;
	}


	return true;
}

void GFXAssetManager::reInitDevice(ComPtr<ID3D11Device> dev) {

	device = dev;

	assetMap.clear();
	animationMap.clear();
	setMap.clear();

	if (!getGFXAssetsFromXML(device)) {
		GameEngine::errorMessage(L"Sprite retrieval from Asset Manifest failed.",
			L"Epic failure");
	}
}


unique_ptr<Sprite> GFXAssetManager::getSpriteFromAsset(const char_t* assetName) {

	GraphicsAsset* const asset = getAsset(assetName);
	if (asset == NULL)
		return NULL;
	unique_ptr<Sprite> sprite;
	sprite.reset(new Sprite());
	sprite->load(asset);
	return sprite;
}

shared_ptr<Animation> GFXAssetManager::getAnimation(const char_t* animationName) {

	if (animationMap.find(animationName) == animationMap.end()) {
		wostringstream ws;
		ws << "Cannot find asset file: " << animationName << "\n";
		ws << "Count : " << animationMap.count(animationName) << "\n";
		OutputDebugString(ws.str().c_str());

		GraphicsAsset* gfxAsset = getAsset(animationName);
		if (gfxAsset == NULL)
			return NULL;

		// create one frame animation
		float frameTime = 1000;
		vector<shared_ptr<Frame>> frames;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = gfxAsset->getWidth();
		rect.bottom = gfxAsset->getHeight();
		shared_ptr<Frame> frame;
		frame.reset(new Frame(rect, Vector2::Zero, frameTime));
		frames.push_back(move(frame));
		shared_ptr<Animation> animationAsset;
		animationAsset.reset(new Animation(gfxAsset->getTexture(), frames, animationName));
		animationMap[animationName] = animationAsset;

	}

	return animationMap[animationName];

}

GraphicsAsset* const GFXAssetManager::getAsset(const char_t* assetName) {

	if (assetMap.find(assetName) == assetMap.end()) {
		wostringstream ws;
		ws << "Cannot find asset file: " << assetName << "\n";
		OutputDebugString(ws.str().c_str());
		return NULL;
	}

	return assetMap[assetName].get();
}

shared_ptr<AssetSet> const GFXAssetManager::getAssetSet(const char_t* setName) {

	if (setMap.find(setName) == setMap.end()) {
		wostringstream ws;
		ws << "Cannot find asset set: " << setName << "\n";
		OutputDebugString(ws.str().c_str());
		return NULL;
	}

	return setMap[setName];
}

//unique_ptr<CreatureAsset> GFXAssetManager::getMonsterAsset(const char_t* assetName) {
//
//	for (xml_node monsterNode : monsterDoc.child("root").children("Monster")) {
//
//		const char* name = monsterNode.attribute("name").as_string();
//		if (string(name) == string(assetName)) {
//
//			xml_node gfxNode = monsterNode.child("gfx");
//			const char* spritesheet = gfxNode.attribute("file").as_string();
//
//			unique_ptr<CreatureAsset> monsterAsset = make_unique<CreatureAsset>();
//
//			if (assetMap.find(spritesheet) == assetMap.end()) {
//				wostringstream msg;
//				msg << "Cannot find asset file: " << spritesheet << "\n";
//				GameManager::showErrorDialog(msg.str(), L"Asset Find Failure");
//				return NULL;
//			}
//
//			Vector2 position(gfxNode.attribute("x").as_int(), gfxNode.attribute("y").as_int());
//			Vector2 size(gfxNode.attribute("width").as_int(), gfxNode.attribute("height").as_int());
//
//			Vector2 origin = Vector2(-1000, -1000);
//			xml_node originNode = gfxNode.child("origin");
//			if (originNode) {
//				origin.x = originNode.attribute("x").as_int();
//				origin.y = originNode.attribute("y").as_int();
//			}
//
//			monsterAsset->loadAsPartOfSheet(assetMap[spritesheet]->getTexture(), name,
//				position, size, origin);
//
//			return monsterAsset;
//		}
//
//	}
//
//	return NULL;
//}
//
//unique_ptr<CreatureAsset> GFXAssetManager::getCharacterAsset(const char_t * assetName) {
//	for (xml_node characterNode : characterDoc.child("root").children("Character")) {
//
//		const char* name = characterNode.attribute("name").as_string();
//		if (string(name) == string(assetName)) {
//
//			xml_node gfxNode = characterNode.child("gfx");
//			const char* spritesheet = gfxNode.attribute("file").as_string();
//
//			unique_ptr<CreatureAsset> characterAsset = make_unique<CreatureAsset>();
//
//			if (assetMap.find(spritesheet) == assetMap.end()) {
//				wostringstream msg;
//				msg << "Cannot find asset file: " << spritesheet << "\n";
//				GameManager::showErrorDialog(msg.str(), L"Asset Find Failure");
//				return NULL;
//			}
//
//			Vector2 position(gfxNode.attribute("x").as_int(), gfxNode.attribute("y").as_int());
//			Vector2 size(gfxNode.attribute("width").as_int(), gfxNode.attribute("height").as_int());
//
//			Vector2 origin = Vector2(-1000, -1000);
//			xml_node originNode = gfxNode.child("origin");
//			if (originNode) {
//				origin.x = originNode.attribute("x").as_int();
//				origin.y = originNode.attribute("y").as_int();
//			}
//
//			characterAsset->loadAsPartOfSheet(assetMap[spritesheet]->getTexture(), name,
//				position, size, origin);
//
//			return characterAsset;
//		}
//
//	}
//
//	return NULL;
//}
//
//bool GFXAssetManager::getTerrainAssetsFromXML() {
//
//
//
//	for (xml_node terrainTypeNode : terrainDoc.child("root").children("Terrain")) {
//
//		unique_ptr<MapTileAsset> newTerrain;
//		TerrainType type;
//		const char_t* name = terrainTypeNode.attribute("name").as_string();
//		if (name == string("Grass")) {
//			type = TerrainType::GRASS;
//			newTerrain = make_unique<MapTileAsset>(type);
//		} else if (name == string("ForestLight")) {
//			type = TerrainType::LIGHT_FOREST;
//			newTerrain = make_unique<MapTileAsset>(type);
//		} else if (name == string("Ocean")) {
//			type = TerrainType::OCEAN;
//			newTerrain = make_unique<MapTileAsset>(type);
//		} else if (name == string("Mountain")) {
//			type = TerrainType::MOUNTAIN;
//			newTerrain = make_unique<MapTileAsset>(type);
//		} else if (name == string("Start")) {
//			type = TerrainType::START_TILE;
//			newTerrain = make_unique<MapTileAsset>(type);
//		} else {
//			wostringstream msg;
//			msg << L"Could not find Map Tile " << name;
//			GameEngine::showWarningDialog(msg.str(), L"Map Tile Warning");
//			continue;
//		}
//
//
//		xml_node gfxNode = terrainTypeNode.child("gfx");
//		const char* spritesheet = gfxNode.attribute("file").as_string();
//
//		if (assetMap.find(spritesheet) == assetMap.end()) {
//			wostringstream msg;
//			msg << "Cannot find asset file: " << spritesheet << "\n";
//			GameManager::showErrorDialog(msg.str(), L"Asset Find Failure");
//			return false;
//		}
//
//		Vector2 position(gfxNode.attribute("x").as_int(), gfxNode.attribute("y").as_int());
//		Vector2 size(gfxNode.attribute("width").as_int(), gfxNode.attribute("height").as_int());
//
//		Vector2 origin = Vector2(0, 0);
//		/*xml_node originNode = gfxNode.child("origin");
//		if (originNode) {
//			origin.x = originNode.attribute("x").as_int();
//			origin.y = originNode.attribute("y").as_int();
//		}*/
//
//		newTerrain->loadAsPartOfSheet(assetMap[spritesheet]->getTexture(), name,
//			position, size, origin);
//
//		terrainTilesMap[type] = move(newTerrain);
//		}
//
//		return true;
//	}



	bool GFXAssetManager::getGFXAssetsFromXML(ComPtr<ID3D11Device> device) {

		string assetsDir =
			gfxAssetsNode.parent().attribute("dir").as_string();

		string gfxDir = assetsDir + gfxAssetsNode.attribute("dir").as_string();

		//for (xml_node spriteNode : gfxAssetsNode.children("sprite")) {

		//	string file_s = gfxDir + spriteNode.attribute("file").as_string();
		//	const char_t* file = file_s.c_str();
		//	const char_t* name = spriteNode.attribute("name").as_string();
		//	string check = name;

		//	Vector2 origin = Vector2(-1000, -1000);
		//	xml_node originNode = spriteNode.child("origin");
		//	if (originNode) {
		//		origin.x = originNode.attribute("x").as_int();
		//		origin.y = originNode.attribute("y").as_int();
		//	}

		//	unique_ptr<GraphicsAsset> gfxAsset = make_unique<GraphicsAsset>();
		//	if (!gfxAsset->load(device, name, StringHelper::convertCharStarToWCharT(file), origin)) {
		//		wstringstream wss;
		//		wss << "Unable to load texture file: " << file;
		//		//MessageBox(0, wss.str().c_str(), L"Critical error", MB_OK);
		//		GameEngine::errorMessage(wss.str());
		//		return false;
		//	}

		//	if (spriteNode.attribute("set")) {
		//		string setName = spriteNode.attribute("set").as_string();
		//		if (setMap.find(setName) == setMap.end()) {
		//			// new set
		//			setMap[setName] = make_shared<AssetSet>(setName.c_str());
		//		}
		//		setMap[setName]->addAsset(check, move(gfxAsset));

		//	} else
		//		assetMap[check] = move(gfxAsset);
		//}

		for (xml_node spritesheetNode : gfxAssetsNode.children("spritesheet")) {
		/*for (xml_node spritesheetNode = gfxAssetsNode.child("spritesheet");
			spritesheetNode; spritesheetNode = spritesheetNode.next_sibling("spritesheet")) {*/

			string file_s = gfxDir + spritesheetNode.attribute("file").as_string();
			const char_t* file = file_s.c_str();

			const char_t* name = spritesheetNode.attribute("name").as_string();

			// the spritesheet itself is never saved into the map
			unique_ptr<GraphicsAsset> masterAsset;
			masterAsset.reset(new GraphicsAsset());
			if (!masterAsset->load(device, file, StringHelper::convertCharStarToWCharT(file))) {
				wstringstream wss;
				wss << "Error load asset file: " << file;
				GameEngine::errorMessage(wss.str());
				return false;
			}

			
		// parse all animations from spritesheet
		//	for (xml_node animationNode = spritesheetNode.child("animation");
		//		animationNode; animationNode = animationNode.next_sibling("animation")) {

		//		const char_t* name = animationNode.attribute("name").as_string();
		//		float frameTime = animationNode.attribute("timePerFrame").as_float();

		//		vector<shared_ptr<Frame>> frames;
		//		for (xml_node spriteNode = animationNode.child("sprite"); spriteNode;
		//			spriteNode = spriteNode.next_sibling("sprite")) {

		//			RECT rect;
		//			rect.left = spriteNode.attribute("x").as_int();
		//			rect.top = spriteNode.attribute("y").as_int();
		//			rect.right = rect.left + spriteNode.attribute("width").as_int();
		//			rect.bottom = rect.top + spriteNode.attribute("height").as_int();

		//			Vector2 origin = Vector2(0, 0);
		//			xml_node originNode = spriteNode.child("origin");
		//			if (originNode) {
		//				origin.x = originNode.attribute("x").as_int();
		//				origin.y = originNode.attribute("y").as_int();
		//			}

		//			shared_ptr<Frame> frame;
		//			if (spriteNode.attribute("frameTime"))
		//				frame.reset(new Frame(rect, origin,
		//					spriteNode.attribute("frameTime").as_float()));
		//			else
		//				frame.reset(new Frame(rect, origin, frameTime));
		//			frames.push_back(move(frame));

		//		}

		//		shared_ptr<Animation> animationAsset;
		//		animationAsset.reset(new Animation(masterAsset->getTexture(), frames, name));
		//		animationMap[name] = animationAsset;
		//	}

			// parse all single sprites from spritesheet
			for (xml_node spriteNode = spritesheetNode.child("sprite"); spriteNode;
				spriteNode = spriteNode.next_sibling("sprite")) {

				const char_t* name = spriteNode.attribute("name").as_string();
				// pos in spritesheet
				Vector2 position = Vector2(spriteNode.attribute("x").as_int(),
					spriteNode.attribute("y").as_int());
				// dimensions in spritesheet
				Vector2 size = Vector2(spriteNode.attribute("width").as_int(),
					spriteNode.attribute("height").as_int());

				Vector2 origin = Vector2(-1000, -1000);
				xml_node originNode = spriteNode.child("origin");
				if (originNode) {
					origin.x = originNode.attribute("x").as_int();
					origin.y = originNode.attribute("y").as_int();
				}

				unique_ptr<GraphicsAsset> spriteAsset = make_unique<GraphicsAsset>();
				spriteAsset->loadAsPartOfSheet(masterAsset->getTexture(), name,
					position, size, origin);

				if (spriteNode.attribute("set")) {
					string setName = spriteNode.attribute("set").as_string();
					if (setMap.find(setName) == setMap.end()) {
						// new set
						setMap[setName] = make_shared<AssetSet>(setName.c_str());
					}
					setMap[setName]->addAsset(name, move(spriteAsset));

				} else
					assetMap[name] = move(spriteAsset);
			}

			assetMap[name] = move(masterAsset);

		}

		return true;
	}
	/**** ***** GFXAssetManager END ***** ****/