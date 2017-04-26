This is an open-source arcade boat racing game with takedown mechanics.

**Check the [wiki](https://github.com/JayFoxRox/Boat-Racing-Game/wiki) for more information**


License TBD
Until then: (c) 2017 Jannik Vogel (JayFoxRox)

Code in the `data/` subfolder might be licensed differently. See the respective file headers.

Additionally the file `data/model/Daylight Box UV.png` is by [KIIRA](https://opengameart.org/content/sky-box-sunny-day) and licensed under [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).

# Screenshots

![Best graphics ever](http://i.imgur.com/VfotwQE.png)
![Realistic broken particle effect](http://i.imgur.com/ruuOuQY.png)

# How-to-build

Aside from a C++ compiler you need the following dependencies:

* CMake
* SDL2
* CURL
* OpenAL
* OpenGL (3.0)
* GLEW
* GLM
* libIL / DevIL
* assimp

On most platforms these simple commands will create a runnable game:

```
git clone https://github.com/JayFoxRox/Boat-Racing-Game.git
cd Boat-Racing-Game
mkdir build
cd build
cmake ..
make
```

Now you can run the generated executable called `main`
