#pragma once

#include <vector>
#include <map>
#include <SDL_mixer.h>
#include <SDL.h>

#include "MediaManager.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"
#include "Player.hpp"
#include "Entity.hpp"
#include "Tile.hpp"
#include "Config.hpp"

class Map {
  Config *cfg;
  SDL_Renderer *ren;
  MediaManager *media;

  Waves *waves;

  map<string, Config *> entityConfs;
	vector<Entity *>entities;

	map<string,Config *> tileConfs;
	vector<Tile *>tiles;

  int startX,startY;

  public:
  Map(MediaManager *newMedia, SDL_Renderer *newRen, Config *newCfg) {
    media=newMedia;
    ren=newRen;
    cfg=newCfg;

    waves=new Waves(ren);

    entityConfs["basic"] = (new Config("entity"));
    entityConfs["big"] = (new Config("bigEntity"));
    
    tileConfs["tile"] = (new Config("tile"));
  }

  int getStartX() { return startX; }
  int getStartY() { return startY; }

  void placeObject(int x, int y, string type) {
    tiles.push_back(new Tile(media, ren, waves, tileConfs["tile"], type, x, y));
  }

  void spawnEntity(int x, int y, string type) {
		entities.push_back(new Entity(media, ren, waves, entityConfs[type], x, y));
	}

  void initMap(int x) {
    if (x==1) {
      startX=100;
      startY=480/2;
      for (int i=0; i<640; i+=stoi((*tileConfs["tile"])["width"]))
        placeObject(i,startY,"floor");
      
      for (int i=0; i<640; i+=stoi((*tileConfs["tile"])["width"]))
        placeObject(i,startY-128,"ceiling");
        
      for(int i=0; i<1; i++)
        spawnEntity(300+(i*50), startY, "basic");
    }
    else if (x==2) {
      startX=560;
      startY=480/2;
      
      for (int i=0; i<640; i+=stoi((*tileConfs["tile"])["width"]))
        placeObject(i,startY,"floor");

      for (int i=0; i<640; i+=stoi((*tileConfs["tile"])["width"]))
        placeObject(i,startY-256,"ceiling");
        
      for(int i=0; i<5; i++)
        spawnEntity(100+(i*50), startY, "big");
    }
    else if (x==3) {
      startX=640/2;
      startY=480/2;
      for (int i=0; i<640; i+=stoi((*tileConfs["tile"])["width"]))
        placeObject(i,startY,"floor");
      
      for (int i=0; i<640; i+=stoi((*tileConfs["tile"])["width"]))
        placeObject(i,startY-128,"ceiling");
        
      for (int i=startY; i>startY-128; i-=stoi((*tileConfs["tile"])["width"]))
        placeObject(640-200,i,"lWall");

      for (int i=startY; i>startY-128; i-=stoi((*tileConfs["tile"])["width"]))
        placeObject(200,i,"rWall");
    }
  }

  void updateEntities(double dt, Player *player) {
    for (auto& e:entities) e->update(dt, player->getX());

    // vector<int> locations;
    // for (int i=0; i<entities.size(); i++) {
    //   if (entities[i]->collide(player->getDest())) locations.push_back(i-locations.size());
    // }
    // for (auto i:locations) entities.erase(entities.begin()+i);
  }

  void onWall(Player *player) {
    for (auto &t:tiles) {
      if (t->collide(player->getDest())) player->hitWall(t);
    }
  }

  void update(double dt, Player *player) {
    waves->updateWaves(dt);
		updateEntities(dt, player);
		for (auto t:tiles) t->update(dt);
    onWall(player);
  }

  ~Map() {
    while (entities.size()>0) entities.erase(entities.begin());
    while (tiles.size()>0) tiles.erase(tiles.begin());
  }
};