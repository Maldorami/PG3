#include "ParseMap.h"
#include "tinyxml2.h"
int uiMapWidth;
int uiMapHeight;
int uiTileWidth;
int uiTileHeight;
int width;
int height;
int firstTileID;
int columns;
int id;
int x;
int y;
int rows;
int tileGID;
int subRectToUse;
int i;
int j;
int maxTiles;
int MapHeight;
int MapWidth;
bool ParseMap::importMap(Map& rkMap, Renderer& rkRenderer, const std::string& rkFilename)
{
	tinyxml2::XMLDocument kXMLDoc;
	kXMLDoc.LoadFile(rkFilename.c_str());

	// map info
	tinyxml2::XMLElement* pkMapElement = kXMLDoc.FirstChildElement("map");

	uiMapWidth = atoi(pkMapElement->Attribute("width"));
	uiMapHeight = atoi(pkMapElement->Attribute("height"));

	rkMap.setDimensions(uiMapWidth, uiMapHeight);

	uiTileWidth = atoi(pkMapElement->Attribute("tilewidth"));
	uiTileHeight = atoi(pkMapElement->Attribute("tileheight"));

	rkMap.setTileDimensions(uiTileWidth, uiTileHeight);

	// tiles image path
	tinyxml2::XMLElement* pkTilesetElement = pkMapElement->FirstChildElement("tileset");
	firstTileID = atoi(pkTilesetElement->Attribute("firstgid"));
	tinyxml2::XMLElement* pkImageElement = pkTilesetElement->FirstChildElement("image");

	std::string kImagePath = "Assets/";
	kImagePath += pkImageElement->Attribute("source");

	rkMap.setTexture(rkRenderer.loadTexture(kImagePath, 0));
	width = atoi(pkImageElement->Attribute("width"));
	height = atoi(pkImageElement->Attribute("height"));
	rkMap.setScale(width, height);


	// add tiles
	columns = rkMap.scaleX() / uiTileWidth;
	rows = rkMap.scaleY() / uiTileHeight;
	MapWidth = columns*uiTileWidth;
	MapHeight = rows*uiTileHeight;
	maxTiles = atoi(pkTilesetElement->Attribute("tilecount"));
	std::map <int, Sprite*> subRects; //container of subrects (to divide the tilesheet image up)	
	std::pair<int, Sprite*> tile;

	std::vector<int> colisionTiles;
	tinyxml2::XMLElement* tilePropieties = pkTilesetElement->FirstChildElement("tile");
	while (tilePropieties){
		int tileIDCollidable = atoi(tilePropieties->Attribute("id"));
		colisionTiles.push_back(tileIDCollidable);
		tilePropieties = tilePropieties->NextSiblingElement("tile");
	}

	//tiles/subrects are counted from 0, left to right, top to bottom
	do{
		do{
			do{
				Sprite* rect = new Sprite();
				rect->setTexture(rkMap.m_kTexture);
				rect->setTile(rkMap.scaleX(), rkMap.scaleY(), x*uiTileWidth, y*uiTileHeight, uiTileWidth, uiTileHeight);
				tile.first = id;
				tile.second = rect;
				subRects.insert(tile);
				id++;
				x++;
			} while (x < columns);
			y++;
			x = 0;
		} while (y < rows);
	} while (id < maxTiles);

	//Layers
	tinyxml2::XMLElement*layerElement;
	layerElement = pkMapElement->FirstChildElement("layer");
	Layer layer;
	while (layerElement)
	{
		//Tiles
		tinyxml2::XMLElement* layerDataElement = layerElement->FirstChildElement("data");
		tinyxml2::XMLElement* tileElement = layerDataElement->FirstChildElement("tile");
	
		while (tileElement)
		{
			tileGID = atoi(tileElement->Attribute("gid"));
			subRectToUse = tileGID - 1;//Work out the subrect ID to 'chop up' the tilesheet image.
			if (subRectToUse >= 0)//we only need to (and only can) create a sprite/tile if there is one to display
			{
				Sprite *asc = new Sprite();
				asc->ID = subRectToUse;
				asc->_texture = subRects[subRectToUse]->_texture;
				asc->vertices = subRects[subRectToUse]->vertices;
				asc->setScale(uiTileWidth, uiTileHeight);
				asc->setPos((-MapWidth / 2 - uiTileWidth*(uiTileWidth / 2)) + i*uiTileWidth, (MapHeight / 2 + uiTileHeight) - j*uiTileHeight);

				bool coli = false;
				for (int i = 0; i < colisionTiles.size(); i++)
					if (subRectToUse == colisionTiles[i])
						coli = true;
				if (coli)
					rkMap._collisionTiles.push_back(asc);
				else
					rkMap._tiles.push_back(asc);		

			}

			tileElement = tileElement->NextSiblingElement("tile");

			//increment x, y
			i++;
			if (i >= uiMapWidth)//if x has "hit" the end (right) of the map, reset it to the start (left)
			{
				i = 0;
				j++;
				if (j >= uiMapHeight)
				{
					j = 0;
				}
			}
		}
		
		/*std::vector<Sprite*>::iterator iter;
		for (iter = rkMap._tiles.begin(); iter != rkMap._tiles.end(); iter++)
		{
			layer.tiles.push_back(*(*iter));
		}
		rkMap._layers.push_back(layer);*/

		layerElement = layerElement->NextSiblingElement("layer");
	}
	return true;
}