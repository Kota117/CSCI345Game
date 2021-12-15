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
#include "Key.hpp"
#include "Tile.hpp"
#include "Config.hpp"
#include "Lightning.hpp"


class Map{
    Config *cfg;
    SDL_Renderer *ren;
    MediaManager *media;

    Waves *waves;

    map<string, Config *>npcConfs;
    vector<Npc *>npcs;

    map<string, Config *>keyConfs;
    vector<Key *>keys;

    map<string,Config *> tileConfs;
    vector<Tile *>tiles;

    Config *lightningConf;
    Lightning *lightning;

    int playerStartX, playerStartY;
    int tileWidth;

    public:
    Map(MediaManager *newMedia, SDL_Renderer *newRen, Waves* newWaves, Config *newCfg){
        media = newMedia;
        ren = newRen;
        cfg = newCfg;

        waves = newWaves;
        
        npcConfs["basic"] = (new Config("npc"));
        npcConfs["big"] = (new Config("bigNpc"));
        
        tileConfs["tile"] = (new Config("tile"));
        tileWidth=stoi((*tileConfs["tile"])["width"]);

        keyConfs["key"] = (new Config("key"));

        lightningConf = new Config("lightning");
        lightning = new Lightning(media, ren, lightningConf);
    }

    Tile *operator[] (int index){
        return tiles[index];
    } 

    int getStartX(){ return playerStartX; }
    int getStartY(){ return playerStartY; }

    void placeTile(int x, int y, string type){
        if(type=="player"){
            playerStartX = x;
            playerStartY = y+16;
        } else if(type=="basic" || type=="big"){
            spawnNpc(x, y+tileWidth, type);
        } else if(type=="key"){
            spawnKey(x, y+tileWidth, type);
        } else if(type!="empty"){
            tiles.push_back(new Tile(media, ren, tileConfs["tile"], type, x, y));
        }
    }

    void spawnNpc(int x, int y, string type){
        npcs.push_back(new Npc(media, ren, waves, npcConfs[type], x, y));
    }
    void spawnKey(int x, int y, string type){
        keys.push_back(new Key(media, ren, waves, keyConfs[type],x, y));
    }

    void initMap(int levelNum) {
        string levelName = "level"+to_string(levelNum);
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
                        tileType = "lWall";
                        break;
                    case 'r': //right wall
                        tileType = "rWall";
                        break;
                    case 'f': //floor
                        tileType = "floor";
                        break;
                    case 'c': //ceiling
                        tileType = "ceiling";
                        break;
                    case 'p': //player
                        tileType = "player";
                        break;
                    case 'e': //enemy (basic)
                        tileType = "basic";
                        break;
                    case 'b': //big enemy
                        tileType = "big";
                        break;
                    case 'k':
                        tileType = "key";
                        break;
                    case 'd':
                        tileType = "door";
                        break;
                    default:
                        tileType = "empty";
                }

                placeTile(placeX, placeY, tileType);
                placeX += tileWidth;
            }

            placeX = 0;
            placeY += tileWidth;
        }
    }

    void updateNpcs(double dt, Player *player){
        for (auto& e:npcs){
            e->update(dt, player->getX());
            e->collisions(tiles);
        }

        vector<int> locations;

        for (int i=0; i<npcs.size(); i++){
            if (npcs[i]->collide(player->getDest())) locations.push_back(i-locations.size());
        }

        for (auto i:locations) npcs.erase(npcs.begin()+i);
      }

    void updateKey(double dt, Player *player){
        for (auto& k:keys) k->update(dt, player->getX());

        vector<int> locations;

        for (int i=0; i<keys.size(); i++){
            if(keys[i]->collide(player->getDest())){
                locations.push_back(i-locations.size());
                player->collectedKey();
            }
        }

        for (auto i:locations) keys.erase(keys.begin()+i);
    }

    void update(double dt, Player *player){
        bool hasCollision = false;
        waves->updateWaves(dt);
        
        updateNpcs(dt, player);
        updateKey(dt, player);

        if (rand()%1001==0){
            lightning->update(dt,tiles,true,rand()%300+100);
        } else lightning->update(dt,tiles);

        for (auto &t:tiles){
            t->update(dt);
            hasCollision = waves->collideSound(t);
            if(hasCollision){
                t->collide(t->getDest());
                hasCollision = false;
            }
        }
        //player->handleCollision(tiles, dt);
        //player->setInAir(player->checkInAir(tiles));
        player->collisions(tiles);
    }

    void render(Player *player){
        waves->renderWaves();

        for (auto t:tiles) t->render();

        player->render();
        lightning->render();

        for (auto e:npcs) e->render();
        for (auto k:keys) k->render();
    }
  
    ~Map(){
        while (npcs.size()>0) npcs.erase(npcs.begin());
        while (tiles.size()>0) tiles.erase(tiles.begin());

        waves->deleteWaves();
    }
};