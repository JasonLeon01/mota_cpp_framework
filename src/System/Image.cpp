#include <Game/System/Image.hpp>

GameImage::GameImage() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->width = 0;
    this->height = 0;
    this->sx = 0;
    this->sy = 0;
    this->origin_x = 0;
    this->origin_y = 0;
    this->scale_x = 1.f;
    this->scale_y = 1.f;
    this->angle = 0;
    this->opacity = 255;
    this->visible = true;
    this->imgFile = "";
}

GameImage::GameImage(string file, float x, float y, int width, int height, int sx, int sy) : GameImage() {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->width = width;
    this->height = height;
    this->sx = sx;
    this->sy = sy;
    if (!filesystem::exists("graphics\\" + file)) {
        print(format("Haven't found file: {}", file));
        return;
    }
    this->imgFile = file;
}

void GameImage::setSprite(string file, float x, float y, int width, int height, int sx, int sy) {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->width = width;
    this->height = height;
    this->sx = sx;
    this->sy = sy;
    if (!filesystem::exists("graphics\\" + file)) {
        print(format("Haven't found file: {}", file));
        return;
    }
    this->imgFile = file;
}

void GameImage::show() {
    Sprite sprite(motaSystem.textureCache[imgFile]);
    if (width != 0 && height != 0) sprite.setTextureRect(IntRect(sx, sy, width, height));
    sprite.setPosition(x, y);
    sprite.setScale(scale_x, scale_y);
    sprite.setOrigin(origin_x, origin_y);
    sprite.setRotation(angle);
    auto cl = sprite.getColor();
    cl.a = opacity;
    sprite.setColor(cl);
    motaSystem.window.draw(sprite);
}
