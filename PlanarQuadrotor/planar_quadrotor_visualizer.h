#pragma once

#include <memory>

#include <SDL.h>
#include <matplot/matplot.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>

#include "planar_quadrotor.h"

struct Punkt {
    float x;
    float y;
};

struct Elipsa {
    int x;
    int y;
    Punkt left;
    Punkt right;
};

struct Wirnik {
    int szerokosc = 6;
    int wysokosc = 16;
    int odleglosc = 8;

    Elipsa mala = { 10, 5 };
    Elipsa duza = { 18, 8 };

    Punkt pozycja;
    Punkt dist;
    Punkt mocowanie_prawe_poczatek;
    Punkt mocowanie_lewe_poczatek;
    Punkt mocowanie_prawe_koniec;
    Punkt mocowanie_lewe_koniec;
};
struct Quadrotor {
    int dlugosc = 110;
    int wysokosc = 10;
    Wirnik wirnik;
    Punkt right_bottom;
    Punkt left_top;
};


class PlanarQuadrotorVisualizer {
private:
    PlanarQuadrotor *quadrotor_ptr;

public:
    PlanarQuadrotorVisualizer(PlanarQuadrotor *quadrotor_ptr);
    std::pair<int, int> PlanarQuadrotorVisualizer::convertToScreenCoordinates(float x, float y, int screenWidth, int screenHeight);
    void render(std::shared_ptr<SDL_Renderer> &gRenderer);
};