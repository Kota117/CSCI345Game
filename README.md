# CSCI345Game

## Referencing Media
If you need to load an animation you can do so with the following:
```animationName->readAnimation(<filename>)```
The filename should only contain the name of the file. It should not contain any path information or a file extension.
Do: "walkRight"
Don't do: "media/animations/walkRight.txt"

If you need to load a sound you can do so with the following:
```media->readSound(<filename>)```
The filename should only contain the name of the file. It should not contain any path information or a file extension.
Do: "footstep"
Don't do: "media/sound/footstep.wav"

If you need to load an image you can do so with the following:
```media->readImage(<filename>)```
The filename should only contain the name of the file. It should not contain any path information or a file extension.
Do: "walkCycle"
Don't do: "media/images/walkCycle.bmp"


## The Media Folder
The "animations" folder contains the .txt file descriptions of animations. This folder does not contain any of the actual sprite assets used in the animations.

The "images" folder contains all image assets for the game. This folder contains all of the sprite sheets used in animation descriptions. 

The "sounds" folder contains the sounds used in the game. This folder also contains background music.
