Appleseed Environment Texture
======
**Appleseed Environment Texture** is a texture plugin for 3ds max to be used in [appleseed renderer](https://github.com/appleseedhq/appleseed-max) as an enviroment map

## Things to implement
* Parameter Blocks for one or both sky models of appleseed renderer
* Add an interface implementation to properly identify the texture in the scene
* Implement parameter parsing in [appleseed-max's](https://github.com/appleseedhq/appleseed-max) projectbuilder
* Bind selected direct light to appleseed's Sun Light object and Sky environment EDF

## Additional things
* Convert 3ds max env. maps to appleseed environment textures with correct mapping
* Convert spherical maps to latitude-longitude environment EDF
* Convert screen maps to Background Environment Shader