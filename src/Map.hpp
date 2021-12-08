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
#include "NPC.hpp"
#include "Tile.hpp"
#include "Config.hpp"
#include "Lightning.hpp"

class Map {
  Config *cfg;
  SDL_Renderer *ren;
  MediaManager *media;

  Waves *waves;

  map<string, Config *>npcConfs;
	vector<Npc *>npcs;

	map<string,Config *> tileConfs;
	vector<Tile *>tiles;

  Config *lightningConf;
  Lightning *lightning;

  int playerStartX, playerStartY;
  int tileWidth;



  public:
  Map(MediaManager *newMedia, SDL_Renderer *newRen, Waves* newWaves, Config *newCfg) {
    media=newMedia;
    ren=newRen;
    cfg=newCfg;

    waves=newWaves;
		
    npcConfs["basic"] = (new Config("npc"));
    npcConfs["big"] = (new Config("bigNpc"));
    
    tileConfs["tile"] = (new Config("tile"));
    tileWidth=stoi((*tileConfs["tile"])["width"]);

    lightningConf = new Config("lightning");
    lightning=new Lightning(media, ren, lightningConf);
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
      spawnNpc(x, y+tileWidth, type);
    }else if(type!="empty"){
      tiles.push_back(new Tile(media, ren, tileConfs["tile"], type, x, y));
    }
  }

  void spawnNpc(int x, int y, string type) {
		npcs.push_back(new Npc(media, ren, waves, npcConfs[type], x, y));
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
        placeX+=tileWidth-5;
      }
      placeX=0;
      placeY+=tileWidth;
    }
  }

  void updateNpcs(double dt, Player *player) {
    for (auto& e:npcs) e->update(dt, player->getX());

    vector<int> locations;
    for (int i=0; i<npcs.size(); i++) {
      if (npcs[i]->collide(player->getDest())) locations.push_back(i-locations.size());
    }

    for (auto i:locations) npcs.erase(npcs.begin()+i);
  }
  
  void hitFloor(Player *player){
    for (auto t:tiles) {
      if (t->collide(player->getDest()) && (t->getType() == "floor")){
        if(!player->isGrounded())
          player->stopFalling();
        player->setY(player->getY()-1);
      }else if (t->collide(player->getDest()) && (t->getType() == "ceiling")){
        player->setVY(0);
        player->setY(player->getY()+1);
      }
    }
  }
  

  void onWall(Player *player) {
    for (auto &t:tiles) {
      if (t->collide(player->getDest())) player->hitWall(t);
    }
  }

  void update(double dt, Player *player) {
    bool hasCollision = false;
    waves->updateWaves(dt);
    
		updateNpcs(dt, player);
    if (rand()%1000==0) { 
      lightning->update(dt,true,rand()%300+100);
      for (auto t:tiles) t->lightUp(); 
    }
    else lightning->update(dt);
		for (auto t:tiles){
      t->update(dt);
      hasCollision = waves->collideSound(t);
      if(hasCollision){
        t->collide(t->getDest());
        hasCollision=false;
      }
    }
    //player->handleCollision(tiles, dt);
    //player->setGrounded(player->checkGrounded(tiles));
    onWall(player);
    hitFloor(player);
  }

  void render(Player *player){
    waves->renderWaves();
    player->render();
    lightning->render();
    for (auto t:tiles) t->render();
    for (auto e:npcs) e->render();
  }
 
  ~Map() {
    while (npcs.size()>0) npcs.erase(npcs.begin());
    while (tiles.size()>0) tiles.erase(tiles.begin());
    waves->deleteWaves();
  }
};