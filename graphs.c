#include <cairo.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include "stocks.h"
// #include "download.h"
// #include <cjson/cJSON.h>

typedef struct {
  MarketStockList **stockData;
  size_t symbols;
  char *title;
  int high;
} GraphData;

GraphData *getValues(const char *queryString, size_t *len) {
  graph->stockData = queryStocks(“AAPL,GOOG”, &numSymbols);
  graph->title = queryString;

  int highestNum = 0;
  MarketStockList *curr = stockData[i];

  for (int i = 0; i < len; i++) {
    for (int j = 0; j < StockList_getLen(stockData[i]); j++) {
      if((int)(curr->stock->value) > highestNum) {
        highestNum = (int)(curr->stock->value);
      }
      curr = curr->next;
    }
  }
  graph->high = highestNum;

  return graph;
}

int getGraph(float upper_x, float upper_y, char *title) {
  SDL_Surface *sdlsurf = SDL_CreateRGBSurface (0, 800, 600, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);

  SDL_Window *window = SDL_CreateWindow ("An SDL2 window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  cairo_surface_t *surface = cairo_image_surface_create_for_data (sdlsurf->pixels, CAIRO_FORMAT_RGB24, sdlsurf->w, sdlsurf->h, sdlsurf->pitch);
  cairo_t *cr;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  cr = cairo_create (surface);

  //Drawing Stuff

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint(cr);

  cairo_set_source_rgb (cr, 0, 0, 0);

  cairo_move_to (cr, 100, 75);
  cairo_select_font_face (cr, "sans_serif", 0, 1);
  cairo_set_font_size (cr, 32);
  cairo_show_text (cr, title);

  cairo_move_to (cr, 400, 525);
  cairo_set_font_size (cr, 20);
  cairo_show_text (cr, "Time");

  cairo_move_to (cr, 80, 350);
  cairo_rotate (cr, -M_PI/2);
  cairo_show_text (cr, "Stock Price");

  cairo_rotate (cr, M_PI/2);
  cairo_move_to (cr, 700, 500);
  cairo_line_to (cr, 100, 500);
  cairo_line_to (cr, 100, 100);
  cairo_stroke (cr);

  //End of Drawing Stuff

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, sdlsurf);
  SDL_FreeSurface(sdlsurf);

  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        done = true;
        break;
      default:
        break;
      }
    }
    SDL_Delay(100);
  }

  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  printf("Success!\n");
  return 0;
}

int main(int argc, char *argv[]) {
  GraphData *graph;
  getValues("AAPL,GOOG", &graph->symbols);
  getGraph(0, 0, "AAPL Stock");
}
