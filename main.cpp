#include "main.h"

bool init()
{
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    // Create window
    gWindow = SDL_CreateWindow("SDLKEYHANDLER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL )
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
      else
      {
        // Initialize renderer color
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
          printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
          success = false;
        }

        // gScreenSurface = SDL_GetWindowSurface( gWindow );
      }
    }
  }

  return success;
}

SDL_Texture* loadTexture( std::string path ) {
  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load(path.c_str());

  // IMG_LoadTexture makes it easier

  if(loadedSurface == NULL)
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  } else {
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

    if(newTexture == NULL)
    {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  return newTexture;
}

bool loadMedia()
{
  bool loading = true;

  gTexture = loadTexture( "lib/texture.png" );

  if( gTexture == NULL )
  {
    printf( "Failed to load texture!\n" );
    loading = false;
  }

  return loading;
}

void close()
{
  // Free allocated memory
  SDL_DestroyTexture(gTexture);
  gTexture = NULL;

  // Destroy window    
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);

  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char *args[])
{
  // Start up SDL and create window
  if (!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    // Load media
    if (!loadMedia())
    {
      printf("Failed to load media!\n");
    }
    else
    {
      // Hack to get window to stay up
      SDL_Event e;

      bool quit = false;

      while (!quit)
      {
        while (SDL_PollEvent(&e) != 0)
        {
          if (e.type == SDL_QUIT) {
            quit = true;
          }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        // Render texture to screen
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}