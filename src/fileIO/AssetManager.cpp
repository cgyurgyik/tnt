// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "fileIO/AssetManager.hpp"
#include "fileIO/VirtualFileSystem.hpp"

// TODO:
// use tnt::logger instead here.

tnt::AssetManager::~AssetManager()
{
    for (auto it : images)
        if (it.second != nullptr)
            SDL_DestroyTexture(it.second);
    images.clear();
    std::map<std::string, SDL_Texture *>{}.swap(images);

    for (auto it : text)
        if (it.second != nullptr)
            SDL_DestroyTexture(it.second);
    text.clear();
    std::map<std::string, SDL_Texture *>{}.swap(text);

    for (auto it : fonts)
        if (it.second != nullptr)
            TTF_CloseFont(it.second);
    fonts.clear();
    std::map<std::string, TTF_Font *>{}.swap(fonts);

    for (auto it : music)
        if (it.second != nullptr)
            Mix_FreeMusic(it.second);
    music.clear();
    std::map<std::string, Mix_Music *>{}.swap(music);

    for (auto it : sfx)
        if (it.second != nullptr)
            Mix_FreeChunk(it.second);
    sfx.clear();
    std::map<std::string, Mix_Chunk *>{}.swap(sfx);
}

SDL_Texture *tnt::AssetManager::LoadImage(SDL_Renderer *ren, std::string filename)
{
    SDL_Surface *temp{IMG_Load(filename.c_str())};
    if (temp == nullptr)
    {
        std::cout << "Couldn't load " << filename << "!\n\tError: " << IMG_GetError() << std::flush;
        return nullptr;
    }

    SDL_Texture *ret{SDL_CreateTextureFromSurface(ren, temp)};
    if (ret == nullptr)
    {
        std::cout << "Couldn't render " << filename << "!\n\tError: " << SDL_GetError() << std::flush;
        return nullptr;
    }

    SDL_FreeSurface(temp);

    return ret;
}

SDL_Texture *tnt::AssetManager::LoadText(SDL_Renderer *ren, TTF_Font *font, std::string text, SDL_Color color)
{
    SDL_Surface *surf{TTF_RenderText_Solid(font, text.c_str(), color)};
    if (surf == nullptr)
    {
        std::cout << "Couldn't load text \"" << text << "\".\n\tError: " << TTF_GetError() << std::flush;
        return nullptr;
    }

    SDL_Texture *tex{SDL_CreateTextureFromSurface(ren, surf)};
    if (tex == nullptr)
    {
        std::cout << "Couldn't render \"" << text << "\".\n\tError: " << SDL_GetError() << std::flush;
        return nullptr;
    }

    SDL_FreeSurface(surf);

    return tex;
}

void tnt::AssetManager::AddImage(SDL_Renderer *ren, std::string_view image)
{
    if (auto it{images.find(image.data())}; it != images.end() && it->second != nullptr)
        return;
    images[image.data()] = LoadImage(ren, image);
}

void tnt::AssetManager::AddFont(const std::string &font, int size)
{
    if (auto it{fonts.find(font)}; it != fonts.end() && it->second != nullptr)
        return;
    fonts[font] = TTF_OpenFont(font.c_str(), size);
}

void tnt::AssetManager::AddText(SDL_Renderer *ren, std::string const &filename, std::string const &text, int size, SDL_Color const &color)
{
    if (auto it{this->text.find(text)}; it != this->text.end() && it->second != nullptr)
        return;

    if (auto it{fonts.find(filename)}; it == fonts.end() || it->second == nullptr)
        AddFont(filename, size).wait();
    auto font{Font(filename)};
    this->text[text] = LoadText(ren, font.get(), text, color);
}

void tnt::AssetManager::AddMusic(const std::string &name)
{
    if (auto it{music.find(name)}; it != music.end() && it->second != nullptr)
        return;
    music[name] = Mix_LoadMUS(name.c_str());
}

void tnt::AssetManager::AddSfx(const std::string &chunk)
{
    if (auto it{sfx.find(chunk)}; it != sfx.end() && it->second != nullptr)
        return;
    sfx[chunk] = Mix_LoadWAV(chunk.c_str());
}

std::future<SDL_Texture *> tnt::AssetManager::Image(const std::string &image)
{
    return std::async(std::launch::deferred, [this, image] {
        AddImage(image).wait();
        return images[image];
    });
}

std::future<TTF_Font *> tnt::AssetManager::Font(std::string const &font, int size)
{
    return std::async(std::launch::deferred, [this, font, size] {
        AddFont(font, size).wait();
        return fonts[font];
    });
}