#include <cairo.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>

int main(int argc, char *argv[]) {
  SDL_Surface *sdlsurf = SDL_CreateRGBSurface (0, 500, 500, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);

  cairo_surface_t *surface = cairo_image_surface_create_for_data (sdlsurf->pixels, CAIRO_FORMAT_RGB24, sdlsurf->w, sdlsurf->h, sdlsurf->pitch);
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 0.9, 0.8, 0.9);
  cairo_paint(cr);

  cairo_set_source_rgb (cr, 0, 0.5, 0.8);

  cairo_move_to (cr, 10, 10);
  cairo_line_to (cr, 40, 10);
  cairo_rel_line_to (cr, -15, 30);
  cairo_close_path(cr);
  cairo_fill (cr);

  //make a heart
  cairo_move_to (cr, 100, 50);
  cairo_line_to (cr, 120, 75);
  cairo_line_to (cr, 140, 50);
  cairo_arc (cr, 130, 50, 10, M_PI, 0);
  cairo_arc (cr, 110, 50, 10, M_PI, 0);
  cairo_close_path(cr);
  cairo_fill (cr);


  cairo_destroy (cr);
  cairo_surface_write_to_png (surface, "test.png");
  cairo_surface_destroy (surface);
  printf("Success\n");
  return 0;
}
