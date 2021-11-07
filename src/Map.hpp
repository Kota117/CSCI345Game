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

  int playerStartX, playerStartY;
  int tileWidth;

  

  public:
  Map(MediaManager *newMedia, SDL_Renderer *newRen, Waves* newWaves, Config *newCfg) {
    media=newMedia;
    ren=newRen;
    cfg=newCfg;

    waves=newWaves;
		
    entityConfs["basic"] = (new Config("entity"));
    entityConfs["big"] = (new Config("bigEntity"));
    
    tileConfs["tile"] = (new Config("tile"));
    tileWidth=stoi((*tileConfs["tile"])["width"]);
  }

  Tile *operator[] (int index) {
    return tiles[index];
  } 

  int getStartX() { return playerStartX; }
  int getStartY() { return playerStartY; }

  void placeTile(int x, int y, string type) {
    if(type=="player"){
      playerStartX=x;
      playerStartY=y;
    }else if(type=="basic" || type=="big"){
      spawnEntity(x, y+tileWidth, type);
    }else if(type!="empty"){
      tiles.push_back(new Tile(media, ren, tileConfs["tile"], type, x, y));
    }
  }

  void spawnEntity(int x, int y, string type) {
		entities.push_back(new Entity(media, ren, waves, entityConfs[type], x, y));
	}

  void initMap(string levelName) {
    ifstream inf("levels/"+levelName+".txt");
    string mapRow = "";

    int placeX = 0;
    int placeY = 0;

    string tileType = "";

    while(!inf.eof()){
      getline(inf, mapRow);
      for(char &c:mapRow){
        switch(c){
          case 'l': //left wall
            tileType="lWall";
            break;

          case 'r': //right wall
            tileType="rWall";
            break;

          case 'f': //floor
            tileType="floor";
            break;

          case 'c': //ceiling
            tileType="ceiling";
            break;

          case 'p': //player
            tileType="player";
            break;

          case 'e': //enemy (basic)
            tileType="basic";
            break;

          case 'b': //big enemy
            tileType="big";
            break;

          default:
            tileType="empty";
        }
        placeTile(placeX, placeY, tileType);
        placeX+=tileWidth;
      }
      placeX=0;
      placeY+=tileWidth;
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
  
  void hitFloor(Player *player){
    for (auto t:tiles) {
      if (t->collide(player->getDest()) && (t->getType() == "floor")){
        player->stopFalling();
        player->setY(player->getY()-1);
      }else if (t->collide(player->getDest()) && (t->getType() == "ceiling")){
        player->setVY(0);
        player->setY(player->getY()+1);
      }
    }
  }

  void onWall(Player *player) {
    for (auto t:tiles) {
      if (t->collide(player->getDest()) && (t->getType() == "lWall" || t->getType() == "rWall")) {
        player->stopMoving();
        if (t->getType() == "lWall") player->setX(player->getX()-1);
        else if (t->getType() == "rWall") player->setX(player->getX()+1);
      }
    }
  }

  void update(double dt, Player *player) {
    waves->updateWaves(dt);
    
		updateEntities(dt, player);
		for (auto t:tiles){
      t->update(dt);
      waves->collideSound(t);
    }
    onWall(player);
    hitFloor(player);
  }
 
  ~Map() {
    while (entities.size()>0) entities.erase(entities.begin());
    while (tiles.size()>0) tiles.erase(tiles.begin());
    waves->deleteWaves();
  }
};