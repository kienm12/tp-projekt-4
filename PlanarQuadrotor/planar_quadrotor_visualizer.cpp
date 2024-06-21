#include "planar_quadrotor_visualizer.h"
#include <cmath>
#pragma once

#include <memory>
#include <SDL.h>
#include <matplot/matplot.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
PlanarQuadrotorVisualizer::PlanarQuadrotorVisualizer(PlanarQuadrotor *quadrotor_ptr): quadrotor_ptr(quadrotor_ptr) {}


std::pair<int, int> PlanarQuadrotorVisualizer::convertToScreenCoordinates(float x, float y, int screenWidth, int screenHeight) {
    int screenX = static_cast<int>(screenWidth / 2 + x);
    int screenY = static_cast<int>(screenHeight / 2 - y);  
    return std::make_pair(screenX, screenY);
}



void PlanarQuadrotorVisualizer::render(std::shared_ptr<SDL_Renderer> &gRenderer) {
    Eigen::VectorXf state = quadrotor_ptr->GetState();
    float q_x, q_y, q_theta;

    /* x, y, theta koordynaty */
    q_x = state[0];
    q_y = state[1];
    q_theta = state[2];
    

    int screenWidth, screenHeight;
    SDL_GetRendererOutputSize(gRenderer.get(), &screenWidth, &screenHeight);

    auto [screenX, screenY] = convertToScreenCoordinates(q_x, q_y, screenWidth, screenHeight);
    q_x = screenX;
	q_y = screenY;

    Quadrotor quadrotor;
    static int last_tick = 0;
    static int mnoznik = 1;

    if (SDL_GetTicks() -last_tick > 200) 
    {
        last_tick = SDL_GetTicks();
        mnoznik = mnoznik==-1 ? 1 : -1;
    }

    quadrotor.wirnik.dist = { quadrotor.wirnik.odleglosc * std::cos(q_theta), quadrotor.wirnik.odleglosc * std::sin(q_theta) };
    quadrotor.wirnik.pozycja = { quadrotor.wirnik.wysokosc * std::sin(q_theta), quadrotor.wirnik.wysokosc * std::cos(q_theta) };
    quadrotor.left_top = { q_x - std::cos(q_theta) * (quadrotor.dlugosc / 2), q_y + std::sin(q_theta) * (quadrotor.dlugosc / 2) };
    quadrotor.right_bottom = { q_x + (quadrotor.dlugosc / 2) * std::cos(q_theta), q_y - std::sin(q_theta) * (quadrotor.dlugosc / 2) };
    quadrotor.wirnik.mocowanie_lewe_poczatek = { quadrotor.right_bottom.x - quadrotor.wirnik.dist.x, quadrotor.right_bottom.y + quadrotor.wirnik.dist.y };
    quadrotor.wirnik.mocowanie_prawe_poczatek = { quadrotor.left_top.x + quadrotor.wirnik.dist.x, quadrotor.left_top.y - quadrotor.wirnik.dist.y };
    quadrotor.wirnik.mocowanie_lewe_koniec = { quadrotor.wirnik.mocowanie_lewe_poczatek.x - quadrotor.wirnik.pozycja.x, quadrotor.wirnik.mocowanie_lewe_poczatek.y - quadrotor.wirnik.pozycja.y };
    quadrotor.wirnik.mocowanie_prawe_koniec = { quadrotor.wirnik.mocowanie_prawe_poczatek.x - quadrotor.wirnik.pozycja.x, quadrotor.wirnik.mocowanie_prawe_poczatek.y - quadrotor.wirnik.pozycja.y };
    quadrotor.wirnik.duza.left = { quadrotor.wirnik.mocowanie_lewe_koniec.x + (quadrotor.wirnik.duza.x * mnoznik), quadrotor.wirnik.mocowanie_lewe_koniec.y };
    quadrotor.wirnik.mala.left = { quadrotor.wirnik.mocowanie_lewe_koniec.x - (quadrotor.wirnik.mala.x * mnoznik), quadrotor.wirnik.mocowanie_lewe_koniec.y };
    quadrotor.wirnik.duza.right = { quadrotor.wirnik.mocowanie_prawe_koniec.x - (quadrotor.wirnik.duza.x * mnoznik), quadrotor.wirnik.mocowanie_prawe_koniec.y };
    quadrotor.wirnik.mala.right = { quadrotor.wirnik.mocowanie_prawe_koniec.x + (quadrotor.wirnik.mala.x * mnoznik), quadrotor.wirnik.mocowanie_prawe_koniec.y };

    int quadrotor_color = 0xFF133D86;
    int mocowanie_wirnika_color = 0xFF85C335;
    int wirnik_color1 = 0xFFFFB340;
    int wirnik_color2 = 0xFFD89735;

    //quadrotor 
    thickLineColor(gRenderer.get(), quadrotor.left_top.x, quadrotor.left_top.y, quadrotor.right_bottom.x, quadrotor.right_bottom.y, quadrotor.wysokosc, quadrotor_color);

    //Wirnik mocowanie
    thickLineColor(gRenderer.get(), quadrotor.wirnik.mocowanie_prawe_poczatek.x, quadrotor.wirnik.mocowanie_prawe_poczatek.y, quadrotor.wirnik.mocowanie_prawe_koniec.x, quadrotor.wirnik.mocowanie_prawe_koniec.y, quadrotor.wirnik.szerokosc, mocowanie_wirnika_color);
    thickLineColor(gRenderer.get(), quadrotor.wirnik.mocowanie_lewe_poczatek.x, quadrotor.wirnik.mocowanie_lewe_poczatek.y, quadrotor.wirnik.mocowanie_lewe_koniec.x, quadrotor.wirnik.mocowanie_lewe_koniec.y, quadrotor.wirnik.szerokosc, mocowanie_wirnika_color);

    //wirniki
    filledEllipseColor(gRenderer.get(), quadrotor.wirnik.duza.left.x, quadrotor.wirnik.duza.left.y, quadrotor.wirnik.duza.x, quadrotor.wirnik.duza.y, wirnik_color1);
    filledEllipseColor(gRenderer.get(), quadrotor.wirnik.mala.left.x, quadrotor.wirnik.mala.left.y, quadrotor.wirnik.mala.x, quadrotor.wirnik.mala.y, wirnik_color2);
    filledEllipseColor(gRenderer.get(), quadrotor.wirnik.duza.right.x, quadrotor.wirnik.duza.right.y, quadrotor.wirnik.duza.x, quadrotor.wirnik.duza.y, wirnik_color1);
    filledEllipseColor(gRenderer.get(), quadrotor.wirnik.mala.right.x, quadrotor.wirnik.mala.right.y, quadrotor.wirnik.mala.x, quadrotor.wirnik.mala.y, wirnik_color2);



    
}

