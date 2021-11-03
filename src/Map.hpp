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
#include "Object.hpp"
#include "Config.hpp"

class Map {
  Config *cfg;
  SDL_Renderer *ren;
  MediaManager *media;

  Waves *waves;

  map<string, Config *> entityConfs;
	vector<Entity *>entities;

	map<string,Config *> objectConfs;
	vector<Object *>objects;

  int startX,startY;

  public:
  Map(MediaManager *newMedia, SDL_Renderer *newRen, Config *newCfg) {
    media=newMedia;
    ren=newRen;
    cfg=newCfg;

    waves=new Waves(ren);

    entityConfs["basic"] = (new Config("entity"));
    entityConfs["big"] = (new Config("bigEntity"));
    objectConfs["floor"] = (new Config("floor"));
  }

  int getStartX() { return startX; }
  int getStartY() { return startY; }

  void placeFloor(int x, int y, string type) {
    objects.push_back(new Object(media, ren, waves, objectConfs[type], type, x, y));
  }

  void spawnEntity(int x, int y, string type) {
		entities.push_back(new Entity(media, ren, waves, entityConfs[type], x, y));
	}

  void initMap(int x) {
    if (x==1) {
      startX=100;
      startY=480/2;
      for (int i=0; i<640; i+=stoi((*objectConfs["floor"])["width"]))
        placeFloor(i,startY,"floor");
        
      for(int i=0; i<5; i++)
        spawnEntity(300+(i*50), startY, "basic");
    }
    else if (x==2) {
      startX=560;
      startY=480/2;
      
      for (int i=0; i<640; i+=stoi((*objectConfs["floor"])["width"]))
        placeFloor(i,startY,"floor");
        
      for(int i=0; i<5; i++)
        spawnEntity(100+(i*50), startY, "big");
    }
  }

  void updateEntities(double dt, Player *player) {
    for (auto& e:entities) e->update(dt, player->getX());

    vector<int> locations;
    for (int i=0; i<entities.size(); i++) {
      if (entities[i]->collide(player->getDest())) locations.push_back(i-locations.size());
    }
    for (auto i:locations) entities.erase(entities.begin()+i);
  }

  void update(double dt, Player *player) {
    waves->updateWaves(dt);
		updateEntities(dt, player);
		for (auto o:objects) o->update(dt);
  }

  ~Map() {
    while (entities.size()>0) entities.erase(entities.begin());
    while (objects.size()>0) objects.erase(objects.begin());
  }
};