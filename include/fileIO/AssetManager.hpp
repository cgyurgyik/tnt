#ifndef TNT_ASSET_MANAGER_HPP
#define TNT_ASSET_MANAGER_HPP

#include <SDL2/SDL_mixer.h>
#include <string_view>

#include <map>

typedef struct _TTF_Font TTF_Font;
typedef struct SDL_Texture SDL_Texture;

namespace tnt
{
class Window;

// namespace tmx
// {
// class TileMap;
// } // namespace tmx

// TODO: add better key value for text SDL_Texture*-s.
// TODO: make everything here asynchronous.

// TODO(maybe)(x): make this a non-singleton. ??
// TODO(maybe): add an animations map. ??
// TODO(maybe): remove friend class AudioManager; ??
// TODO(maybe): use std::unordered_map instead ??
// TODO(maybe): use weak_ptr's for values to make the As

/// @brief The class responsible for caching and handling
/// assets like audio, fonts, textures and Tiled Maps.
class AssetManager final
{
public:
  /// @brief The destructor of @em AssetManager.
  ~AssetManager() noexcept;

  /// @brief The function used to access @em AssetManager's functionalities.
  /// @return AssetManager&
  static AssetManager &This();

  /// @brief Add @em font with size @em size to the @em fonts map,
  /// if it isn't already there.
  /// @param font The name of the font.
  /// @param size The size of the font.
  void AddFont(std::string_view font, int size);

  /// @brief Add @em image to the @em images map, if it isn't
  /// already on the map.
  /// @param win The window where @em image should be drawn.
  /// @param image The name of the image.
  void AddImage(Window const *win, std::string_view image);

  // void AddText(Window const *win, std::string_view filename,
  // std::string_view text, int size, SDL_Color const &color);

  /// @brief Add @em music to the @em musics map, if it
  /// isn't already there.
  /// @param music The name of the music.
  void AddMusic(std::string_view music);

  /// @brief Add @em chunk to the @em sfx map, if it
  /// isn't already there.
  /// @param chunk The name of the sfx file.
  void AddSfx(std::string_view chunk);

  // void AddMap(std::string_view name);

  /// @brief Add @em image to the @em images map if it isn't already there,
  /// and loads a texture from the image.
  /// @param win The window where the @em image should be drawn.
  /// @param image The name of the image.
  /// @return SDL_Texture*
  SDL_Texture *Image(Window const *win, std::string_view image);

  // SDL_Texture *Text(Window const *win, std::string_view txt);

  /// @brief Add @em font with size @em size to the @em fonts map
  /// if it isn't already on the map, and then returns the font.
  /// @param font The name of the font.
  /// @param size The size of the font.
  /// @return TTF_Font*
  TTF_Font *Font(std::string_view font, int size);

  /// @brief Add the music to the @em music map if it isn't
  /// already on the map, and then returns the music data.
  /// @param name The name of the music file.
  /// @return Mix_Music*
  Mix_Music *Music(std::string_view name);

  /// @brief Add the @em chunk to the @em sfx map if it isn't
  /// already on the map, and then returns the chunk data.
  /// @param chunk The name of the sfx to load.
  /// @return Mix_Chunk*
  Mix_Chunk *Sfx(std::string_view chunk);
  // tmx::TileMap *Map(std::string_view name);

private:
  AssetManager();

  // SDL_Texture *LoadText(Window const *win, TTF_Font *font,
  // std::string_view text, SDL_Color color);

  std::map<std::string, SDL_Texture *> images;
  std::map<std::string, SDL_Texture *> text;
  std::map<std::string, TTF_Font *> fonts;
  std::map<std::string, Mix_Music *> music;
  std::map<std::string, Mix_Chunk *> sfx;
  // std::map<std::string, tmx::TileMap *> maps;
};
} // namespace tnt

#endif //! TNT_ASSET_MANAGER_HPP